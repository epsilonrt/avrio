/**
 * @file thmi.h
 * @brief tiny HMI
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
 */
#ifndef _AVRIO_THMI_H_
#define _AVRIO_THMI_H_

#include <avrio/defs.h>
#include <avrio/keyb.h>
#include <avrio/lcd.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup thmi_module tiny HMI
 *  Ce module permet de gérer une interface homme-machine avec une carte tiny
 *  HMI.
 *  @{
 */
  
/* internal public functions ================================================ */

/**
 * @brief Lecture de la version du firmware
 * @return Version du firmware 0xMMNN, MM version majeure, NN mineure
 */
uint16_t usThmiVersion (void);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_THMI_H_ not defined */
