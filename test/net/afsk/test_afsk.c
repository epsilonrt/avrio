/**
 * @file test_fsk.c
 * @brief Test unitaire Modem AFSK 1200
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140215 - Initial version by epsilonRT
 */
#include <avrio/afsk.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define AFSK_MSG "ABDCEF"

static char cTxMsg[] = AFSK_MSG;
static char cRxMsg[64];
enum {
  IDLE,
  PREAMBLE_FLAG,
  PROCESS_FRAME,
  TRAILER_FLAG,
  ERROR
};

/* internal public functions ================================================ */
int
main(void) {
  FILE *f;
  int c;
  int iStatus;
  char *p;

  f = &xAfskPort;
  vLedInit();
  vAfskInit (AFSK_MODE_NOBLOCK);

  for (;;) {

    // Transmits the message
    fprintf(f, "%c%s%c", HDLC_FLAG, cTxMsg, HDLC_FLAG);

    p = cRxMsg;
    iStatus = IDLE;

    while ( ((c = fgetc (f)) != EOF) || (bAfskSending()) ) {

      if (c != EOF) {

        switch (iStatus) {
          case IDLE:
            if (c == HDLC_FLAG)
              iStatus = PREAMBLE_FLAG;
            break;
          case PREAMBLE_FLAG:
            if (c != HDLC_FLAG) {

              *p++ = (char) c;
              iStatus = PROCESS_FRAME;
            }
            break;
          case PROCESS_FRAME:
            if (c != HDLC_FLAG) {

              *p++ = (char) c;
            }
            else
              iStatus = TRAILER_FLAG;
            break;
          default:
            break;
        }
      }
      else
        delay_ms (2);
    }

    if (iStatus == TRAILER_FLAG) {

      if (strncmp (cRxMsg, cTxMsg, strlen(cTxMsg)) == 0) {

        vLedToggle (LED_LED1);
      }
      else {

        for (uint8_t i = 0; i < 8; i++) {

          vLedSet (LED_LED1);
          delay_ms (25);
          vLedClear (LED_LED1);
          delay_ms (75);
        }
      }
    }
    if (errno) {

      clearerr(f);
    }
    delay_ms (5000);
  }
  return 0;
}
