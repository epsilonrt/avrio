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
 * @file itutorial_sensor.c
 * @brief iDwaRF Tutorial - Sensor style
 *
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120227 - initial version from iDwaRF version 2.5 
 * 
 * 
 */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <idwarf/sensor.h>

/* public variables ========================================================= */
// Valeurs échangées entre les différentes routines de l'utilisateur
volatile bool xButChanged = false;  // activation du bouton

/* internal public functions ================================================ */
void vConfigForSleep (uint16_t * pusSampleInterval);
void vExitFromSleep (void);
void vBackchannelProcess (uint8_t ucUserData, uint8_t * pucBuffer, uint8_t ucLength);
uint8_t ucTxProcess (volatile uint8_t * pucTxData);

// -----------------------------------------------------------------------------
// Le programme principal
int 
main (void) {

  vLedInit();
  vButInit();

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
  
  // the user should be able to wake up the sensor at any time.
  //  this can be done with the button attached to the nodeboard
  //  the following code enables the input, enables the interrupt
  //  and sleeps in the usercode - not in the system code.
  //  This is done to ensure a very short reaction time
  // configure pin as input
  // enable external interrupt on the button pin
  PCICR   |= _BV(BUTTON_PCIE); // PCIE pin change interrupt enable
  BUTTON_PCMSK |= _BV(BUTTON_BUTTON1_PCINT);  // enable interrupt for PCINT
  
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
}


// -----------------------------------------------------------------------------
// vBackchannelProcess (callback)
//  appellée quand des données ont été reçues du hub.
//  Les données sont passées en paramètres à cette fonction.
//
void
vBackchannelProcess (uint8_t ucUserData, uint8_t * pucBuffer, uint8_t ucLength) {

  // Données reçues venant du hub: Placez votre code ici.
  vLedClear (LED_LED1);
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
  // Placez votre code ici pour écrire les données à envoyer au hub.
  static uint8_t ucData;
  uint8_t ucBtn;
  uint8_t * pucFirst;

  // read the push button
  ucBtn  = (xButGet(BUTTON_BUTTON1) != 0 ? 1 : 0);
  ucData++;

  pucFirst = (uint8_t *) pucTxData;  // remember original ptr value

  // store the values inside the data paket
  *pucTxData++ = ucBtn;
  *pucTxData++ = ucData;

  vLedSet (LED_LED1);

  return pucTxData - pucFirst;  // calcule la taille du payload
}

// ------------------------------------------------------------------------------
// Pin Change Interrupt - button was pressed/released
ISR (BUTTON_vect) {

  // disable this interrupt to prevent multiple firing (switch bounce)
  PCICR &= ~_BV(BUTTON_PCIE);
  xButChanged = 1;  // set flag for cbConfigForSleep
}

/* ========================================================================== */

