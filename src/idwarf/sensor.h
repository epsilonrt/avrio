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
 * @file sensor.h
 * @brief Capteur WirelessUSB iDwaRF
 *
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120309 - Initial version by epsilonRT from iDwaRF V2.5 (20110523)
 */
#ifndef _IDWARF_SENSOR_H_
#define _IDWARF_SENSOR_H_

#include "idwarf.h"
#include "iprotocol.h"

/**
 * @addtogroup idwarf_module
 * @{
 *
 *  @defgroup idwarf_sensor Capteur WirelessUSB iDwaRF
 *
 *  Implémente l'interface logicielle (API) permettant de gérer un capteur 
 *  WirelessUSB.
 *  Cette interface se résume essentiellement à 4 fonctions de gestion que 
 *  l'utilisateur doit implémenté \b lui-même \b : \n
 *  - \ref cbConfigForSleep() appelée juste avant que le capteur passe en mode 
 *    PowerDown
 *  - \ref cbExitFromSleep() appelée après le retour du capteur du mode 
 *    PowerDown.
 *  - \ref cbBackchannelProcess() appelée lorsque des données provenant du hub 
 *    ont été reçues dans le canal de retour.
 *  - \ref cbTxProcess() chargée d'assembler les données à envoyer au hub
 *  .
 *  Une fois l'initialisation effectuée à l'aide de \ref rfInit(), l'utilisateur
 *  doit enregistrer ses fonctions de gestion à l'aide des fonctions 
 *  d'enregistrement ci-dessous puis appeler le plus régulièrement possible
 *  \ref rfProcessAll() dans la boucle infinie du programme principal. \n
 *  @example idwarf/tutorial/sensor/tutorial_sensor.c
 *  Exemple simple qui envoie l'état d'un bouton poussoir à chaque cycle et qui
 *  allume une LED en fonction. Une autre LED permet de visualiser la réponse
 *  du hub.
 *  @example idwarf/terminal/sensor/terminal_sensor.c
 *  Exemple complet qui transmet :
 *  - La température mesurée à l'aide d'un circuit DS1621.
 *  - La luminosité mesurée à l'aide d'une LDR (utilisation ADC).
 *  - La tension batterie (utilisation ADC).
 *  - L'état d'un bouton poussoir.
 *  .
 *  Bascule l'état d'une LED à chaque réception d'informations du hub.
 *  @{
 */

/**
 * @ingroup idwarf_sensor
 * @brief Exemple de fonction utilisateur de gestion du passage en sommeil
 *
 * Appelée juste avant que le capteur passe en mode PowerDown. Cette fonction 
 * permet par exemple de désactiver des capteurs locaux pour réduire la 
 * consommation durant la période PowerDown. \n
 * Cette fonction doit être enregistrée avec 
 * \ref rfRegisterCBUserConfigForSleep().
 * 
 * @param sampleInterval Pointeur sur le nombre de tranches de temps de 125 ms
 * de sommeil que le firmware va effectuer. Cette valeur peut être modifiée 
 * pour réduire ou augmenter le temps de sommeil. Il est possible de désactiver 
 * le sommeil en mettant sampleInterval à zéro.
 */
void cbConfigForSleep (U16 *sampleInterval); 

/**
 * @ingroup idwarf_sensor
 * @brief Exemple de fonction utilisateur de gestion du reveil
 *
 * Appelée après le retour du capteur du mode PowerDown. Cette fonction 
 * permet par exemple de réactiver des capteurs locaux. \n
 * Cette fonction doit être enregistrée avec 
 * \ref rfRegisterCBUserExitFromSleep()
 */
void cbExitFromSleep (void);

/**
 * @ingroup idwarf_sensor
 * @brief Exemple de fonction utilisateur de gestion des données reçues du hub
 *
 * Appelée lorsque des données provenant du hub ont été reçues dans le 
 * canal de retour. Ces données servent par exemple à modifier un signal E/S 
 * dans le capteur ou à engendrer une tension analogique par PWM. \n
 * L'exemple "terminal" allume la LED quand des données sont reçues.
 * Cette fonction doit être enregistrée avec 
 * \ref rfRegisterCBUserBackchannelProcess().
 * 
 * @param userdata Une valeur entre 0 et 31 envoyée par le hub
 * @param buf Pointeur sur le buffer où sont les données reçues du hub
 * @param length Nombre d'octets stockés dans le buffer
 */
void cbBackchannelProcess (U8 userdata, U8 *buf, U8 length);

/**
 * @ingroup idwarf_sensor
 * @brief Exemple de fonction utilisateur de transmission de données
 *
 * Assemble les données à envoyer au hub. Cette fonction permet par 
 * exemple de lire les valeurs de capteurs et de les placer dans le tampon 
 * global d'émission. Les données sont alors envoyées automatiquement au 
 * hub dans le paquet actuel. \n
 * Le nombre maximal d'octets pouvant être transmis est défini par \ref
 * IDWARF_PAYLOAD_SIZE. \n
 * Cette fonction doit être enregistrée avec \ref rfRegisterCBUserTxProcess().
 * 
 * @param pTxData Pointeur sur le buffer où stocker les données à transmettre au hub
 * @return Nombre d'octets stockés dans le buffer devant être transmis. Il est 
 * possible de n'envoyer aucune donnée en renvoyant 0, mais \b cela n'est pas
 * conseillé ! \b .
 */
U8 cbTxProcess (volatile U8 *pTxData);

/**
 * @ingroup idwarf_sensor
 * @brief Type de fonction de gestion de mise en sommeil
 *
 * Permet de simplifier la déclaration de la fonction d'enregistrement.
 */
typedef void (*CBUSERCONFIGFORSLEEP)(U16 *);

/**
 * @ingroup idwarf_sensor
 * @brief Type de fonction de gestion de réveil
 *
 * Permet de simplifier la déclaration de la fonction d'enregistrement.
 */
typedef void (*CBUSEREXITFROMSLEEP)(void);

/**
 * @ingroup idwarf_sensor
 * @brief Type de fonction de gestion de réception
 *
 * Permet de simplifier la déclaration de la fonction d'enregistrement.
 */
typedef void (*CBUSERBACKCHANNELPROCESS)(U8, U8 *, U8);

/**
 * @ingroup idwarf_sensor
 * @brief Type de fonction de gestion de transmission
 *
 * Permet de simplifier la déclaration de la fonction d'enregistrement.
 */
typedef U8   (*CBUSERTXPROCESS)(volatile U8 *);

/**
 * @ingroup idwarf_sensor
 * @brief Enregistre la fonction de gestion de mise en sommeil
 * 
 * @param cbcfs Nom de la fonction implémentée par l'utilisateur
 */
void rfRegisterCBUserConfigForSleep (CBUSERCONFIGFORSLEEP cbcfs);

/**
 * @ingroup idwarf_sensor
 * @brief Enregistre la fonction de gestion de reveil
 * 
 * @param cbefs Nom de la fonction implémentée par l'utilisateur
 */
void rfRegisterCBUserExitFromSleep (CBUSEREXITFROMSLEEP cbefs);

/**
 * @ingroup idwarf_sensor
 * @brief Enregistre la fonction de gestion de réception
 * 
 * @param cbbcp Nom de la fonction implémentée par l'utilisateur
 */
void rfRegisterCBUserBackchannelProcess (CBUSERBACKCHANNELPROCESS cbbcp);

/**
 * @ingroup idwarf_sensor
 * @brief Enregistre la fonction de gestion de transmission
 * 
 * @param cbtxp Nom de la fonction implémentée par l'utilisateur
 */
void rfRegisterCBUserTxProcess (CBUSERTXPROCESS cbtxp);

/**
 * @ingroup idwarf_sensor
 * @brief Endors le circuit CYPRESS WirelessUSB
 * 
 * Cette fonction de la bibliothèque devrait être utilisé conjointement avec 
 * \ref avrSleep() et seulement à l'intérieur de la fonction de mise en
 * sommeil de l'utilisateur ( \ref cbConfigForSleep() ). \n
 * Elle passe la puce RF en powerdown. Elle peut être utilisé pour économiser 
 * l'énergie, mais seulement si l'utilisateur souhaite gérer lui même la mise
 * en sommeil et mets donc sampleInterval à 0 dans \ref cbConfigForSleep(). \n 
 * Si vous utilisez le firmware du sommeil et pas \ref avrSleep() cette fonction 
 * n'est pas nécessaire. 
 */
void rfPowerDown (void);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#endif  /* _IDWARF_SENSOR_H_ */
