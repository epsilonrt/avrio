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
 * @file spi.h
 * @brief SPI Driver
 *
 * Revision History ------------------------------------------------------------
 *    20120227 - Initial version by epsilonRT
 */
#ifndef _IDWARF_SPI_H_
#define _IDWARF_SPI_H_

#include "itypes.h"

/* internal public functions ================================================ */
U8 SPI_MasterReceive(void);
void SPI_MasterTransmit(U8);

/* ========================================================================== */
#endif  /* _IDWARF_SPI_H_ not defined */
