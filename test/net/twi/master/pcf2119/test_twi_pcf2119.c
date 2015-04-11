/**
 * @file test_twi_pcf2119.c
 * @ingroup test_group
 * @brief Tests unitaires du module \ref twi_module "Bus I2C".
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/twi.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <string.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

/* constants ================================================================ */
#define LED_ALIVE LED_LED1

#define POR PD6
#define POR_DDR   DDRD
#define POR_PORT  PORTD

// ------------------------------------------------------------------------------
// Caractéristiques du circuit esclave
#define PCF2119_BASE 0x74
#define PCF2119_SA0  0x02
#define BTHQ21605_ADDR  (PCF2119_BASE + PCF2119_SA0)

// Bits
#define PCF2119_IR 0
#define PCF2119_DR 1
#define PCF2119_RS 6
#define PCF2119_CO 7

// Normal commands
#define PCF2119_CLR                      (1<<0)
#define PCF2119_HOME                     (1<<1)
#define PCF2119_ENTM(__id,__s)          ((1<<2) | ((__id)<<1) | ((__s)<<0))
#define PCF2119_DCTL(__d,__c,__b)       ((1<<3) | ((__d)<<2)  | ((__c)<<1) | ((__b)<<0))
#define PCF2119_SHFT(__sc,__rl)         ((1<<4) | ((__sc)<<3) | ((__rl)<<2))
#define PCF2119_FSET(__dl,__m,__sl,__h) ((1<<5) | ((__dl)<<4) | ((__m)<<2) | ((__sl)<<1) | ((__h)<<0))

// Extended commands
#define PCF2119_SETVA(__va) ((1<<7) | (__va))
#define PCF2119_SETVB(__vb) ((1<<7) | (1<<6) | (__vb))

// IĠc commands
#define PCF2119_CTRL(__co, __rs) (((__co)<<PCF2119_CO) | ((__rs)<<PCF2119_RS))
#define PCF2119_IFRAME PCF2119_CTRL(0,PCF2119_IR)
#define PCF2119_DFRAME PCF2119_CTRL(0,PCF2119_DR)

/* private variables ======================================================== */
static xTwiFrame xFrame;

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}
// ------------------------------------------------------------------------------
static void
vPcf2119Reset (void) {

  POR_PORT |= _BV(POR);
  delay_us (20);
  POR_PORT &= ~_BV(POR);
  delay_ms (2);
}

// ------------------------------------------------------------------------------
// Le BATRONBTHQ21605V n'est pas ASCII !!! le caractère 0x20 n'est pas l'espace !
// L'espace a pour code 0xA0, soit 0x20 + 0x80 ...
static inline void
vPcf2119Clear (void) {
  eTwiStatus eError;
  uint8_t pucBuffer[11] = { PCF2119_IFRAME, PCF2119_HOME };
  uint8_t ucCount;

  xFrame.pxData = pucBuffer;
  
  // Return home
  xFrame.xLen = 2;
  eError = eTwiSend (&xFrame);
  vAssert (eError == TWI_SUCCESS);
 
  pucBuffer[0] = PCF2119_DFRAME;
  for (ucCount = 1; ucCount < 11; ucCount++)
    pucBuffer[ucCount] = ' ' + 0x80;
  xFrame.xLen = sizeof(pucBuffer);
  
  for (ucCount = 0; ucCount < 8; ucCount++) {
  
    eError = eTwiSend (&xFrame);
    vAssert (eError == TWI_SUCCESS);
  }
}

// ------------------------------------------------------------------------------
static void
vPcf2119Init (void) {
  eTwiStatus eError;
  uint8_t pucInit[] = {
    PCF2119_IFRAME,
    PCF2119_FSET(0,0,0,1), // Function set:   H=1
    PCF2119_SETVA(40), // Set VA: 0 à 63 - Plage correcte 32 à 48 - 40
    PCF2119_SETVB(0), // Set VB: 0 à 63
    PCF2119_FSET(0,1,0,0), // Function set:  8 bits - 2 lignes - H=0
    PCF2119_DCTL(1,1,0), // Display On/Off:  Display ON - Cursor ON - Blink OFF
    PCF2119_ENTM(1,0) // Entry mode set:  Increment - Shift Cursor
  };
 
  POR_DDR  |= _BV(POR);
  vPcf2119Reset();

  xFrame.xAddr = BTHQ21605_ADDR;
  xFrame.xLen = sizeof(pucInit);
  xFrame.pxData = pucInit;
  eError = eTwiSend (&xFrame);
  vAssert (eError == TWI_SUCCESS);
  vPcf2119Clear();
}

// ------------------------------------------------------------------------------
static void
vPcf2119Write (uint8_t ucDr, uint8_t ucByte) {
  eTwiStatus eError;
  uint8_t pucBuffer[2];
  
  pucBuffer[0] = PCF2119_CTRL(0, ucDr);
  pucBuffer[1] = ucByte;
  xFrame.xLen = sizeof(pucBuffer);
  xFrame.pxData = pucBuffer;
  eError = eTwiSend (&xFrame);
  vAssert (eError == TWI_SUCCESS);
}

// ------------------------------------------------------------------------------
static inline void
vPcf2119Putchar (char c) {

  // les codes "ASCII" sont décalés de 0x80 dans le BATRONBTHQ21605V ...
  vPcf2119Write (PCF2119_DR, c | 0x80);
}

/* main ===================================================================== */
int
main (void) {
  eTwiStatus eError;
  char c;
  
  vTwiInit ();
  vLedInit ();
  vLedSet (LED_ALIVE);

  eError = eTwiSetSpeed (100);
  vAssert (eError== TWI_SUCCESS);
  
  vPcf2119Init();

  for (;;) {
    
    
    vLedSet (LED_ALIVE);
    for (c = 'A'; c < 'A' + 16; c++)
      vPcf2119Putchar(c);
    delay_ms(1000);

    vLedClear (LED_ALIVE);
    vPcf2119Clear();
    delay_ms(1000);
  }
  return 0;
}

/** @} */
/* ========================================================================== */
