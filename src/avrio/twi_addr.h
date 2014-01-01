/**
 * @file twi_addr.h
 * @brief Adresses des circuits TWI
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_TWI_ADDR_H_
#define _AVRIO_TWI_ADDR_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* PCF2119 - I2C */
#define PCF2119_BASE 0x74   /**< Adresse de base du PCF2119 */
#define PCF2119_SA0  0x02   /**< La broche SA0 permet de décaler l'adresse I2C */

/* ST7032 - I2C */
#define ST7032_BASE 0x7C   /**< Adresse de base du ST7032 */

/* THMI - I2C */
#define THMI_BASE 0x7E   /**< Adresse de base du THMI */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_TWI_ADDR_H_ not defined */
