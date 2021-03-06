/*
 * iDwaRF Template - Sensor style
 *
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.
 */

#include <idwarf/sensor.h>

/* constants ================================================================ */
/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */
/* private functions ======================================================== */
// ------------------------------------------------------------------------------

/* internal public functions ================================================ */
void vConfigForSleep (uint16_t * pusSampleInterval);
void vExitFromSleep (void);
void vBackchannelProcess (uint8_t ucUserData, uint8_t * pucBuffer, uint8_t ucLength);
uint8_t ucTxProcess (volatile uint8_t * pucTxData);

// -----------------------------------------------------------------------------
// Le programme principal
int 
main (void) {

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
  return 0; // à modifier
}

/* ========================================================================== */
