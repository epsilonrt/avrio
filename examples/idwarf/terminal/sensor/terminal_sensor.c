/**
 * @file template_sensor.c
 * @brief iDwaRF Template - Sensor style
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120227 - initial version from iDwaRF version 2.5 
 *
 * This file is part of the Terminal - program example - sensor version.
 * The device will connect to a present hub and transmit its sensor data
 * via rf. The sampling of the analog voltages as well as the access to
 * the one-wire device is done here. 
 */

#include <avr/interrupt.h>
#include <avrio/ds1621.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <avrio/ds1621.h>

#include <idwarf/sensor.h>
#include "terminal_sensor.h"

/* constants ================================================================ */
#define LED_ALIVE LED_LED1
#define LED_BACKDATA LED_LED2
#define DS1621_SLADDR DS1621_BASE

/* public variables ========================================================= */
volatile bool xButChanged = false;    // only used for the next 2 functions
// store the measured temperature for next txProcess
volatile int16_t iTemp;

/* internal public functions ================================================ */
void vConfigForSleep (uint16_t * pusSampleInterval);
void vExitFromSleep (void);
void vBackchannelProcess (uint8_t ucUserData, uint8_t * pucBuffer, uint8_t ucLength);
uint8_t ucTxProcess (volatile uint8_t * pucTxData);

// -----------------------------------------------------------------------------
// Le programme principal
int 
main (void) {

  // initialize the nodeboard peripherals
  vLedInit ();
  vButInit ();
  
  vTwiInit ();
  eTwiSetSpeed (100);
  vDs1621Init (DS1621_SLADDR, ONESHOT | POL);

  vBatteryMonitorInit();
  vLdrInit();

  // initialise le firmware iDwaRF
  rfInit();

  // Enregistre les fonctions de traitement de l'utilisateur 
  rfRegisterCBUserConfigForSleep (vConfigForSleep);
  rfRegisterCBUserExitFromSleep (vExitFromSleep);
  rfRegisterCBUserBackchannelProcess (vBackchannelProcess);
  rfRegisterCBUserTxProcess (ucTxProcess);

  for (;;) {
  
    // la boucle principale gère les événements du réseau...
    rfProcessAll();
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// vConfigForSleep (callback) 
//  appelée par le système avant qu'il ne s'endorme
//  pusSampleInterval est un pointeur sur une variable contenant le nombre de 
//  tranches de temps de 125 ms pour lequel le système va s'endormir.
//  Cette variable peut être modifiée afin de réduire ou augmenter
//  le temps d'endormissement effectif.
void 
vConfigForSleep (uint16_t * pusSampleInterval) {
  // placez votre code ici: Il sera appellé avant d'endormissement du capteur.

  vDs1621Start(DS1621_SLADDR);

  // max. DS1621 conversion time 750ms = 6 sleep cycles
  if (*pusSampleInterval < 6) 
    *pusSampleInterval = 6;

  // turn off power to the LDR, this saves a lot of power
  vLdrDisable();

  // turn off the led -> indicates sleeping
  vLedClear(LED_ALIVE);;

  // the user should be able to wake up the sensor at any time.
  //  this can be done with the button attached to the nodeboard
  //  the following code enables the input, enables the interrupt
  //  and sleeps in the usercode - not in the system code.
  //  This is done to ensure a very short reaction time
  // configure pin as input
  // enable external interrupt on the button pin
  PCICR   |= _BV(BUTTON_PCIE); // PCIE2: PCINT23..16 pin enable
  BUTTON_PCMSK |= _BV(BUTTON_BUTTON1_PCINT);  // enable interrupt for PCINT19 (PD3)
  
  // set the RF-Chip to sleep - save power
  rfPowerDown();
  
  // sleep here in increments of 125ms. This is done to have a rapid
  //  wakeup when the user presses the button. If we would use the normal
  //  sleep period. The sensor would not send its data packet till after
  //  the whole sleep period.
  for ( ;*pusSampleInterval > 0 ; (*pusSampleInterval)--) {
  
    avrSleep(); // sleep for 125ms
    // no more sleep if button state has changed
    if (xButChanged) 
      break;  // value set in PCINT2_vect interrupt
  }
  xButChanged = 0;    // return to startvalue
  *pusSampleInterval = 0;  // no more sleep in the system code - wake up
}


// -----------------------------------------------------------------------------
// vExitFromSleep (callback) 
//  appelée par le système juste après son réveil.
//
void 
vExitFromSleep (void) {
  // placez votre code de réveil ici.
  
  iTemp = iDs1621GetTemp (DS1621_SLADDR);

  // turn on LED -> indicates wake up
  vLedSet(LED_ALIVE);;

  // turn on power to the LDR
  vLdrEnable();

  // enable the ADC and set the ADMUX to battery voltage input and VCC as AREF
  // this is necessary to let the high impedance battery voltage resistor devider
  // fully charge the ADC input before it's getting actually converted
  vAdcEnable();
}

// -----------------------------------------------------------------------------
// ucTxProcess (callback)
//  chargé de remplir les données dans le paquet à envoyer au hub.
//  renvoie le nombre d'octets stockés dans la paquet (11 au maximum)
//  Format d'un paquet:
//  [ HEADER | DEVICEID(2) | PAYLOAD(11 max.) | CRC(2) | CHECKSUM ]
//  valeur renvoyée | type de paquet | actuellement transmis
//        11        -    data (5)    -  17 octets
//        ...
//         1        -    data (5)    -  7 octets
//         0 - pas conseillé - Transmets aucune donnée
//
uint8_t 
ucTxProcess (volatile uint8_t * pucTxData) {
  uint8_t ucBatt, ucLdr, ucBtn;
  uint8_t * pucFirstByte;
  
  // Placez votre code ici pour écrire les données à envoyer au hub.
  
  // read all analog sensors (i.e. battery monitor and LDR)
  // ADC is already enabled in cbExitFromSleep()
  ucBatt = ucReadAdc(BATT);
  ucLdr  = ucReadAdc(LDR);
  vAdcDisable();

  // read the push button
  ucBtn  = (xButGet(BUTTON_BUTTON1) != 0 ? 1 : 0);
  
  pucFirstByte = (uint8_t *) pucTxData;  // remember original ptr value
  
  // store the values inside the data paket
  *pucTxData++ = ucBtn;
  *pucTxData++ = ucBatt;
  *pucTxData++ = (iTemp >> 8);    // highbyte
  *pucTxData++ = (iTemp & 0xff);  // lowbyte
  *pucTxData++ = ucLdr;
 
  return pucTxData - pucFirstByte;  // return the amount of bytes stored in the buffer
}


// -----------------------------------------------------------------------------
// vBackchannelProcess (callback)
//  appellée quand des données ont été reçues du hub.
//  Les données sont passées en paramètres à cette fonction.
//
void
vBackchannelProcess (uint8_t ucUserData, uint8_t * pucBuffer, uint8_t ucLength) {

  // Données reçues venant du hub: Placez votre code ici.

  if (ucLength > 0) {
    // do something with the backchannel user date
    vLedToggle(LED_BACKDATA);
  }
}

// ------------------------------------------------------------------------------
// Pin Change Interrupt - button was pressed/released
ISR (BUTTON_vect) {

  // disable this interrupt to prevent multiple firing (switch bounce)
  PCICR &= ~_BV(BUTTON_PCIE);
  xButChanged = 1;  // set flag for cbConfigForSleep
}

/* ========================================================================== */
