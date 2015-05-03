/**
 * @file itypes.h
 * @brief Types utilisés dans le firmware iDwaRF-Net
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120310 - Initial version by epsilonRT

 *	code excluded from cydefine.h. 
 *	based on the original Cypress firmware.
 *	- ported by Christian Meinhardt for the iDwaRF firmware. www.chip45.com
 *			 cmeinhardt@cwmsys.de
 *
 * Copyright 2003-2004, Cypress Semiconductor Corporation.
 *
 * This software is owned by Cypress Semiconductor Corporation (Cypress)
 * and is protected by and subject to worldwide patent protection (United
 * States and foreign), United States copyright laws and international
 * treaty provisions. Cypress hereby grants to licensee a personal,
 * non-exclusive, non-transferable license to copy, use, modify, create
 * derivative works of, and compile the Cypress Source Code and derivative
 * works for the sole purpose of creating custom software in support of
 * licensee product to be used only in conjunction with a Cypress integrated
 * circuit as specified in the applicable agreement. Any reproduction,
 * modification, translation, compilation, or representation of this
 * software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * Cypress reserves the right to make changes without further notice to the
 * materials described herein. Cypress does not assume any liability arising
 * out of the application or use of any product or circuit described herein.
 * Cypress does not authorize its products for use as critical components in
 * life-support systems where a malfunction or failure may reasonably be
 * expected to result in significant injury to the user. The inclusion of
 * Cypress' product in a life-support systems application implies that the
 * manufacturer assumes all risk of such use and in doing so indemnifies
 * Cypress against all charges.
 *
 * Use may be limited by and subject to the applicable Cypress software
 * license agreement.
 */
#ifndef _ITYPES_H_
#define _ITYPES_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @addtogroup idwarf_module
 * @{
 */
/* types ==================================================================== */

/**
 * @brief Entier de 8 bits non signé (octet)
 */
typedef uint8_t   U8;

/**
 * @brief Entier de 16 bits non signé
 */
typedef uint16_t  U16;

/**
 * @brief Entier de 32 bits signé
 */
typedef uint32_t  U32;

/**
 * @brief Entier de 8 bits signé (octet)
 */
typedef int8_t    I8;

/**
 * @brief Entier de 16 bits signé
 */
typedef int16_t   I16;

/**
 * @brief Entier de 32 bits signé
 */
typedef int32_t   I32;

/**
 * @}
 */

/* constants ================================================================ */
#if !defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#ifndef NULL
# define NULL            0
#endif

#ifndef F_CPU 
# define F_CPU 7372800UL			// iDwaRF
#endif

#ifndef fOSC 
# define fOSC F_CPU
#endif

#ifndef BAUDRATE
# define BAUDRATE  115200	// 115,2 kbps
#endif

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
#endif /* _ITYPES_H_ */
