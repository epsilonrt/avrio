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
 * @file twi_addr.h
 * @brief Adresses des circuits TWI
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_TWI_ADDR_H_
#define _AVRIO_TWI_ADDR_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
#if ! defined(__DOXYGEN__)

/* PCF2119 - I2C */
#define PCF2119_BASE 0x74   /**< Adresse de base du PCF2119 */
#define PCF2119_SA0  0x02   /**< La broche SA0 permet de décaler l'adresse I2C */

/* ST7032 - I2C */
#define ST7032_BASE 0x7C   /**< Adresse de base du ST7032 */

/* THMI - I2C */
#define THMI_BASE 0x7E   /**< Adresse de base du THMI */

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_TWI_ADDR_H_ not defined */
