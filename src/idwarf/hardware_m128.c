/**
 * @file hardware.c
 * @brief Hardware Driver - ATmega128 version
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120228 - Initial version by epsilonRT
 */
#include "hardware.h"
#include "timer.h"
#include "cypress.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* constants ================================================================ */
/*
  Les déclarations ci-dessous ne peuvent pas être modifiées du fait de la 
  forte imbrication dans le code de chip45.
 */
#define WSS     PB0 // output
#define WSCK    PB1 // output
#define WMOSI   PB2 // output
#define WPD     PB5 // output
#define WRESET  PB6 // output

#define WMISO   PB3 // input
#define WIRQ    PE7 // input

/* public variables ========================================================= */
const FLASH_STRUCTURE  gsFlash = {

  {
    SEEDED_BIND_CHI,
    SEEDED_BIND_PNI,
    { 0x00000000 } ,  // sHubMid
    0x0000,           // u16DevicdId
    0x0000 			      // u16DeviceInterval
  },
  { 0x00000000 },     // sSensorMid
  SEEDED_BIND_CRC,    // u8FlashCrc
  { 0x00 }            // sRadio
};

HW_STATUS gu8HardwareStatus = 0;

/* private functions ======================================================== */

/* Partie spécifique à un mcu */

//--------------------------------------------------------------------------
static void
PIO_Init(void) {

  /* Version ATmega 128 */
  DDRB  |=   _BV(WSS) | _BV(WMOSI) | _BV(WSCK) | _BV(WPD) | _BV(WRESET);
  DDRB  &= ~ _BV(WMISO);
  PORTB &= ~(_BV(WMISO) | _BV(WSS) | _BV(WMOSI) | _BV(WSCK) | _BV(WPD) | _BV(WRESET));
  DDRE  &= ~ _BV(WIRQ);
  PORTE |=   _BV(WIRQ); // pull-up resistor on
}

//--------------------------------------------------------------------------
static void
SPI_MasterInit(void) {

  SPCR = _BV(SPE) | _BV(MSTR);
}

//--------------------------------------------------------------------------
static void
INT_Init(void) {

  EICRB &= ~(_BV(ISC71) | _BV(ISC70)); // low-level irq
  // EIMSK |= _BV(INT7); // enable irq
  EIMSK &= ~_BV(INT7); // disable irq
}


/* internal public functions ================================================ */

//--------------------------------------------------------------------------
//
//  Function:    HwInit
//
//  Description: Start-up mcu devices
//
//--------------------------------------------------------------------------
void 
HwInit(void) {

  PIO_Init();
  SPI_MasterInit();
  SpiRadioOn();
  TimerInit();
  INT_Init();
  RadioInit();
  sei();
}

//--------------------------------------------------------------------------
//
//  Function:    HwValidFlash
//
//  Description: Validates the Flash copy is valid.
//
//  Returns:     true if the Flash Checksum is valid
//
//--------------------------------------------------------------------------
bool HwValidFlash(void) {
  U8 Index;
  U8 CrcResult;
  U8 TempArray[sizeof(FLASH_STRUCTURE)-(sizeof(RADIO_PARAMETERS)+1)];
  const U8 *FlashPointer;
  
  FlashPointer = &gsFlash.sNetConfig.u8ChIndex;
  
  // Copy FLASH structure to RAM (CRC function expects RAM based elements)
  for ( Index = 0; 
        Index < sizeof(FLASH_STRUCTURE)-(sizeof(RADIO_PARAMETERS)+1); 
        ++Index) {
        
    TempArray[Index] = *FlashPointer;
    ++FlashPointer;
  }

  // Validate FLASH_STRUCTURE except for the radio parameters and FlashCrc
  CrcResult = ProtocolComputeCrc( (U8 *)TempArray, 
                sizeof(FLASH_STRUCTURE)-(sizeof(RADIO_PARAMETERS)+1), 0);

  if (CrcResult == gsFlash.u8FlashCrc) {
  
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------
//
//  Function:    HwClearFlash
//
//  Description: Clear all parameters, except for the xtal, and alert the
//               user that this operation has been completed by flashing
//               the red led
//
//--------------------------------------------------------------------------
void 
HwClearFlash(void) {

}

/* ========================================================================== */
