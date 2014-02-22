/**
 * @file afsk.c
 * @brief Modem AFSK 1200 Bd
 * @author Francesco Sacchi <batt@develer.com>
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 */
#include <string.h>
#include <errno.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#include "afsk.h"
#include "queue.h"

#include "avrio-config.h"
#include "avrio-board-afsk.h"
#include "avrio-cfg-afsk.h"

#if 0
#define LOG_LEVEL  AX25_LOG_LEVEL
#define LOG_FORMAT AX25_LOG_FORMAT
#include <cfg/log.h>
#else
#define LOG_INFO(str,...)
#define LOG_ERR(str,...)
#define LOG_WARN(str,...)
#endif

/* configuration ============================================================ */
/*
 * ADC sample rate.
 * The demodulator filters are designed to work at this frequency.
 * If you need to change this remember to update vAddAdcSample().
 */
#define AFSK_SAMPLERATE 9600

/*
 * Bitrate of the received/transmitted data.
 * The demodulator filters and decoderes are designed to work at this frequency.
 * If you need to change this remember to update vAddAdcSample().
 */
#define AFSK_BITRATE    1200

/* constants ================================================================ */
#define AFSK_SAMPLEPERBIT (AFSK_SAMPLERATE / AFSK_BITRATE)

#define PHASE_BIT    8
#define PHASE_INC    1

#define PHASE_MAX    (AFSK_SAMPLEPERBIT * PHASE_BIT)
#define PHASE_THRES  (PHASE_MAX / 2) // - PHASE_BIT / 2)

// Modulator constants
#define MARK_FREQ  1200
#define MARK_INC   (uint16_t)(DIV_ROUND(SIN_LEN * (uint32_t)MARK_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define SPACE_FREQ 2200
#define SPACE_INC  (uint16_t)(DIV_ROUND(SIN_LEN * (uint32_t)SPACE_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define DAC_SAMPLEPERBIT (CONFIG_AFSK_DAC_SAMPLERATE / AFSK_BITRATE)

#define BIT_STUFF_LEN 5

#define SIN_LEN 512 //< Full wave length

/* structures =============================================================== */

/*
 * HDLC (High-Level Data Link Control) context.
 * Maybe to be moved in a separate HDLC module one day.
 */
typedef struct Hdlc {
  uint8_t demod_bits; //< Bitstream from the demodulator.
  uint8_t bit_idx;    //< Current received bit.
  uint8_t currchar;   //< Current received character.
  bool rxstart;       //< True if an HDLC_FLAG char has been found in the bitstream.
} Hdlc;

/*
 * AFSK1200 modem context.
 */
typedef struct xAfsk {

  /* Current sample of bit for output data. */
  uint8_t sample_count;

  /* Current character to be modulated */
  uint8_t curr_out;

  /* Mask of current modulated bit */
  uint8_t tx_bit;

  /* True if bit stuff is allowed, false otherwise */
  bool bit_stuff;

  /* Counter for bit stuffing */
  uint8_t stuff_cnt;
  /*
   * DDS phase accumulator for generating modulated data.
   */
  uint16_t phase_acc;

  /* Current phase increment for current modulated bit */
  uint16_t phase_inc;

  /*
   * Delay line used to delay samples by (AFSK_SAMPLEPERBIT / 2)
   * The 1 is added because the FIFO macros need
   * 1 byte more to handle a buffer (AFSK_SAMPLEPERBIT / 2) bytes long.
   */
  xQueue delay_fifo;
  uint8_t delay_buffer[AFSK_SAMPLEPERBIT / 2 + 1];

  /* FIFO for received data */
  xQueue rx_fifo;
  uint8_t rx_buffer[CONFIG_AFSK_RX_BUFLEN];

  /* FIFO for transmitted data */
  xQueue tx_fifo;
  uint8_t tx_buffer[CONFIG_AFSK_TX_BUFLEN];

  /* IIR filter X cells, used to filter sampled data by the demodulator */
  int16_t iir_x[2];

  /* IIR filter Y cells, used to filter sampled data by the demodulator */
  int16_t iir_y[2];

  /*
   * Bits sampled by the demodulator are here.
   * Since ADC samplerate is higher than the bitrate, the bits here are
   * AFSK_SAMPLEPERBIT times the bitrate.
   */
  uint8_t sampled_bits;

  /*
   * Current phase, needed to know when the bitstream at ADC speed
   * should be sampled.
   */
  int8_t curr_phase;

  /* Bits found by the demodulator at the correct bitrate speed. */
  uint8_t found_bits;

  /* True while modem sends data */
  volatile bool sending;

  /*
   * AFSK modem mode.
   */
  uint8_t mode;

  volatile int8_t error;

  /* Hdlc context */
  Hdlc hdlc;

  /*
   * Preamble length.
   * When the AFSK modem wants to send data, before sending the actual data,
   * shifts out preamble_len HDLC_FLAG characters.
   * This helps to synchronize the demodulator filters on the receiver side.
   */
  uint16_t preamble_len;

  /*
   * Trailer length.
   * After sending the actual data, the AFSK shifts out
   * trailer_len HDLC_FLAG characters.
   * This helps to synchronize the demodulator filters on the receiver side.
   */
  uint16_t trailer_len;

  /*
   * Indique que le préambule est terminé et que la réception du payload
   * est en cours.
   */
  // bool endof_rx_preamble;
} xAfsk;

/* private variables ======================================================== */
/*
 * Sine table for the first quarter of wave.
 * The rest of the wave is computed from this first quarter.
 * This table is used to generate the modulated data.
 */
static const uint8_t PROGMEM sin_table[] = {
  128, 129, 131, 132, 134, 135, 137, 138, 140, 142, 143, 145, 146, 148, 149, 151,
  152, 154, 155, 157, 158, 160, 162, 163, 165, 166, 167, 169, 170, 172, 173, 175,
  176, 178, 179, 181, 182, 183, 185, 186, 188, 189, 190, 192, 193, 194, 196, 197,
  198, 200, 201, 202, 203, 205, 206, 207, 208, 210, 211, 212, 213, 214, 215, 217,
  218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
  234, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 243, 244, 245,
  245, 246, 246, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 252,
  253, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255
};

static xAfsk af;

/* STATIC_ASSERT ============================================================ */
#if (CONFIG_AFSK_DAC_SAMPLERATE % AFSK_BITRATE) != 0
#error "The DAC sample rate must be a multiple of bit rate"
#endif
#if AFSK_SAMPLEPERBIT != 8
#error "The number of samples per bit must be equal to 8"
#endif

// The size of sin_table must be the quarter of full wave length
STATIC_ASSERT (sizeof(sin_table) == (SIN_LEN / 4), sin_table_bad_size)

/* macros =================================================================== */
#define BIT_DIFFER(bitline1, bitline2) (((bitline1) ^ (bitline2)) & 0x01)
#define EDGE_FOUND(bitline)  BIT_DIFFER((bitline), (bitline) >> 1)
#define SWITCH_TONE(inc)  (((inc) == MARK_INC) ? SPACE_INC : MARK_INC)
#ifndef ASSERT
#define ASSERT(b)
#endif

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vStartTx(void) {

  if (!af.sending) {

    af.phase_inc = MARK_INC;
    af.phase_acc = 0;
    af.stuff_cnt = 0;
    af.sending = true;
    af.preamble_len = DIV_ROUND(CONFIG_AFSK_PREAMBLE_LEN * AFSK_BITRATE, 8000);
    vAfskTxEnable();
  }
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

    af.trailer_len  = DIV_ROUND(CONFIG_AFSK_TRAILER_LEN  * AFSK_BITRATE, 8000);
  }
}

// -----------------------------------------------------------------------------
static int
iGetChar (FILE * pxStream) {

  if (af.error) {

    errno = af.error;
    af.error = 0;
    return _FDEV_ERR;
  }

  if (af.mode & AFSK_MODE_NOBLOCK) {

    if (!xQueueIsEmpty (&af.rx_fifo)) {

      return (unsigned int) ucQueuePull (&af.rx_fifo);
    }
  }
  else {

    while (xQueueIsEmpty (&af.rx_fifo)) {

#if CONFIG_AFSK_RXTIMEOUT != -1
      // Timeout
#endif
      wdt_reset();
    }
    return (unsigned int) ucQueuePull (&af.rx_fifo);
  }

  return _FDEV_EOF;
}

// -----------------------------------------------------------------------------
static int
iPutChar (char c, FILE * pxStream) {

  vQueueWaitUntilIsFull (&af.tx_fifo);
//  vQueueLock (&af.tx_fifo, QUEUE_LOCK_WR);
  vQueuePush (&af.tx_fifo, c);
  vStartTx();
//  vQueueUnlock (&af.tx_fifo, QUEUE_LOCK_WR);
  return 0;
}

// -----------------------------------------------------------------------------
// Given the index, this function computes the correct sine sample
// based only on the first quarter of wave.
// @return sinus value from 0 to 255
__STATIC_ALWAYS_INLINE (uint8_t
  ucSinSample (uint16_t idx)) {

  ASSERT(idx < SIN_LEN);
  uint16_t new_idx = idx % (SIN_LEN / 2);
  new_idx = (new_idx >= (SIN_LEN / 4)) ? (SIN_LEN / 2 - new_idx - 1) : new_idx;

  uint8_t data = pgm_read_byte(&sin_table[new_idx]);

  return (idx >= (SIN_LEN / 2)) ? (255 - data) : data;
}

/* -----------------------------------------------------------------------------
 * High-Level Data Link Control parsing function.
 * Parse bitstream in order to find characters.
 *
 * @param bit  current bit to be parsed.
 * @return 0 if all is ok, AFSK_RXFIFO_OVERRUN if the fifo is full.
 */
static int8_t
iHdlcParse (bool bit) {
  int8_t ret = 0;

  af.hdlc.demod_bits <<= 1;
  af.hdlc.demod_bits |= bit ? 1 : 0;

  /* HDLC Flag */
  if (af.hdlc.demod_bits == HDLC_FLAG) {

    // Stocke le flag dans la pile
    if (!xQueueIsFull(&af.rx_fifo)) {

      vQueuePush(&af.rx_fifo, HDLC_FLAG);
      af.hdlc.rxstart = true;
    }
    else {

      ret = AFSK_RXFIFO_OVERRUN;
      af.hdlc.rxstart = false;
    }

    af.hdlc.currchar = 0;
    af.hdlc.bit_idx = 0;
    return ret; // Sortie sur HDLC_FLAG
  }

  /* Reset */
  if ((af.hdlc.demod_bits & HDLC_RESET) == HDLC_RESET) {

    af.hdlc.rxstart = false;
    return ret; // Sortie sur HDLC_RESET
  }

  if (!af.hdlc.rxstart) {

    // Sortie réception d'un octet != HDLC_FLAG alors que
    // premier HDLC_FLAG non reçu
    return ret;
  }

  /* Stuffed bit */
  if ((af.hdlc.demod_bits & 0x3f) == 0x3e) {

    return ret; // Sortie sur bit de bourrage
  }

  if (af.hdlc.demod_bits & 0x01) {

    af.hdlc.currchar |= 0x80;
  }

  if (++af.hdlc.bit_idx >= 8) {

    // Un octet reçu
    if ((af.hdlc.currchar == HDLC_FLAG
         || af.hdlc.currchar == HDLC_RESET
         || af.hdlc.currchar == AX25_ESC)) {

      // Si octect est un octet spécial, ajouter un caractère d'échapement avant
      if (!xQueueIsFull(&af.rx_fifo)) {

        vQueuePush(&af.rx_fifo, AX25_ESC);
      }
      else {

        af.hdlc.rxstart = false;
        ret = AFSK_RXFIFO_OVERRUN;
      }
    }

    // Ajout du caractère reçu dans la pile
    if (!xQueueIsFull(&af.rx_fifo)) {

      vQueuePush(&af.rx_fifo, af.hdlc.currchar);
    }
    else {

      af.hdlc.rxstart = false;
      ret = AFSK_RXFIFO_OVERRUN;
    }

    af.hdlc.currchar = 0;
    af.hdlc.bit_idx = 0;
  }
  else {

    af.hdlc.currchar >>= 1;
  }

  return ret;
}

/* -----------------------------------------------------------------------------
 * This function has to be called by the DAC ISR when a sample of the configured
 * channel has been converted out.
 * @return Le prochain échantillon pour le DAC
 */
static uint8_t
ucNextDacSample (void) {
  AFSK_STROBE_ON();

  /* Vérifies si nous sommes au début d'un cycle d'échantillonnage */
  if (af.sample_count == 0) {

    if (af.tx_bit == 0) {

      /* Nous venons de terminer la transmission d'un char, en prends un nouveau */
      if (xQueueIsEmpty (&af.tx_fifo) && (af.trailer_len == 0)) {

        vAfskTxDisable();
        af.sending = false;
        AFSK_STROBE_OFF();
        return 0;
      }
      else {

        // Si nous venons de terminer l'envoi d'un octet non 'stuffé',
        // réinitialiser le compteur bitstuff.
        if (!af.bit_stuff) {

          af.stuff_cnt = 0;
        }

        af.bit_stuff = true;

        // Gestion des flags de préambule et remorque
        if (af.preamble_len == 0) {

          if (xQueueIsEmpty (&af.tx_fifo)) {

            af.trailer_len--;
            af.curr_out = HDLC_FLAG;
          }
          else {

            af.curr_out = ucQueuePull(&af.tx_fifo);
          }
        }
        else {

          af.preamble_len--;
          af.curr_out = HDLC_FLAG;
        }

        // Gestion des caractères ESC
        if (af.curr_out == AX25_ESC) {

          if (xQueueIsEmpty (&af.tx_fifo)) {

            vAfskTxDisable ();
            af.sending = false;
            AFSK_STROBE_OFF();
            return 0;
          }
          else {

            af.curr_out = ucQueuePull(&af.tx_fifo);
          }
        }
        else {

          if (af.curr_out == HDLC_FLAG || af.curr_out == HDLC_RESET) {

            // Si ces caractères ne sont pas échappés, dévalider le bit stuffing
            af.bit_stuff = false;
          }
        }
      }
      // Démarre avec le masque du bit de poids faible
      af.tx_bit = 0x01;
    }

    // Vérifies si bit stuffing nécessaire
    if (af.bit_stuff && af.stuff_cnt >= BIT_STUFF_LEN) {

      // Si il y a plus de 5 '1' dde suite inséres un 0
      af.stuff_cnt = 0;
      // Changement de tonalité
      af.phase_inc = SWITCH_TONE(af.phase_inc);
    }
    else {

      // NRZI: si l'on veut transmettre un 1, la modulation de fréquence va
      // rester inchangée; avec un 0, il y aura un changement de ton.
      if (af.curr_out & af.tx_bit) {

        // Transmission d'un '1' :
        // - Reste sur la tonalité précédente
        // - Incrémente le compteur de bit stuffing
        af.stuff_cnt++;
      }
      else {

        // Transmission d'un '0' :
        // - Change la tonalité
        // - Clear le compteur de bit stuffing
        af.stuff_cnt = 0;
        af.phase_inc = SWITCH_TONE(af.phase_inc);
      }

      // Passe au bit suivant
      af.tx_bit <<= 1;
    }
    af.sample_count = DAC_SAMPLEPERBIT;
  }

  // Prends un nouvel échantillon pour l'envoyer au DAC
  af.phase_acc += af.phase_inc;
  af.phase_acc %= SIN_LEN;

  af.sample_count--;
  AFSK_STROBE_OFF();
  return ucSinSample (af.phase_acc);
}

/* -----------------------------------------------------------------------------
 * This function has to be called by the ADC ISR when a sample of the configured
 * channel is available.
 * @param curr_sample current sample from the ADC.
 */
static void
vAddAdcSample (int8_t curr_sample) {
  AFSK_STROBE_ON();

  /*
   * Frequency discrimination is achieved by simply multiplying
   * the sample with a delayed sample of (samples per bit) / 2.
   * Then the signal is lowpass filtered with a first order,
   * 600 Hz filter. The filter implementation is selectable
   * through the CONFIG_AFSK_FILTER config variable.
   */

  af.iir_x[0] = af.iir_x[1];

#if (CONFIG_AFSK_FILTER == AFSK_BUTTERWORTH)
  af.iir_x[1] = ((int8_t)ucQueuePull(&af.delay_fifo) * curr_sample) >> 2;
  //af.iir_x[1] = ((int8_t)ucQueuePull(&af.delay_fifo) * curr_sample) / 6.027339492;
#elif (CONFIG_AFSK_FILTER == AFSK_CHEBYSHEV)
  af.iir_x[1] = ((int8_t)ucQueuePull(&af.delay_fifo) * curr_sample) >> 2;
  //af.iir_x[1] = ((int8_t)ucQueuePull(&af.delay_fifo) * curr_sample) / 3.558147322;
#else
#error "Filter type not found !"
#endif

  af.iir_y[0] = af.iir_y[1];

#if CONFIG_AFSK_FILTER == AFSK_BUTTERWORTH
  /*
   * This strange sum + shift is an optimization for af.iir_y[0] * 0.668.
   * iir * 0.668 ~= (iir * 21) / 32 =
   * = (iir * 16) / 32 + (iir * 4) / 32 + iir / 32 =
   * = iir / 2 + iir / 8 + iir / 32 =
   * = iir >> 1 + iir >> 3 + iir >> 5
   */
  af.iir_y[1] = af.iir_x[0] + af.iir_x[1] + (af.iir_y[0] >> 1) + (af.iir_y[0] >> 3) + (af.iir_y[0] >> 5);
  //af.iir_y[1] = af.iir_x[0] + af.iir_x[1] + af.iir_y[0] * 0.6681786379;
#elif CONFIG_AFSK_FILTER == AFSK_CHEBYSHEV
  /*
   * This should be (af.iir_y[0] * 0.438) but
   * (af.iir_y[0] >> 1) is a faster approximation :-)
   */
  af.iir_y[1] = af.iir_x[0] + af.iir_x[1] + (af.iir_y[0] >> 1);
  //af.iir_y[1] = af.iir_x[0] + af.iir_x[1] + af.iir_y[0] * 0.4379097269;
#endif

  /* Save this sampled bit in a delay line */
  af.sampled_bits <<= 1;
  af.sampled_bits |= (af.iir_y[1] > 0) ? 1 : 0;

  /* Store current ADC sample in the af.delay_fifo */
  vQueuePush (&af.delay_fifo, curr_sample);

  /* If there is an edge, adjust phase sampling */
  if (EDGE_FOUND (af.sampled_bits)) {

    if (af.curr_phase < PHASE_THRES) {

      af.curr_phase += PHASE_INC;
    }
    else {

      af.curr_phase -= PHASE_INC;
    }
  }
  af.curr_phase += PHASE_BIT;

  /* sample the bit */
  if (af.curr_phase >= PHASE_MAX) {

    af.curr_phase %= PHASE_MAX;

    /* Shift 1 position in the shift register of the found bits */
    af.found_bits <<= 1;

    /*
     * Determine bit value by reading the last 3 sampled bits.
     * If the number of ones is two or greater, the bit value is a 1,
     * otherwise is a 0.
     * This algorithm presumes that there are 8 samples per bit.
     */
    uint8_t bits = af.sampled_bits & 0x07;
    if (bits == 0x07 // 111, 3 bits set to 1
        || bits == 0x06 // 110, 2 bits
        || bits == 0x05 // 101, 2 bits
        || bits == 0x03) { // 011, 2 bits

      af.found_bits |= 1;
    }

    /*
     * NRZI coding: if 2 consecutive bits have the same value
     * a 1 is received, otherwise it's a 0.
     */
    af.error = iHdlcParse (!EDGE_FOUND(af.found_bits));
  }
  AFSK_STROBE_OFF();
}

// -----------------------------------------------------------------------------
ISR(ADC_vect) {

AFSK_DBG_ON();
  vAfskAdcIrqClear();
  vAddAdcSample(((int16_t)((ADC) >> 2) - 128));

  if (af.sending) {

    vAfskDacWrite (ucNextDacSample());
  }
  else {

    vAfskDacWrite (128); // VOUT/2 -> sin(0)
  }
AFSK_DBG_OFF();
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vAfskInit (int mode) {

  vQueueSetBuffer (&af.tx_fifo, af.tx_buffer, sizeof (af.tx_buffer));
  vQueueSetBuffer (&af.rx_fifo, af.rx_buffer, sizeof (af.rx_buffer));
  vQueueSetBuffer (&af.delay_fifo, af.delay_buffer, sizeof (af.delay_buffer));
  vAfskAdcInit();
  vAfskDacInit();
  AFSK_STROBE_INIT();
  LOG_INFO("MARK_INC %d, SPACE_INC %d\n", MARK_INC, SPACE_INC);

  af.mode = mode;
  af.phase_inc = MARK_INC;
//  af.endof_rx_preamble = false;
  sei();
}

// -----------------------------------------------------------------------------
bool
bAfskSending (void) {

  return af.sending;
}

/* avr-libc stdio interface ================================================= */
FILE xAfskPort = FDEV_SETUP_STREAM (iPutChar, iGetChar, _FDEV_SETUP_RW);

/* ========================================================================== */
