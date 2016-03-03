/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file log.h
 * @brief Log (compatibilité sysio)
 */
#ifndef _AVRIO_LOG_H_
#define _AVRIO_LOG_H_
#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup log_module Log
 *  @{
 */

/* constants ================================================================ */
#define LOG_EMERG   0 /**< system is unusable */
#define LOG_ALERT   1 /**< action must be taken immediately */
#define LOG_CRIT    2 /**< critical conditions */
#define LOG_ERR     3 /**< error conditions */
#define LOG_WARNING 4 /**< warning conditions */
#define LOG_NOTICE  5 /**< normal but significant condition */
#define LOG_INFO    6 /**< informational */
#define LOG_DEBUG   7 /**< debug-level messages */

/* structures =============================================================== */
typedef struct xLog xLog;

/**
 * @brief 
 */
struct xLog {

  FILE * stream;
  int mask;
};

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/* macros =================================================================== */
/**
 * @brief
 * @param p
 */
#define LOG_MASK(p)

/**
 * @brief
 * @param p
 */
#define LOG_UPTO(p)

/**
 * @brief
 * @param fmt
 */
#define PDEBUG(fmt,...)

/**
 * @brief
 * @param fmt
 */
#define PINFO(fmt,...)

/**
 * @brief
 * @param fmt
 */
#define PNOTICE(fmt,...)

/**
 * @brief
 * @param fmt
 */
#define PWARNING(fmt,...)

/**
 * @brief
 * @param fmt
 */
#define PERROR(fmt,...)

/* internal public functions ================================================ */

/**
 * @brief 
 * @param priority
 * @param format
 */
void vLog (int priority, const char *format, ...);

/**
 * @brief 
 * @param priority
 * @param format
 */
void vLog_P (int priority, const char *format, ...);

/**
 * @brief 
 * @param mask
 */
static inline void vLogSetMask (int mask);

/**
 * @brief 
 * @return 
 */
static inline int iLogMask (void);

/**
 * @brief 
 * @param f
 */
static inline void vLogSetFile (FILE * f);

/**
 * @brief 
 * @return 
 */
static inline FILE * pxLogFile (void);

/**
 *   @}
 * @}
 */
#  else
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */


/* public variables ========================================================= */
extern xLog xAvrioLog;

/* macros =================================================================== */
#define LOG_MASK(p) (1<<((p)&0x07))
#define LOG_UPTO(p) ((LOG_MASK(p) << 1) - 1)

#ifndef AVRIO_LOG_ENABLE
#include <avr/pgmspace.h>

#define vLog(p,fmt,...) if (xAvrioLog.mask & LOG_MASK(p)) {\
    fprintf(stderr, fmt,##__VA_ARGS__); }

#define vLog_P(p,fmt,...) if (xAvrioLog.mask & LOG_MASK(p)) {\
    fprintf_P (stderr, PSTR (fmt), ##__VA_ARGS__); }

#define PDEBUG(fmt,...) if (xAvrioLog.mask & LOG_MASK(LOG_DEBUG)) {\
    fprintf_P(stderr, PSTR("debug: %s():%d: "fmt"\n"),\
              __FUNCTION__, __LINE__,##__VA_ARGS__); }

#define PINFO(fmt,...) if (xAvrioLog.mask & LOG_MASK(LOG_INFO)) {\
    fprintf_P(stderr, PSTR("info: %s():%d: "fmt"\n"),\
              __FUNCTION__, __LINE__,##__VA_ARGS__); }

#define PNOTICE(fmt,...) if (xAvrioLog.mask & LOG_MASK(LOG_NOTICE)) {\
    fprintf_P(stderr, PSTR("notice: %s():%d: "fmt"\n"),\
              __FUNCTION__, __LINE__,##__VA_ARGS__); }

#define PWARNING(fmt,...) if (xAvrioLog.mask & LOG_MASK(LOG_WARNING)) {\
    fprintf_P(stderr, PSTR("warn: %s():%d: "fmt"\n"),\
              __FUNCTION__, __LINE__,##__VA_ARGS__); }

#define PERROR(fmt,...) if (xAvrioLog.mask & LOG_UPTO(LOG_ERR)) {\
    fprintf_P(stderr, PSTR("error: %s():%d: "fmt"\n"),\
              __FUNCTION__, __LINE__,##__VA_ARGS__); }

#else
#define PDEBUG(fmt,...) vLog_P (LOG_DEBUG,PSTR("%s():%d: "fmt),__FUNCTION__, __LINE__,##__VA_ARGS__)
#define PINFO(fmt,...) vLog_P (LOG_INFO,PSTR("%s():%d: "fmt),__FUNCTION__, __LINE__,##__VA_ARGS__)
#define PNOTICE(fmt,...) vLog_P (LOG_NOTICE,PSTR("%s():%d: "fmt),__FUNCTION__, __LINE__,##__VA_ARGS__)
#define PWARNING(fmt,...) vLog_P (LOG_WARNING,PSTR("%s():%d: "fmt),__FUNCTION__, __LINE__,##__VA_ARGS__)
#define PERROR(fmt,...) vLog_P (LOG_ERR,PSTR("%s():%d: "fmt),__FUNCTION__, __LINE__,##__VA_ARGS__)

void vLog (int priority, const char *format, ...);
void vLog_P (int priority, const char *format, ...);
#endif

// -----------------------------------------------------------------------------
INLINE void
vLogSetMask (int mask) {

  xAvrioLog.mask = mask;
}

// -----------------------------------------------------------------------------
INLINE int
iLogMask (void) {

  return xAvrioLog.mask;
}

// -----------------------------------------------------------------------------
INLINE void
vLogSetFile (FILE * f) {

  xAvrioLog.stream = f;
}

// -----------------------------------------------------------------------------
INLINE FILE *
pxLogFile (void) {

  return xAvrioLog.stream;
}
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_LOG_H_ */
