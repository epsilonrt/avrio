/**
 * @file iface.h
 * @brief Modélisation de l'interface d'un coupleur WirelessUSB CYWUSB6935
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_IFACE_H_
#define _AVRIO_WUSB_IFACE_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
 
/* internal public functions ================================================ */
void vWIfcInit (void);
void vWIfcReset (void);
uint8_t ucWIfcRead (uint8_t ucAddress);
void vWIfcWrite (uint8_t ucAddress, uint8_t ucData);
void vWIfcBurstRead (uint8_t ucAddress, uint8_t * pucData, uint8_t ucLength);
void vWIfcBurstWrite (uint8_t ucAddress, const uint8_t * pucData, uint8_t ucLength);
void vWIfcBurstWrite_P ( uint8_t ucAddress, const uint8_t * pucData, uint8_t ucLength);

# if ! defined(__DOXYGEN__)

#include "avrio-config.h"

#ifdef AVRIO_WUSB_ENABLE
/* ========================================================================== */
# include <avrio/delay.h>
# include "avrio-board-wusb.h"
# include "wusb-config.h"

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void 
  vWIfcSleep (void)) {

    vWusbIoSet (WUSBIO_PD);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void 
  vWIfcWakeup (void)) {
    
    vWusbIoClear (WUSBIO_PD);
    delay_ms (5);
  }

#endif /* AVRIO_WUSB_ENABLE defined */

# endif /* __DOXYGEN__ not defined */
  /* ======================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_WUSB_IFACE_H_ defined */
