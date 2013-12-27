/**
 * @file debug.h
 * @brief Fonctionnalités de débogage
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20111217 - Initial version by epsilonRT
 */
#include <avrio/defs.h>
#include <avrio/led.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup util_group
 * @{
 *
 *  @defgroup debug_group Débogage
 *  @{
 */

#  if defined(__DOXYGEN__)
  /* 
   * __DOXYGEN__ defined
   * Partie documentation ne devant pas être compilée.
   * =========================================================================
   */
#   define vDbgPutc(c)
#   define vDbgNewLine()
#   define vDbgPuts(s)
#   define vDbgPuts_P(s)
#   define vDbgPutHexByte(b)
#   define vDbgPutHexWord(w)
#   define vDbgPutHexDword(d)
#   define vDbgPutHexBytes(d,l)
#   define vDbgPutDec(w)
#   define vDbgLedSet(m)
#   define vDbgLedClear(m)
#   define vDbgLedToggle(m)

  /**
   *   @}
   * @}
   */
#  else
  /* 
   * __DOXYGEN__ not defined
   * Partie ne devant pas être documentée.
   * =========================================================================
   */

/* macros =================================================================== */
#   ifndef AVRIO_DEBUG_STREAM
// -----------------------------------------------------------------------------

#    define vDbgPutc(c)
#    define vDbgNewLine()
#    define vDbgPuts(s)
#    define vDbgPuts_P(s)
#    define vDbgPutHexByte(b)
#    define vDbgPutHexWord(w)
#    define vDbgPutHexDword(d)
#    define vDbgPutHexBytes(d,l)
#    define vDbgPutDec(w)

#   else
// -----------------------------------------------------------------------------
#    include <avrio/term.h>
#    include <stdio.h>

#    define vDbgPutc(c) fputc(c, AVRIO_DEBUG_STREAM)
#    define vDbgNewLine() vDbgPutc('\r')
#    define vDbgPuts(s) fputs(s, AVRIO_DEBUG_STREAM)
#    define vDbgPuts_P(s) fputs_P(s, AVRIO_DEBUG_STREAM)
#    define vDbgPutHexByte(b) vTermPutHexByte(b, AVRIO_DEBUG_STREAM)
#    define vDbgPutHexWord(w) vTermPutHexWord(w, AVRIO_DEBUG_STREAM)
#    define vDbgPutHexDword(d) vTermPutHexDword(d, AVRIO_DEBUG_STREAM)
#    define vDbgPutHexBytes(d,l) vTermPutHexBytes(d, l, AVRIO_DEBUG_STREAM)
#    define vDbgPutDec(w) vTermPutDec(w, AVRIO_DEBUG_STREAM)

// -----------------------------------------------------------------------------
#   endif  /* AVRIO_DEBUG_STREAM defined */

#   ifndef AVRIO_DEBUG_LED
// -----------------------------------------------------------------------------

#    define vDbgLedSet(m)
#    define vDbgLedClear(m)
#    define vDbgLedToggle(m)

#else
// -----------------------------------------------------------------------------

#    define vDbgLedSet(m) vLedSet(m)
#    define vDbgLedClear(m) vLedClear (m)
#    define vDbgLedToggle(m) vLedToggle (m)

// -----------------------------------------------------------------------------
#   endif  /* AVRIO_DEBUG_LED defined */

#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
