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
 * @file iface.c
 * @brief Implémentation de l'interface d'un coupleur WirelessUSB CYWUSB6935
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_WUSB_ENABLE
/* ========================================================================== */
# include <avr/pgmspace.h>
# include <util/atomic.h>
# include <avrio/spi.h>
# include <avrio/wusb/net/device/iface.h>

/* constants ================================================================ */
#define WUSB_WRITE_FLAG          0x80
#define WUSB_AUTO_INCREMENT_FLAG 0x40
#define WUSB_ADDRESS_MASK        0x3F

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vWIfcInit(void) {

  vWusbIoInit ();
#ifdef WUSBIO_SPI_INIT
  vSpiSetSsAsOutput ();
  vSpiMasterInit (WUSBIO_SPI_INIT);
#endif
  vWIfcReset ();
}

// -----------------------------------------------------------------------------
void 
vWIfcReset (void) {

  vWusbIoIrqDisable ();
  vWusbIoSet (WUSBIO_RESET | WUSBIO_PD);
  delay_ms (2); // t(PWR_RST) t(PWR_PD) = 1300 us min.

  // Chip starts moving. It resets the output
  // Crystal and Oscillator start-up
  vWusbIoClear (WUSBIO_RESET | WUSBIO_PD);
  delay_ms (3);                // t(PDN_X13) = 2000 us typ.
}

// -----------------------------------------------------------------------------
uint8_t 
ucWIfcRead (uint8_t ucAddress) {
  uint8_t ucData;
  
  vWusbIoSet (WUSBIO_SS);

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWrite (ucAddress & WUSB_ADDRESS_MASK);
    ucData = ucSpiMasterRead();
  }
  
  vWusbIoClear (WUSBIO_SS);
  
  return ucData;
} 

// -----------------------------------------------------------------------------
void 
vWIfcWrite (uint8_t ucAddress, uint8_t ucData){
  
  vWusbIoSet (WUSBIO_SS);

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWrite ((ucAddress & WUSB_ADDRESS_MASK) | WUSB_WRITE_FLAG);
    vSpiMasterWrite (ucData);
  }
  
  vWusbIoClear (WUSBIO_SS);
} 

// -----------------------------------------------------------------------------
void 
vWIfcBurstRead (uint8_t ucAddress, uint8_t * pucData, uint8_t ucLength) {

  vWusbIoSet (WUSBIO_SS);

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWrite ((ucAddress & WUSB_ADDRESS_MASK) | WUSB_AUTO_INCREMENT_FLAG);
    while (ucLength--) {
    
      *pucData++ = ucSpiMasterRead ();
    }
  }

  vWusbIoClear (WUSBIO_SS);
}

// -----------------------------------------------------------------------------
void 
vWIfcBurstWrite (uint8_t ucAddress, const uint8_t * pucData, uint8_t ucLength) {

  vWusbIoSet (WUSBIO_SS);

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWrite ((ucAddress & WUSB_ADDRESS_MASK) | 
                               WUSB_AUTO_INCREMENT_FLAG | WUSB_WRITE_FLAG);
    while (ucLength--) {

      vSpiMasterWrite (*pucData++);
    }
  }
  
  vWusbIoClear (WUSBIO_SS);
}

// -----------------------------------------------------------------------------
void 
vWIfcBurstWrite_P(uint8_t ucAddress, const uint8_t * pucData, uint8_t ucLength) {

  vWusbIoSet (WUSBIO_SS);

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWrite ((ucAddress & WUSB_ADDRESS_MASK) | 
                               WUSB_AUTO_INCREMENT_FLAG | WUSB_WRITE_FLAG);
    while (ucLength--) {

      vSpiMasterWrite (pgm_read_byte_near (pucData++));
    }
  }
  
  vWusbIoClear (WUSBIO_SS);
}

/* interrupt service routine ================================================ */
extern void vWDevIsr (void);

ISR(WUSBIO_vect) {

  vWDevIsr ();
}

#endif /* AVRIO_WUSB_ENABLE defined */
/* ========================================================================== */
