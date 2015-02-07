/**
 * @file lcd_ctrl_twihmi.h
 * @brief Définition pour le contrôleur LCD THMI
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_THMI_CTRL_H_
#define _AVRIO_THMI_CTRL_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* constants ================================================================ */
/* Adresse des registres */
#define THMI_IR  0       /* Registre d'instruction */
#define THMI_DR  1       /* Registre de donnée */

/* Bits de contrôle du registre d'instruction */
#define THMI_CLR   0x01  /* Commande Effacement */
#define THMI_HOME  0x02  /* Commande Retour HOME */

#define THMI_GMEM  0x40  /* Accès CGRAM */
#define THMI_DMEM  0x80  /* Accès DDRAM */

/* Reglage contraste et mise en fonction alimentation */
#define THMI_SETCONTR(__BON,__C43210) \
  ( THMI_CSET | ((__BON) & THMI_BON) | (__C43210) ) 
#define THMI_SETBL(__B543210) \
  ( THMI_BSET | (__B543210) ) 

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_THMI_CTRL_H_ not defined */
