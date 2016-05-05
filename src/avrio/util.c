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
 * @file util.c
 * @brief Module utilitaire - Implémentation C
 *
 * Revision History ------------------------------------------------------------
 *    20120317 - Initial version by epsilonRT
 */

/* ========================================================================== */
#include <avrio/util.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint8_t 
ucUtilReverseBits (uint8_t ucByte) {
    uint8_t ucCount = sizeof(ucByte) * 8 - 1;
    uint8_t ucResult = ucByte;
 
    ucByte >>= 1;
    while(ucByte) {
    
       ucResult <<= 1;
       ucResult |= ucByte & 1;
       ucByte >>= 1;
       ucCount--;
    }
    ucResult <<= ucCount;
    return ucResult;
}
/* ========================================================================== */
