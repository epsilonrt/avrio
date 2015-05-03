/**
 * @file assert.c
 * @brief Assertion
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
 */
#include "avrio-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <avrio/assert.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
__assert (const char *__func, int __lineno, const char *__sexp) {
  
  fprintf (stderr, "Err %s:%d %s\n", __func, __lineno, __sexp);
  abort();
}
/* ========================================================================== */
