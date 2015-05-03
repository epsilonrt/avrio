/**
 * @file rtc.c
 * @brief Horloge Temps réel implémentation
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120402 - Initial version by epsilonRT
 */
#include <avr/pgmspace.h>
#include "rtc.h"

/* private variables ======================================================== */
static xMutex xRtcHeartBeatsMutex = 0;

/* private variables ======================================================== */
#define RTC_WEEKDAYS_LEN 3
static const char mon[] PROGMEM = "Lun";
static const char tue[] PROGMEM = "Mar";
static const char wed[] PROGMEM = "Mer";
static const char thu[] PROGMEM = "Jeu";
static const char fri[] PROGMEM = "Ven";
static const char sat[] PROGMEM = "Sam";
static const char sun[] PROGMEM = "Dim";

static PGM_P pcRtcWeekDays[7] = {
  sun,
  mon,
  tue,
  wed,
  thu,
  fri,
  sat
};

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
xMutex *
pxRtcHeartBeatsMutex (void) {

  return &xRtcHeartBeatsMutex;
}

// -----------------------------------------------------------------------------
bool 
xRtcYearIsLeap (int usYear) {

  if (!(usYear % 100))
    return !(bool)(usYear % 400);
  else
    return !(bool)(usYear % 4);
}

// -----------------------------------------------------------------------------
const char *
pcRtcWeekdayToStr (uint8_t wd) {
  static char pcStr[RTC_WEEKDAYS_LEN + 1];

  return strcpy_P (pcStr, pcRtcWeekDays[wd - 1]);
}

// -----------------------------------------------------------------------------
int
iRtcDateToStr (char * pcStr, const xRtcTime * pxDate) {

  return sprintf_P (pcStr, PSTR("%s %02d/%02d/%02d"), 
                    pcRtcWeekdayToStr (pxDate->ucWeekDay),
                    pxDate->ucDate, pxDate->ucMonth,
                    pxDate->usYear - (pxDate->usYear / 1000) * 1000);
}

// -----------------------------------------------------------------------------
int
iRtcTimeToStr (char * pcStr, const xRtcTime * pxTime) {

  return sprintf_P (pcStr, PSTR("%02d:%02d:%02d"), 
                    pxTime->ucHour, pxTime->ucMinute, pxTime->ucSecond);
}

// -----------------------------------------------------------------------------
int
iRtcPrintDateToStream (FILE * pxStream, const xRtcTime * pxDate) {

  return fprintf_P (pxStream, PSTR("%s %02d/%02d/%02d"), 
                    pcRtcWeekdayToStr (pxDate->ucWeekDay),
                    pxDate->ucDate, pxDate->ucMonth,
                    pxDate->usYear - (pxDate->usYear / 1000) * 1000);
}

// -----------------------------------------------------------------------------
int
iRtcPrintTimeToStream (FILE * pxStream, const xRtcTime * pxTime) {

  return fprintf_P (pxStream, PSTR("%02d:%02d:%02d"), 
                    pxTime->ucHour, pxTime->ucMinute, pxTime->ucSecond);
}

/* ========================================================================== */
