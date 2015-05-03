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
 * @file template_hub.c
 * @brief iDwaRF Template - Hub style
 *
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120227 - initial version from iDwaRF version 2.5 
 */
#include <idwarf/hub.h>

/* constants ================================================================ */
/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */
/* private functions ======================================================== */
// ------------------------------------------------------------------------------

/* internal public functions ================================================ */
void vSensorPacketReceived (PACKET_TYPES, uint16_t, uint8_t, volatile uint8_t *);

// -----------------------------------------------------------------------------
// Le programme principal
int 
main (void) {

  // initialise le firmware iDwaRF
  rfInit();

  // Enregistre les fonctions de traitement de l'utilisateur 
  rfRegisterCBSensorDataReceived (vSensorPacketReceived);
  
  for (;;) {
    
    // la boucle principale gère les événements du réseau...
    rfProcessAll();
  }
}

// -----------------------------------------------------------------------------
// vSensorPacketReceived (callback)
//  xPacketType     - Type de paquet reçu: BIND_REQUEST or fixed/variable data packets
//  DeviceID        - Identifiant du capteur qui a envoyé le paquet
//  ucUserDataCount - nombre d'octets de payload stockés dans pucBuffer
//  pucBuffer       - Pointeur sur les données transmises. Seuls les ucUserDataCount octets sont valides.
//
void vSensorPacketReceived (PACKET_TYPES xPacketType, uint16_t usDeviceId, 
                uint8_t ucUserDataCount, volatile uint8_t * pucBuffer) {

  // Un paquet a été reçu venant d'un capteur: Placez votre code ici.

}
