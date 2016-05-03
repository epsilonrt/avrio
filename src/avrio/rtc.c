/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file rtc.c
 * @brief Horloge Temps réel implémentation
 *
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
