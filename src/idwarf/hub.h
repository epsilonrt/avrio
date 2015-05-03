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
 * @file hub.h
 * @brief Hub WirelessUSB iDwaRF
 *
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120309 - Initial version by epsilonRT from iDwaRF V2.5 (20110523)
 */
#ifndef _IDWARF_HUB_H_
#define _IDWARF_HUB_H_

#include "idwarf.h"
#include "iprotocol.h"
#include "ihost.h"

/**
 * @addtogroup idwarf_module
 * @{
 *
 *  @defgroup idwarf_hub Hub WirelessUSB iDwaRF
 *
 *  Implémente l'interface logicielle permettant de gérer un hub WirelessUSB.
 *  Cette interface se résume essentiellement à 2 fonctions permettant de 
 *  recevoir les informations transmises par les capteurs et de leur
 *  transmettre en réponse des informations (de façon asynchrone donc...) :
 *  - \ref cbPacketReceived() appelée juste avant que le capteur passe en mode 
 *    PowerDown
 *  - \ref cbExitFromSleep() appelée après le retour du capteur du mode 
 *    PowerDown.
 *  - \ref cbBackchannelProcess() appelée lorsque des données provenant du hub 
 *    ont été reçues dans le canal de retour.
 *  - \ref cbTxProcess() chargée d'assembler les données à envoyer au hub
 *  .
 *  Une fois l'initialisation effectuée à l'aide de \ref rfInit(), l'utilisateur
 *  doit enregistrer sa fonction de gestion des paquets reçus à l'aide de 
 *  \ref rfRegisterCBSensorDataReceived() puis appeler \ref rfProcessAll() à
 *  chaque itération de la boucle infinie du programme principal. \n
 *  @example idwarf/tutorial/hub/tutorial_hub.c
 *  Exemple simple qui fait clignoter une LED à chaque réception d'un paquet et
 *  qui renvoit une réponse. Elle récupère l'état d'un bouton poussoir envoyé
 *  par chaque capteur et bascule l'état d'une uatre LED en fonction.
 *  @example idwarf/terminal/hub/terminal_hub.c
 *  Exemple complet qui affiche sur la liaison série les informations reçues
 *  des capteurs qui intégre des commandes permettant de gérer l'intégration
 *  et l'exclusion des capteurs et de leur envoyer des informations.
 *  @{
 */

/* types ==================================================================== */

/**
 * @ingroup idwarf_hub
 * @brief Type de fonction de gestion de réception de paquets
 *
 * Permet de simplifier la déclaration de la fonction d'enregistrement.
 */
typedef void (*CBSENSORPACKETRECEIVED)(PACKET_TYPES PacketType, U16 DeviceId, U8 UserDataCount, volatile U8 *buf);

/* internal public functions ================================================ */

/**
 * @ingroup idwarf_hub
 * @brief Exemple de fonction utilisateur de gestion de réception de paquets
 *
 * Appelée lorsqu'un paquet a été reçu venant d'un capteur. Il est possible 
 * d'accéder directement aux données actuelles du capteur à cet endroit. 
 * Toutefois, tout traitement prolongé \b doit être effectué \b dans le 
 * programme principal, par exemple en copiant les données fournies par cette 
 * fonction dans un buffer partagé ( \ref queue_module ). 
 * Cette fonction doit être enregistrée avec 
 * \ref rfRegisterCBSensorDataReceived().
 * 
 * @param PacketType Type de paquet reçu
 * @param DeviceId Identifiant du capteur à l'origine des informations.
 * @param buf Pointeur sur le buffer où sont les données reçues du hub
 * @param UserDataCount Nombre d'octets stockés dans le buffer
 */
void cbPacketReceived (PACKET_TYPES PacketType, U16 DeviceId, 
                       U8 UserDataCount, volatile U8 *buf);


/**
 * @ingroup idwarf_hub
 * @brief Enregistre la fonction de réception de paquets
 * 
 * @param cbSensor Nom de la fonction implémentée par l'utilisateur
 */
void rfRegisterCBSensorDataReceived (CBSENSORPACKETRECEIVED cbSensor);

/**
 * @ingroup idwarf_hub
 * @brief Effectue une réinitialisation logicielle du firmware et du processeur.
 *  
 * Pas besoin de beaucoup, mais peut être pratique si vous voulez recommencer 
 * à zéro sans aucun capteur ou données stockées pour le traitement.
 */
void   rfReset (void);

/**
 * @ingroup idwarf_hub
 * @brief Renvoie la variable d'état interne du firmware du hub. 
 * 
 * Pas vraiment nécessaire, mais peut être documenté et utilisé davantage dans 
 * l'avenir.
 */
PROTOCOL_STATUS    rfGetProtocolStatus (void);

/**
 * @ingroup idwarf_hub
 * @brief Valide le mode bind
 *
 * Le hub vérifie régulièrement PN0/CH0 à l'écoute de demande d'intégration de
 * capteur.
 * Si un capteur n'a pas la moindre idée des paramètres réseau 
 * (PN Code/Canal) à utiliser pour son intégration, il enverra des demandes 
 * d'intégration sur PN0/CH0 tant que nécessaire. \n
 * Si le hub est configuré pour fonctionner sur PN3 il ne pourra 
 * jamais "entendre" la demande du capteur car il n'écoute pas ce PN. \n
 * Il y a donc un mode où le hub écoute régulièrement PN0/CH0 quand il y a 
 * du temps disponible.
 * Ce "bind" mode est activé et désactivé par les fonctions suivantes.
 * Gardez à l'esprit, qu'un mode "bind" actif peut réduire le débit de données 
 * et constituer un risque potentiel pour la sécurité si vous ne voulez pas 
 * que d'autres capteurs soient intégrés au réseau
 */
void   rfStartBind (void);

/**
 * @ingroup idwarf_hub
 * @brief Invalide le mode bind
 */
void  rfStopBind (void);


/**
 * @ingroup idwarf_hub
 * @brief Renvoie l'identifiant le plus élevé pouvant être géré par le hub
 *
 * Chaque capteur lié obtient une DeviceId et ses données sont stockées 
 * par le hub. Il existe plusieurs configurations possibles pour le stockage de
 * ces informations et cette fonction retourne l'identifiant le plus élevé 
 * possible. Le hub ne pourra donc gérer au plus que rfGetMaximumDeviceId()+1
 * en même temps.
 * @return Capacité du hub en nombre de capteurs 
 */
U16 rfGetMaximumDeviceId (void);


/**
 * @ingroup idwarf_hub
 * @brief Test de la validité et lecture du MID d'un capteur
 *
 * Chaque capteur intégré au réseau est mémorisé par le firmware du hub. 
 * Chaque module RF a son propre identifiant unique, appelé MID. Cette valeur 
 * et le DeviceID correspondant sont mémorisé par le firmware. \n
 * Cette fonction peut être utilisée pour:
 * - Tester pour chaque DeviceId s'il y a un capteur valide correspondant
 * - Obtenir la valeur du MID chaque capteur MID valide
 * @param DeviceId Identifiant du capteur 
 * @param mid Pointeur où stocker le MID résultat (0 = pas de MID renvoyé)
 * @return 1 si l'identifiant correspond à un capteur valide, 0 sinon.
 */
U8 rfGetSensorMID (U16 DeviceId, MID * mid);

/**
 * @ingroup idwarf_hub
 * @brief Suppression d'un capteur
 *
 * Si le hub ne doit plus traiter les messages d'un capteur spécifique ou si 
 * un capteur n'est plus disponible, un appel à cette fonction supprime le 
 * capteur ayant le DeviveId dans la mémoire du hub et définit son statut comme
 * "non lié".
 * @param DeviceId Identifiant du capteur 
 * @return CMD_STATUS_SUCCESS en cas de succès, CMD_STATUS_UNKNOWN_ID si 
 * l'identifiant est inconnu
 */
U8 rfDeleteSensor (U16 DeviceId);


/**
 * @ingroup idwarf_hub
 * @brief Mémorise les données à trasmettre à un capteur par la canal de réponse
 *
 * Chaque fois que le capteur envoie des données au hub, le hub peut répondre 
 * avec des données qu'il envoie vers le capteur. Etant donné que le moment
 * exact où le capteur va envoyer son paquet n'est inconnu, la réponse vers le
 * capteur est stocké en mémoire par le hub.
 * Cette fonction est utilisé pour stocker les données pour le capteur dans la 
 * mémoire du hub. \n
 * Au prochain échange avec le capteur, ces données lui seront transférées.
 * @param DeviceId Identifiant du capteur à qui seront envoyées les données
 * @param userData Une valeur entre 0 et 31 à transmettre au capteur
 * @param beaconTime Cette valeur permet de paramétrer le temps d'endormissement
 * du capteur (exprimée en tranches de temps de 125 ms). Si cette valeur vaut
 * -1, le temps d'endormissement n'est pas transmis et cela permet d'utiliser
 * deux octets supplémentaires pour le payload.le capteur 
 * @param buf Pointeur vers un buffer contenant les données à transmettre
 * @param length Nombre d'octets dans le buffer. Si beaconTime est spécifié
 * ce nombre doit être compris entre 0 et 8, si beaconTime vaut -1, ce nombre
 * peut être compris entre 0 et 10.
 * @return CMD_STATUS_SUCCESS en cas de succès, CMD_STATUS_UNKNOWN_ID si 
 * l'identifiant est inconnu
 */
U8 rfSetBackchannelData (U16 DeviceId, U8 userData, U16 beaconTime, U8 *buf, U8 length);


/**
 * @ingroup idwarf_hub
 * @brief Teste s'il est possible de mémoriser des données pour un capteur
 *
 * Aide à l'utilisation de \ref rfSetBackchannelData()
 * @param DeviceId Identifiant du capteur
 * @return 1 si il y a de la place pour stocker des données pour ce capteur, 0 sinon.
 */
U8 rfIsBackchannelFree (U16 DeviceId);


/**
 * @ingroup idwarf_hub
 * @brief Reconfiguration des paramètres du réseau
 *
 * Définit le PN Code, le canal et son sous-ensemble et permet de changer le 
 * MID du hub (d'où sont tirés le checksum seed et le CRC seed).
 * Tous les capteurs déja liés sont perdus. Avec cette fonction, plus d'un 
 * hub peut être utilisé dans le même espace. Chaque hub est configuré pour 
 * utiliser un PN Code différent donc ils ne gênent pas du tout.
 * 
 * @param pnCode PN Code
 * @param nChannel Canal initial. Ce paramètre définit le sous-ensemble de 
 * canaux utilisés (cf le DVK Technical Reference Manual).
 * @param pMid MID du hub qui sera mémorisé en RAM (celui gravé dans la puce RF 
 * n'est pas modifiable).
 * @return CMD_STATUS_SUCCESS en cas de succès, un code COMMAND_STATUS en cas d'erreur
 */
U8 rfConfigureNetwork (U8 pnCode, U8 nChannel, U32 *pMid);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#endif  /* _IDWARF_HUB_H_ */
