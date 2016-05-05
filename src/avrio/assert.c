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
