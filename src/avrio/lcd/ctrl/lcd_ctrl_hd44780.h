/**
 * @file lcd_ctrl_hd44780.h
 * @brief Définition pour le contrôleur LCD HD44780
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_HD44780_CTRL_H_
#define _AVRIO_HD44780_CTRL_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* constants ================================================================ */
/* Adresse des registres */
#define HD44780_IR  0       /* Registre d'instruction */
#define HD44780_DR  1       /* Registre de donnée */

/* Bits de contrôle du registre d'instruction */
#define HD44780_CLR   0x01  /* Commande Effacement */
#define HD44780_HOME  0x02  /* Commande Retour HOME */

#define HD44780_GMEM  0x40  /* Accès CGRAM */
#define HD44780_DMEM  0x80  /* Accès DDRAM */

/* Bits d'état du registre d'instruction */
#define HD44780_BF 0x80     /* BUSY Flag */

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_HD44780_CTRL_H_ not defined */
