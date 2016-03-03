/**
 * Copyright © 2016 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file log.c
 * @brief Gestion des logs
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avrio/log.h>

/* constants ================================================================ */
#ifdef AVRIO_LOGLEVEL
#define AVRIO_LOGMASK LOG_UPTO(AVRIO_LOGLEVEL)
#else
#define AVRIO_LOGMASK LOG_UPTO(LOG_ERR)
#endif


/* public variables ========================================================= */
xLog xAvrioLog = {

  .stream = NULL,
  .mask = AVRIO_LOGMASK
};

#ifdef AVRIO_LOG_ENABLE

static const char error[] PROGMEM = "error: ";
static const char warning[] PROGMEM = "warning: ";
static const char notice[] PROGMEM = "notice: ";
static const char info[] PROGMEM = "info: ";
static const char debug[] PROGMEM = "debug: ";
static const char unknown[] PROGMEM = "unknown: ";

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static const char *
get_priority_name (int pri) {
  switch (pri) {
    case LOG_EMERG:
    case LOG_ALERT:
    case LOG_CRIT:
    case LOG_ERR:
      return error;
    case LOG_WARNING:
      return warning;
    case LOG_NOTICE:
      return notice;
    case LOG_INFO:
      return info;
    case LOG_DEBUG:
      return debug;
    default:
      break;
  }
  return unknown;
}
// -----------------------------------------------------------------------------

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vLog (int priority, const char *format, ...) {
  va_list va;

  va_start (va, format);
  if (xAvrioLog.mask & LOG_MASK (priority)) {
    FILE * f = (xAvrioLog.stream) ? xAvrioLog.stream : stderr;

    (void) fprintf_P (f, get_priority_name (priority));
    (void) vfprintf (f, format, va);
    (void) fputc ('\n', f);
    fflush (f);
  }
  va_end (va);
}

// -----------------------------------------------------------------------------
void
vLog_P (int priority, const char *format, ...) {

  if (xAvrioLog.mask & LOG_MASK (priority)) {
    va_list va;
    FILE * f = (xAvrioLog.stream) ? xAvrioLog.stream : stderr;

    va_start (va, format);
    (void) fprintf_P (f, get_priority_name (priority));
    (void) vfprintf_P (f, format, va);
    (void) fputc ('\n', f);
    va_end (va);
  }
}

#endif

#ifdef LOG_ASSERT
/* private functions ======================================================== */

void __vLogAssertFail (__const char *__assertion, __const char *__file,
                       unsigned int __line, __const char *__function) {

  if (xAvrioLog.bIsInit) {

    vLog (LOG_CRIT, "%s:%d: %s: Assertion '%s' failed.",
          __file, __line, __function, __assertion);
  }
  else {

    (void) fprintf (stderr, "%s(%s): %s:%d: %s: Assertion '%s' failed.\n",
                    __progname, get_priority_name (LOG_CRIT),
                    __file, __line, __function, __assertion);
  }
  abort();
}

void __vLogAssertPerrorFail (int __errnum, __const char *__file,
                             unsigned int __line, __const char *__function) {


  if (xAvrioLog.bIsInit) {

    vLog (LOG_CRIT, "%s:%d: %s: %s.",
          __file, __line, __function, strerror (__errnum));
  }
  else {

    (void) fprintf (stderr, "%s(%s): %s:%d: %s: %s.\n",
                    __progname, get_priority_name (LOG_CRIT),
                    __file, __line, __function, strerror (__errnum));
  }
  abort();
}
#endif

/* ========================================================================== */
