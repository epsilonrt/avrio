/**
 * @file twi_noirq.c
 * @brief Module I2C avec le coupleur natif
 * @author Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20111203 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#if defined(AVRIO_TWI_ENABLE) && defined (TWI_MASTER_ENABLE)
/* ========================================================================== */
#include <avrio/twi.h>
#include <avr/io.h>
#include <util/twi.h>
#include <util/atomic.h>
#include "avrio-board-twi.h"

/* constants ================================================================ */
#  define TWI_RD  _BV(0)// bit RW dans l'adresse circuit

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static int8_t 
prviStart (xTwiDeviceAddr xDeviceAddr) {
  uint8_t   ucStatus;

  // send START condition
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

  // wait until transmission completed
  while(!(TWCR & _BV(TWINT)))
    ;

  // check value of TWI Status Register.
  ucStatus = TW_STATUS;
  if ((ucStatus != TW_START) && (ucStatus != TW_REP_START)) 
    return -1;

  // send device address
  TWDR = xDeviceAddr;
  TWCR = _BV(TWINT) | _BV(TWEN);

  // wail until transmission completed and ACK/NACK has been received
  while(!(TWCR & _BV(TWINT)))
    ;

  // check value of TWI Status Register.
  ucStatus = TW_STATUS;
  if ((ucStatus != TW_MT_SLA_ACK) && (ucStatus != TW_MR_SLA_ACK) ) 
    return -1;

  return 0;
}

// -----------------------------------------------------------------------------
static int8_t 
prviReStart (xTwiDeviceAddr xDeviceAddr) {

  return prviStart (xDeviceAddr);
}


// -----------------------------------------------------------------------------
static void 
prvvStop (void) {

  /* send stop condition */
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
  
  // wait until stop condition is executed and bus released
  while (TWCR & _BV(TWSTO))
    ;
}


// -----------------------------------------------------------------------------
static int8_t 
prviWrite (uint8_t ucData) {  
    
  // send data to the previously addressed device
  TWDR = ucData;
  TWCR = _BV(TWINT) | _BV(TWEN);

  // wait until transmission completed
  while(!(TWCR & _BV(TWINT)))
    ;

  // check value of TWI Status Register. Mask prescaler bits
  if (TW_STATUS != TW_MT_DATA_ACK) 
    return -1;
  return 0;
}


// -----------------------------------------------------------------------------
static uint8_t 
  prvucReadAck (void) {

  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
  while(!(TWCR & _BV(TWINT)))
    ;

  return TWDR;
}


// -----------------------------------------------------------------------------
static uint8_t 
  prvucReadNack (void) {

  TWCR = _BV(TWINT) | _BV(TWEN);
  while(!(TWCR & _BV(TWINT)))
    ;
  
  return TWDR;
}

// -----------------------------------------------------------------------------
eTwiStatus
prveGetStatus (void) {
  eTwiStatus eStatus = TWI_SUCCESS;
  uint8_t ucStatus = TW_STATUS; /* Lecture des bits d'états */

  switch (ucStatus) {

    /* ===================================================================== */
    /* Master General                                                        */
    /* ===================================================================== */

    /* --->> 0x48: L'adresse esclave n'a pas été reconnue << --------------- */
    case TW_MR_SLA_NACK:
    /* --->> 0x20: L'adresse esclave n'a pas été reconnue << --------------- */
    case TW_MT_SLA_NACK:
      eStatus = TWI_ERROR_ADDR_NACK;
      break;

    /* ===================================================================== */
    /* Master Transmitter status codes                                       */
    /* ===================================================================== */

    case TW_MT_DATA_NACK:
    /* --->> 0x38: Bus arbitration lost << --------------------------------- */
      eStatus = TWI_ERROR_DATA_NACK;
      break;
    case TW_MT_ARB_LOST:
      eStatus = TWI_STATUS_ARB_LOST;
      break;

    /* ===================================================================== */
    /* Divers                                                                */
    /* ===================================================================== */
    /* --->> 0xF8: No relevant state information << ------------------------ */
    case TW_NO_INFO:
      // Ne fait rien
      break;

    /* --->> 0x00: Bus error due to illegal start or stop condition << ----- */
    case TW_BUS_ERROR:
      eStatus = TWI_ERROR_ILLEGAL_START_STOP;
      break;

    /* --->> Mode non pris en charge << ------------------------------------ */
    default:
      eStatus = TWI_ERROR_ILLEGAL_CODE;
      break;
  }

  return eStatus;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTwiInit (void) {

  vTwiEnablePullup ();
}

// -----------------------------------------------------------------------------
eTwiStatus 
eTwiWrite (xTwiDeviceAddr xDeviceAddr, uint8_t ucByte) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    eStatus = TWI_SUCCESS;
    if (prviStart (xDeviceAddr & ~TWI_RD) == 0) {

      if (prviWrite (ucByte) != 0) {
      
        eStatus = prveGetStatus ();
      }
    }
    else {
    
      eStatus = prveGetStatus ();
    }
    prvvStop (); 
  }
  
  return eStatus;
}

// -----------------------------------------------------------------------------
eTwiStatus 
eTwiRead (xTwiDeviceAddr xDeviceAddr, uint8_t * pucByte) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    
    eStatus = TWI_SUCCESS;
    if (prviStart (xDeviceAddr | TWI_RD) == 0) {

      *pucByte = prvucReadNack ();
    }
    else {
    
      eStatus = prveGetStatus ();
    }
    prvvStop (); 
  }
  
  return eStatus;
}

// -----------------------------------------------------------------------------
eTwiStatus
eTwiSend (xTwiFrame * pxFrame) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xTwiLength xCount = 0;
    uint8_t * pucData = pxFrame->pxData;
    eStatus = TWI_SUCCESS;
    
    if (prviStart (pxFrame->xAddr & ~TWI_RD) == 0) {

      while (xCount < pxFrame->xLen) {
      
        if (prviWrite (*pucData++) != 0) {
        
          eStatus = prveGetStatus ();
          break;
        }
        xCount++;
      }
      if (xCount != 0) {
      
        pxFrame->xLen = xCount;
      }
    }
    else {
    
      eStatus = prveGetStatus ();
    }
    prvvStop ();
  }
  
  return eStatus;
}

// -----------------------------------------------------------------------------
eTwiStatus
eTwiReceive (xTwiFrame * pxFrame) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xTwiLength xCount = 0;
    uint8_t * pucData = pxFrame->pxData;
    eStatus = TWI_SUCCESS;

    if (prviStart (pxFrame->xAddr | TWI_RD) == 0) {

      while ((xCount + 1) < pxFrame->xLen) {
      
        *pucData++ = prvucReadAck();
        xCount++;
      }
      *pucData = prvucReadNack ();
      xCount++;
      if (xCount != 0) {
      
        pxFrame->xLen = xCount;
      }
    }
    else {
    
      eStatus = prveGetStatus ();
    }
    prvvStop (); 
  }
  
  return eStatus;
}

/*=============================================================================
 |                          Mémoires I2C                                      |
 =============================================================================*/
// ----------------------------------------------------------------------------
eTwiStatus 
eTwiMem8Write (uint8_t ucMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xTwiLength xCount = 0;
    uint8_t * pucData = pxFrame->pxData;
    
    eStatus = TWI_SUCCESS;
    if (prviStart (pxFrame->xAddr & ~TWI_RD) != 0) 
      goto eTwiMem8WriteErr;

    if (prviWrite (ucMemAddr) != 0)
      goto eTwiMem8WriteErr;
      
    while (xCount < pxFrame->xLen) {
    
      if (prviWrite (*pucData++) != 0)
        goto eTwiMem8WriteErr;
      xCount++;
    }
    if (xCount != 0) {
    
      pxFrame->xLen = xCount;
    }
    goto eTwiMem8WriteOut;
    
eTwiMem8WriteErr:
    eStatus = prveGetStatus ();
eTwiMem8WriteOut:
    prvvStop (); 
  }
  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiMem8Read (uint8_t ucMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xTwiLength xCount = 0;
    uint8_t * pucData = pxFrame->pxData;
    eStatus = TWI_SUCCESS;

    if (prviStart (pxFrame->xAddr & ~TWI_RD) != 0) 
      goto eTwiMem8ReadErr;
      
    if (prviWrite (ucMemAddr) != 0)
      goto eTwiMem8ReadErr;

    if (prviReStart (pxFrame->xAddr | TWI_RD) != 0)
      goto eTwiMem8ReadErr;

    while ((xCount + 1) < pxFrame->xLen) {
    
      *pucData++ = prvucReadAck();
      xCount++;
    }
    *pucData = prvucReadNack ();
    xCount++;
    if (xCount != 0) {
    
      pxFrame->xLen = xCount;
    }

    goto eTwiMem8ReadOut;
    
eTwiMem8ReadErr:
    eStatus = prveGetStatus ();
eTwiMem8ReadOut:
    prvvStop (); 
  }
  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus 
eTwiMem16Write (uint16_t usMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xTwiLength xCount = 0;
    uint8_t * pucData = pxFrame->pxData;
    
    eStatus = TWI_SUCCESS;
    if (prviStart (pxFrame->xAddr & ~TWI_RD) != 0) 
      goto eTwiMem16WriteErr;
      
    if (prviWrite (usMemAddr >> 8) != 0)
      goto eTwiMem16WriteErr;
      
    if (prviWrite (usMemAddr) != 0)
      goto eTwiMem16WriteErr;
      
    while (xCount < pxFrame->xLen) {
    
      if (prviWrite (*pucData++) != 0)
        goto eTwiMem16WriteErr;
      xCount++;
    }
    if (xCount != 0) {
    
      pxFrame->xLen = xCount;
    }
    goto eTwiMem16WriteOut;
    
eTwiMem16WriteErr:
    eStatus = prveGetStatus ();
eTwiMem16WriteOut:
    prvvStop (); 
  }
  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiMem16Read (uint16_t usMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xTwiLength xCount = 0;
    uint8_t * pucData = pxFrame->pxData;
    
    eStatus = TWI_SUCCESS;
    if (prviStart (pxFrame->xAddr & ~TWI_RD) != 0) 
      goto eTwiMem16ReadErr;
      
    if (prviWrite (usMemAddr >> 8) != 0)
      goto eTwiMem16ReadErr;
      
    if (prviWrite (usMemAddr) != 0)
      goto eTwiMem16ReadErr;

    if (prviReStart (pxFrame->xAddr | TWI_RD) != 0)
      goto eTwiMem16ReadErr;

    while ((xCount + 1) < pxFrame->xLen) {
    
      *pucData++ = prvucReadAck();
      xCount++;
    }
    *pucData = prvucReadNack ();
    xCount++;
    if (xCount != 0) {
    
      pxFrame->xLen = xCount;
    }

    goto eTwiMem16ReadOut;
    
eTwiMem16ReadErr:
    eStatus = prveGetStatus ();
eTwiMem16ReadOut:
    prvvStop (); 
  }
  return eStatus;
}

#endif /* defined(AVRIO_TWI_ENABLE) && defined (TWI_MASTER_ENABLE) */
/* ========================================================================== */
