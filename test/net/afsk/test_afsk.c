/**
 * @file test_afsk.c
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

#define AFSK_MSG "ABDCEF"

static char cTxMsg[] = AFSK_MSG;
static char cRxMsg[32];
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
  FILE *af;
  int c;
  int iStatus;
  char *p;

  af = &xAfskPort;
  vLedInit();
  vAfskInit (AFSK_MODE_NOBLOCK);

  for (;;) {

    // Transmits the message
    fprintf(af, "%c%s%c", HDLC_FLAG, cTxMsg, HDLC_FLAG);
    vLedToggle (LED_LED2);
    while ((c = fgetc (af)) == EOF)
      ;

    p = cRxMsg;
    iStatus = IDLE;
    do  {

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
    } while ((c = fgetc (af)) != EOF);

    if (iStatus == TRAILER_FLAG) {

      if (strncmp (cRxMsg, cTxMsg, strlen(cTxMsg)) == 0) {

        vLedToggle (LED_LED1);
      }
    }
    delay_ms (1000);
  }
  return 0;
}
