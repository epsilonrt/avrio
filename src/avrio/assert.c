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
 * @file assert.c
 * @brief Assertion
 *
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
