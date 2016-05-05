/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
