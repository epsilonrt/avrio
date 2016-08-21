/**
 * Copyright © 2016 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file
 * @brief Modules Blyss™ - Implémentation
 */
#include "avrio-config.h"

/* ========================================================================== */
#include "avrio-board-blyss.h"
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avrio/blyss.h>
#include <avrio/delay.h>

/* macros =================================================================== */
#if defined(BLYSS_IN_BIT) && ! defined(BLYSS_RX_DISABLE)
#define __BLYSS_RX_ENABLE__
#endif
#if defined(BLYSS_OUT_BIT) && ! defined(BLYSS_TX_DISABLE)
#define __BLYSS_TX_ENABLE__
#endif

/* constants ================================================================ */
#define BLYSS_FLAG 0xFE
#define BLYSS_NOF_BITS 52

/* timing setup ============================================================= */
#define HEADER_TIME 2400  // us
#define T1          400   // us
#define T2          800   // us
#define FOOTER_TIME 24    // ms

/* private constants ======================================================== */
static const char error[] = "Error";
static const char ok[] = "Ok";
static const uint8_t channels[] = { 0, 8, 4, 2, 1, 3, 5, 6, 7 };
static const uint8_t barrel[] = { 0x98, 0xDA, 0x1E, 0xE6, 0x67 };

#ifdef __BLYSS_TX_ENABLE__
// -----------------------------------------------------------------------------
/* private variables ======================================================== */
static  uint8_t ucBlyssLastToken = 0x7D;
static  uint8_t ucBlyssRollingIndex;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
INLINE  void
vSetOut (void) {

  BLYSS_OUT_PORT |= _BV (BLYSS_OUT_BIT);
}

// -----------------------------------------------------------------------------
INLINE  void
vClearOut (void) {

  BLYSS_OUT_PORT &= ~_BV (BLYSS_OUT_BIT);
}

// -----------------------------------------------------------------------------
INLINE  void
vSendBit (bool bit) {

  vClearOut ();
  if (bit) {

    delay_us (T1);
  }
  else {

    delay_us (T2);
  }
  vSetOut ();
  if (bit) {

    delay_us (T2);
  }
  else {

    delay_us (T1);
  }
}

// -----------------------------------------------------------------------------
INLINE  void
vSendHeader (void) {

  vSetOut ();
  delay_us (HEADER_TIME);
}

// -----------------------------------------------------------------------------
INLINE  void
vSendFooter (void) {

  vClearOut ();
  delay_ms (FOOTER_TIME);
}

// -----------------------------------------------------------------------------
INLINE  void
vSendFrame (xBlyssFrame * f) {
  uint8_t byte;
  uint8_t mask = 0;

  vSendHeader();
  for (uint8_t i = 0; i < BLYSS_NOF_BITS; i++) {

    if (mask == 0) {

      mask = 0x80;
      byte = f->raw[i / 8];
    }

    vSendBit (byte & mask);
    mask >>= 1;
  }
  vSendFooter ();
}

// -----------------------------------------------------------------------------
INLINE  void
vSetFlag (xBlyssFrame * f) {

  vBlyssFrameSetBits (f, BLYSS_IDX_FLAG, 8, BLYSS_FLAG);
}

// -----------------------------------------------------------------------------
INLINE  void
vSetToken (xBlyssFrame * f) {

  vBlyssFrameSetBits (f, BLYSS_IDX_TOKEN, 8, ucBlyssLastToken);
  ucBlyssLastToken += 10;
}

// -----------------------------------------------------------------------------
INLINE  void
vSetRollingCode (xBlyssFrame * f) {

  vBlyssFrameSetBits (f, BLYSS_IDX_ROLL, 8, barrel[ucBlyssRollingIndex]);
  if (++ucBlyssRollingIndex >= sizeof (barrel)) {

    ucBlyssRollingIndex = 0;
  }
}

// -----------------------------------------------------------------------------
#endif /* __BLYSS_TX_ENABLE__ defined */

#ifdef __BLYSS_RX_ENABLE__ 
// -----------------------------------------------------------------------------
#include <avrio/led.h>

enum {
  eStatusIdle,
  eStatusHeader,
  eStatusT1,
  eStatusT2,
  eStatusFrameAvailable
};

/* private variables ======================================================== */
static volatile uint8_t ucRxStatus;
static xBlyssFrame xRxFrame;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
ISR (BLYSS_IN_TIMER_vect) {
#if 0
  TCNT3 = 0;
#else
  static uint8_t ucBitCounter;
  static uint16_t usT1;

  uint16_t usTime = usBlyssTimerRead();
  uint8_t ucNextEdge = eEdgeRising;

  switch (ucRxStatus) {

    case eStatusIdle:
      ucRxStatus = eStatusHeader;
      ucNextEdge = eEdgeFalling;
      break;

    case eStatusHeader:
      if ( (usTime > 2200) && (usTime < 2800)) {

        ucBitCounter = 0;
        ucRxStatus = eStatusT1;
        vLedSet (LED_LED1);
      }
      else {

        ucRxStatus = eStatusIdle;
      }
      break;

    case eStatusT1:
      if ( (usTime > 1200) || (usTime < 100)) {

        ucRxStatus = eStatusIdle;
        vLedClear (LED_LED1);
      }
      else {

        usT1 = usTime;
        ucNextEdge = eEdgeFalling;
        ucRxStatus = eStatusT2;
      }
      break;

    case eStatusT2:
      if ( (usTime > 1200) || (usTime < 100)) {

        ucRxStatus = eStatusIdle;
      }
      else {
        uint8_t ucMask = _BV (7 - ucBitCounter % 8);

        if ( (usT1 > 600) && (usTime < 600)) {

          xRxFrame.raw[ucBitCounter / 8] &= ~ucMask;
        }
        else if ( (usT1 < 600) && (usTime > 600)) {

          xRxFrame.raw[ucBitCounter / 8] |= ucMask;
        }
        else {

          ucRxStatus = eStatusIdle;
          vLedClear (LED_LED1);
          break;
        }

        if (++ucBitCounter == BLYSS_NOF_BITS) {

          ucRxStatus = eStatusFrameAvailable;
          ucNextEdge = eEdgeNone;
          vLedClear (LED_LED1);
        }
        else {

          ucRxStatus = eStatusT1;
        }
      }
      break;
  }
  vBlyssTimerSetEdge (ucNextEdge);
#endif
}

// -----------------------------------------------------------------------------
#endif /* __BLYSS_RX_ENABLE__ defined */

// -----------------------------------------------------------------------------
INLINE void
vBlyssFrameSetGlobalChannel (xBlyssFrame * f, uint8_t global_channel) {

  vBlyssFrameSetBits (f, BLYSS_IDX_GCHAN, 4, global_channel);
}

// -----------------------------------------------------------------------------
INLINE void
vBlyssFrameSetAddress (xBlyssFrame * f, uint16_t addr) {

  vBlyssFrameSetBits (f, BLYSS_IDX_ADDR, 16, addr);
}

// -----------------------------------------------------------------------------
INLINE  uint8_t
ucFlag (const xBlyssFrame * f) {

  return vBlyssFrameGetBits (f, BLYSS_IDX_FLAG, 8);
}

// -----------------------------------------------------------------------------
INLINE  uint8_t
ucToken (const xBlyssFrame * f) {

  return vBlyssFrameGetBits (f, BLYSS_IDX_TOKEN, 8);
}

// -----------------------------------------------------------------------------
static  uint8_t
ucRollingCode (const xBlyssFrame * f) {

  return vBlyssFrameGetBits (f, BLYSS_IDX_ROLL, 8);
}

// -----------------------------------------------------------------------------
static bool
bIsValidRollingCode (uint8_t code) {

  for (uint8_t i = 0; i < sizeof (barrel); i++) {

    if (barrel[i] == code) {

      return true;
    }
  }
  return false;
}

// -----------------------------------------------------------------------------
static uint8_t
ucDecodeChannel (uint8_t field) {
  uint8_t c;

  for (c = 0; c < sizeof (channels); c++) {

    if (field == channels[c]) {

      return c;
    }
  }
  return -1;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vBlyssInit (void) {

#ifdef __BLYSS_TX_ENABLE__
  vClearOut();
  BLYSS_OUT_DDR |= _BV (BLYSS_OUT_BIT);
#endif /* __BLYSS_TX_ENABLE__ defined */

#ifdef __BLYSS_RX_ENABLE__
  vBlyssTimerInit();
  vBlyssTimerSetEdge (eEdgeRising);
  vLedInit();
#endif /* __BLYSS_RX_ENABLE__ defined */
}

// -----------------------------------------------------------------------------
void
vBlyssSend (xBlyssFrame * f, uint8_t repeat) {

#ifdef __BLYSS_TX_ENABLE__
  vSetRollingCode (f);
  vSetToken (f);

  while (repeat--) {

    vSendFrame (f);
  }
#endif /* __BLYSS_TX_ENABLE__ defined */
}

// -----------------------------------------------------------------------------
bool
bBlyssReceive (xBlyssFrame * f) {

#ifdef __BLYSS_RX_ENABLE__
  if (ucRxStatus == eStatusFrameAvailable) {

    memcpy (f, (const void *) &xRxFrame, sizeof (xBlyssFrame));
    ucRxStatus = eStatusIdle;
    vBlyssTimerSetEdge (eEdgeRising);
  }
#endif /* __BLYSS_RX_ENABLE__ defined */
  return false;
}

// -----------------------------------------------------------------------------
void
vBlyssFrameInit (xBlyssFrame * f, const uint8_t * id) {

  memset (f, 0, sizeof (xBlyssFrame));
#ifdef __BLYSS_TX_ENABLE__
  vSetFlag (f);
  vBlyssFrameSetGlobalChannel (f, id[0]);
  vBlyssFrameSetAddress (f, (id[1] << 8) |  id[2]);
#endif /* __BLYSS_TX_ENABLE__ defined */
}

// -----------------------------------------------------------------------------
void
vBlyssFrameSetChannel (xBlyssFrame * f, uint8_t channel) {

  if (channel < (sizeof (channels) - 1)) {

    vBlyssFrameSetBits (f, BLYSS_IDX_CHAN, 4, channels[channel]);
  }
}

// -----------------------------------------------------------------------------
uint8_t
ucBlyssFrameChannel (const xBlyssFrame * f) {

  return ucDecodeChannel (vBlyssFrameGetBits (f, BLYSS_IDX_CHAN, 4));
}

// -----------------------------------------------------------------------------
bool
bBlyssIsValidFrame (const void * buffer) {
  const xBlyssFrame * f = buffer;

  if (f->raw[0] == BLYSS_FLAG) {

    return bIsValidRollingCode (ucRollingCode (f));
  }
  return false;
}

// -----------------------------------------------------------------------------
bool
bBlyssIsValidChannel (uint8_t channel) {

  return ucDecodeChannel (channel) != (uint8_t) - 1;
}

// -----------------------------------------------------------------------------
void
vBlyssPrintFrameToFile (const xBlyssFrame * f, FILE * out) {
  uint16_t i;

  fprintf_P (out, PSTR ("RF frame          : "));
  for (i = 0; i < BLYSS_NOF_BITS; i += 4) {

    fprintf_P (out, PSTR ("%X"), vBlyssFrameGetBits (f, i, 4));
  }

  i = ucFlag (f);
  fprintf_P (out, PSTR ("\nRF footprint      : %02X - %s\n"), i, i == BLYSS_FLAG ? ok : error);
  fprintf_P (out, PSTR ("RF global channel : %X\n"), ucBlyssFrameGlobalChannel (f));
  fprintf_P (out, PSTR ("RF adress         : %04X\n"), usBlyssFrameAddress (f));

  fprintf_P (out, PSTR ("RF channel        : %u\n"), ucBlyssFrameChannel (f));
  fprintf_P (out, PSTR ("Light status      : %s\n"), bBlyssFrameState (f) ? "On" : "Off");

  i = ucRollingCode (f);
  fprintf_P (out, PSTR ("Rolling code      : %02X - %s\n"), i, bIsValidRollingCode (i) ? ok : error);
  fprintf_P (out, PSTR ("Token             : %02X\n"), ucToken (f));
}


// -----------------------------------------------------------------------------
void
vBlyssFrameSetBits (xBlyssFrame * f, uint8_t index, uint8_t len, uint16_t value) {
  uint8_t i = index / 8;
  uint8_t mask, ls;

  for (uint8_t c = 0; c < len; c += 4, index += 4, i++) {

    if ( (index % 8) == 0) {

      // index multiple de 8 (MSB)
      if ( (len - c) >= 8) {

        // copie un octet
        f->raw[i] = value >> c;
        c += 4;
        index += 4;
        continue;
      }

      // quartet MSB
      mask = 0x0F;
      ls = 4;
    }
    else {

      // quartet LSB
      mask = 0xF0;
      ls = 0;
    }

    // copie quartet
    f->raw[i] = (f->raw[i] & mask) | ( (value >> ( (len - c) - 4)) << ls);
  }
}

// -----------------------------------------------------------------------------
uint16_t
vBlyssFrameGetBits (const xBlyssFrame * f, uint8_t index, uint8_t len) {
  uint16_t value = 0;
  uint8_t i = index / 8;
  uint8_t nibble, rest;

  for (uint8_t c = 0; c < len; c += 4, index += 4, i++) {

    rest = (len - c);

    if ( (index != 4) && ( (index % 8) == 0)) {


      // index multiple de 8 (MSB)
      if (rest >= 8) {

        // copie un octet
        value |=  f->raw[i] << (rest - 8);
        c += 4;
        index += 4;
        continue;
      }

      // quartet MSB
      nibble = f->raw[i] >> 4;
    }
    else {

      // quartet LSB
      nibble = f->raw[i] & 0x0F;
    }

    // copie quartet
    value |= nibble << (rest - 4);
  }

  return value;
}
/* ========================================================================== */
