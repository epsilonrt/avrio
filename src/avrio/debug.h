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
 * @file debug.h
 * @brief Fonctionnalités de débogage
 *
 * Revision History ------------------------------------------------------------
 *    20111217 - Initial version by epsilonRT
 */
#include <avrio/defs.h>
#include <avrio/led.h>
#include <stdio.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup debug_group Débogage
 *  @{
 */

#if defined(__DOXYGEN__)
  /* 
   * __DOXYGEN__ defined
   * Partie documentation ne devant pas être compilée.
   * =========================================================================
   */
#define vDbgPutc(c)
#define vDbgNewLine()
#define vDbgPuts(s)
#define vDbgPuts_P(s)
#define vDbgPutHexByte(b)
#define vDbgPutHexWord(w)
#define vDbgPutHexDword(d)
#define vDbgPutHexBytes(d,l)
#define vDbgPutDec(w)
#define vDbgLedSet(m)
#define vDbgLedClear(m)
#define vDbgLedToggle(m)
#define dprintf(fmt, ...)
#define eprintf(fmt, ...)

  /**
   *   @}
   * @}
   */
#else
  /* 
   * __DOXYGEN__ not defined
   * Partie ne devant pas être documentée.
   * =========================================================================
   */

/* macros =================================================================== */
#ifdef DEBUG_USE_PROGMEM
#include <avr/pgmspace.h>
#define dprintf(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)
#define eprintf(fmt, ...) fprintf_P(stderr, PSTR(fmt), ##__VA_ARGS__)
#else
#define dprintf(fmt,...) printf(fmt, ##__VA_ARGS__)
#define eprintf(fmt,...) fprintf(stderr, fmt, ##__VA_ARGS__)
#endif

#ifndef AVRIO_DEBUG_STREAM
// -----------------------------------------------------------------------------

#define vDbgPutc(c)
#define vDbgNewLine()
#define vDbgPuts(s)
#define vDbgPuts_P(s)
#define vDbgPutHexByte(b)
#define vDbgPutHexWord(w)
#define vDbgPutHexDword(d)
#define vDbgPutHexBytes(d,l)
#define vDbgPutDec(w)

#else
// -----------------------------------------------------------------------------
#include <avrio/term.h>

#define vDbgPutc(c) fputc(c, AVRIO_DEBUG_STREAM)
#define vDbgNewLine() vDbgPutc('\r')
#define vDbgPuts(s) fputs(s, AVRIO_DEBUG_STREAM)
#define vDbgPuts_P(s) fputs_P(s, AVRIO_DEBUG_STREAM)
#define vDbgPutHexByte(b) vTermPutHexByte(b, AVRIO_DEBUG_STREAM)
#define vDbgPutHexWord(w) vTermPutHexWord(w, AVRIO_DEBUG_STREAM)
#define vDbgPutHexDword(d) vTermPutHexDword(d, AVRIO_DEBUG_STREAM)
#define vDbgPutHexBytes(d,l) vTermPutHexBytes(d, l, AVRIO_DEBUG_STREAM)
#define vDbgPutDec(w) vTermPutDec(w, AVRIO_DEBUG_STREAM)

// -----------------------------------------------------------------------------
#endif  /* AVRIO_DEBUG_STREAM defined */

#ifndef AVRIO_DEBUG_LED
// -----------------------------------------------------------------------------

#define vDbgLedSet(m)
#define vDbgLedClear(m)
#define vDbgLedToggle(m)

#else
// -----------------------------------------------------------------------------

#define vDbgLedSet(m) vLedSet(m)
#define vDbgLedClear(m) vLedClear (m)
#define vDbgLedToggle(m) vLedToggle (m)

// -----------------------------------------------------------------------------
#endif  /* AVRIO_DEBUG_LED defined */

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
