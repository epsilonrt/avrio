/**
 * @file tnc.h
 * @brief Terminal Node Controller (Implémentation C)
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version
 */
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#include <avrio/tnc.h>
#include <avrio/crc.h>

#if defined(TNC_LED_RX) || defined(TNC_LED_TX)
#include <avrio/led.h>
#define LED_INIT() vLedInit()
#else
#define LED_INIT()
#endif

#ifdef TNC_LED_RX
#define SET_LED_RX() vLedSet(TNC_LED_RX)
#define CLR_LED_RX() vLedClear(TNC_LED_RX)
#else
#define SET_LED_RX()
#define CLR_LED_RX()
#endif

#ifdef TNC_LED_TX
#define SET_LED_TX() vLedSet(TNC_LED_TX)
#define CLR_LED_TX() vLedClear(TNC_LED_TX)
#else
#define SET_LED_TX()
#define CLR_LED_TX()
#endif

/* private ================================================================== */

// -----------------------------------------------------------------------------
static inline int
prviSetError (xTnc *p, int iError) {

  p->state = iError;
  return iError;
}

// -----------------------------------------------------------------------------
static
uint8_t
htoi (uint8_t c) {

  c = toupper (c);
  if (!isdigit(c))
    c -= 7;
  return c - '0';
}

// -----------------------------------------------------------------------------
static inline void
vClearState (struct xTnc *p) {

  p->state = 0;
  CLR_LED_RX();
  CLR_LED_TX();
}

/* public  ================================================================== */

//##############################################################################
//#                                                                            #
//#                             xTnc Class                                    #
//#                                                                            #
//##############################################################################

// -----------------------------------------------------------------------------
void
vTncInit (struct xTnc *p, FILE *fin, FILE *fout) {

  LED_INIT();
  vTncClear (p);
  p->fin = fin;
  p->fout = fout;
}

// -----------------------------------------------------------------------------
void vTncClear (xTnc *p) {

  memset (p, 0, sizeof (xTnc));
}

// -----------------------------------------------------------------------------
int
iTncError (xTnc *p) {

  return p->state;
}

// -----------------------------------------------------------------------------
int
iTncPoll (xTnc *p) {
  int c;

  if (p->fin == 0) {

    return prviSetError (p, TNC_FILE_NOT_FOUND);
  }

  if (p->state == TNC_EOT)
    p->state = TNC_SUCCESS;

  do {

    c = fgetc (p->fin);

    if (c == EOF) {

      if (ferror(p->fin))
        return prviSetError (p, TNC_IO_ERROR);
    }
    else {

      switch(c) {

        case TNC_SOH:
          p->crc_rx = CRC_CCITT_INIT_VAL;
          p->state = TNC_SOH;
          p->len = 0;
          SET_LED_RX();
          break;

        case TNC_STX:
          if (p->state == TNC_SOH) {

            p->cnt = 0;
            p->state = TNC_STX;
          }
          else
            vClearState(p);
          break;

        case TNC_ETX:
          if (p->state == TNC_STX) {

            p->cnt = 0;
            p->crc_tx = 0;
            p->state = TNC_ETX;
          }
          else
            vClearState(p);
          break;

        case TNC_EOT:
          if (p->state == TNC_ETX) {

            p->state = TNC_EOT;
            CLR_LED_RX();
            if (p->crc_rx != p->crc_tx)
              return prviSetError (p, TNC_CRC_ERROR);
          }
          else
            vClearState(p);
          break;

        default:
          if (isxdigit (c)) {

            // Digit hexa
            switch (p->state) {

              case TNC_STX:
                p->crc_rx = usCrcCcittUpdate (c, p->crc_rx);
                if (p->cnt++ & 1) {
                  // LSB
                  if (p->len < TNC_RXBUFSIZE) {

                    p->rxbuf[p->len++] = p->msb + htoi (c);
                  }
                  else {

                    p->state = TNC_NOT_ENOUGH_MEMORY;
                  }
                }
                else {
                  // MSB
                  p->msb = htoi (c) << 4;
                }
                break;

              case TNC_ETX:
                if (p->cnt <= 12) {

                  p->crc_tx += ((uint16_t) htoi (c)) << (12 - p->cnt);
                  p->cnt += 4;
                }
                else {

                  // Plus de 4 octets de CRC reçu
                  p->state = TNC_ILLEGAL_MSG;
                }
                break;

              default:
                // Digit hexa en dehors d'une trame, on ignore
                vClearState(p);
                break;
            }
          }
          break;

      }
    }
  } while ((c != EOF) && (p->state >= TNC_SUCCESS) && (p->state != TNC_EOT));

  return p->state;
}

// -----------------------------------------------------------------------------
ssize_t
iTncWrite (xTnc *p, const void *buf, size_t count) {
  ssize_t index = 0;

  if (count) {


    if (p->fout < 0) {

      return prviSetError (p, TNC_FILE_NOT_FOUND);
    }
    else {
      int state = TNC_SOH;
      uint8_t cnt = 0;
      char str[5];
      const uint8_t *b = (uint8_t *) buf;
      uint16_t crc = CRC_CCITT_INIT_VAL;

      while (state != TNC_EOT) {
        uint8_t c = 0;

        switch (state) {

          case TNC_SOH:
            c = state;
            state = TNC_STX;
            SET_LED_TX();
            break;

          case TNC_STX:
            c = state;
            state = TNC_TXT;
            break;

          case TNC_TXT:
            if (cnt++ & 1) {

              // LSB
              c = str[1];
              if (++index >= count)
                state = TNC_ETX;
            }
            else {

              // MSB
              snprintf (str, 3, "%02X", b[index]);
              c = str[0];
            }
            crc = usCrcCcittUpdate (c, crc);
            break;

          case TNC_ETX:
            c = state;
            cnt = 0;
            state = TNC_CRC;
            snprintf (str, 5, "%04X", crc);
            break;

          case TNC_CRC:
            if (cnt < 4) {

              c = str[cnt++];
            }
            else {

              c = TNC_EOT;
              state = TNC_EOT;
              CLR_LED_TX();
            }
            break;

          default:
            break;
        }
        if (fputc (c, p->fout) == EOF) {

          return prviSetError (p, TNC_IO_ERROR);
        }
      }
    }
  }
  return index;
}

/* ========================================================================== */
