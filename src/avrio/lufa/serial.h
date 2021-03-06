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
 * @file 
 * @brief CDC Virtual Serial
 *
 *
 * Revision History ------------------------------------------------------------
 *    20121226 - Initial version by epsilonRT
 */
#ifndef _MSL_SERIAL_H_
#define _MSL_SERIAL_H_
#include <avrio/defs.h>
#include <stdio.h>

/* public variables ========================================================= */
extern FILE xUsbSerial;

/* internal public functions ================================================ */
void vSerialInit (void);
uint16_t usSerialHit (void);
void vSerialUsbTask (void);
#endif  /* _MSL_SERIAL_H_ not defined */
/* ========================================================================== */
