/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file iface.h
 * @brief Modélisation de l'interface d'un coupleur WirelessUSB CYWUSB6935
 *
 * Revision History ------------------------------------------------------------
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
