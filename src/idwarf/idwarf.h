/**
 * @file idwarf.h
 * @brief iDwaRF Definitions and Declarations
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120304 - Initial version by epsilonRT
 */
#ifndef _IDWARF_H_
#define _IDWARF_H_

#include "itypes.h"
#include "version.h"

/**
 * @defgroup idwarf_module Firmware iDwaRF-Net
 * Le logiciel iDwaRF-Net permet de porter le protocole WirelessUSB de Cypress
 * sur le microcontrôleur AVR ATmega168. Le module iDwaRF-168, programmable
 * à volonté, assume avec une égale facilité le rôle de concentrateur ou de 
 * capteur dans un réseau radio N:1 de capteurs. Il est facile d'inclure des 
 * fonctions propres à l'utilisateur. \n
 * WirelessUSB fonctionne dans la bande ISM 2,4 GHz. Chaque réseau radio 
 * WirelessUSB dispose d'un choix de 79 canaux. \n
 * Le protocole trouve toujours un canal libre, même avec un grand nombre
 * d'appareils WirelessUSB utilisés simultanément.
 * La transmission repose sur un processus DSSS (Direct Sequence Spread
 * Spectrum) robuste, de sorte que la réception des données est encore assurée
 * avec un taux d'erreur de 10%. Le protocole change automatiquement de
 * canal en cas de perturbations récurrentes. \n
 * Semblable en cela à Bluetooth, WirelessUSB existe en version pour zone
 * locale (LS) jusqu'à 10 m et en version à longue portée (LR) atteignant 50 m.\n
 * Le module iDwaRF-168 est basé sur cette dernière version. \n
 * \image html wirelessUSB.png "Le réseau radio en étoile se compose d'un concentrateur et de plusieurs capteurs."
 * \image latex wirelessUSB.eps "Le réseau radio en étoile se compose d'un concentrateur et de plusieurs capteurs." width=7cm
 * Un module iDwaRF programmé comme concentrateur (hub) forme le centre d'un 
 * réseau radio N:1 en étoile qui peut comporter un grand nombre de capteurs.
 * Le concentrateur fonctionne en général sans interruption et est raccordé à un 
 * PC ou à un autre micro-contrôleur hôte par liaison sérielle. \n
 * Dans le cas d'applications simples, le module iDwaRF peut exécuter lui même
 * des fonctions hôte spécifiques à l'utilisateur.
 * Une unité capteur se compose d'un module iDwaRF programmé en conséquence
 * et du capteur proprement dit. \n
 * L'oscillateur interne RC de l'AVR peut servir à diminuer la consommation. En
 * outre, le module n'est actif que par intermittence (beacon time).
 * Les transferts sont toujours déclenchés par le capteur et confi rmés par le
 * concentrateur. Celui-ci peut aussi faire parvenir des données au capteur par 
 * le canal de retour (back channel). \n
 * Ce module reprends l'implémentation du firmware original en version 2.5 
 * disponible sur le site de <a href="http://shop.chip45.com/epages/es10644620.sf/en_US/?ObjectPath=/Shops/es10644620/Categories/AVR-Funk-Module-Boards/AVR-Funk-Module-Boards-2-4GHz-ISM/AVR-Funk-Module-Boards-2-4GHz-ISM-iDwaRF-Net">chip45.com</a>
 * (lui même dérivé de l'implémentation du DVK de CYPRESS) en lui ajoutant la 
 * possibilité de modifier certaines ressources utilisées
 * comme le timer de temporisation ou la mémoire utilisé par le hub pour 
 * stocker les informations sur les capteurs et donc d'utiliser
 * iDwaRF-Net sur d'autres cartes cibles que le module iDwaRF de chip45.com.\n
 * @{
 */

/* constants ================================================================ */

/**
 * @brief Nombre d'octets nécessaires pour stocker les informations d'un capteur
 *
 * <b> Ne pas être modifié <\b> car l'implémentation du DVK CYPRESS utilise 
 * le décalage de 4 rangs vers la droite ou la gauche au lieu de faire des 
 * divisions ou des multiplications par cette valeur.
 */
#define IDWARF_BACKDATA_SIZE  16

/**
 * @brief Nombre maximum d'octets pouvant être transmis dans une trame WirelessUSB
 *
 * 7 octets sont nécessaires pour la gestion du capteur :
 * - 4 octets pour le MID
 * - 1 octet pour la longueur du payload
 * - 1 octet de 8 bits d'état
 * - 1 octet de 3 bits pour les flags de séquence et le single bit
 * .
 */
#define IDWARF_PAYLOAD_SIZE  (IDWARF_BACKDATA_SIZE - 7)

/* macros =================================================================== */
/**
 * @brief Convertir un temps en seconde en nombre de tranches de temps
 * @param  __sec Nombre de secondes à convertir (peut être un nombre réel)
 * @return le nombre de tranche de temps (valeur tronquée)
 */
#define IDWARF_SLEEP_TIME(__sec)  (U16)((__sec) / 125E-3)

/* internal public functions ================================================ */

/**
 * @brief Initialise toutes les ressources utilisées par le firmware iDwaRF
 *
 * Dans la version ATmega168/328 (iDwaRF Node), les ressources utilisées
 * sont les suivantes :
 * - \b TIMER0 : \n 
 *  Utilisé pour les temporisations. Programé sous interruption de débordement 
 *  avec prédivision par 64. Un préchargement de TCNT0 à 0x8D avec un quartz de 
 *  7372800 Hz donne une période de déclenchement de l'interruption de 1 ms.
 * - \b SPI : \n
 *   Utilisé en mode master FCLK = F_CPU/4, sans interruption.
 * - \b WATCHDOG : \n
 *   Utilisé en interruption pour le réveil du mode Power-Down (125 ms)
 * .
 * <CENTER><TABLE>
 * <CAPTION>Broches utilisées</CAPTION>
 * <TR> <TH>Broche</TH> <TH>Fonction</TH> <TH>Sens</TH></TR>
 * <TR> <TD>INT0</TD>   <TD>IRQ</TD>      <TD>In</TD></TR>
 * <TR> <TD>PC0</TD>    <TD>/PD</TD>      <TD>Out</TD></TR>
 * <TR> <TD>PC1</TD>    <TD>/RESET</TD>   <TD>Out</TD></TR>
 * <TR> <TD>PB2</TD>    <TD>/SS</TD>      <TD>Out</TD></TR>
 * <TR> <TD>PB3</TD>    <TD>MOSI</TD>     <TD>Out</TD></TR>
 * <TR> <TD>PB4</TD>    <TD>MISO</TD>     <TD>In</TD></TR>
 * <TR> <TD>PB5</TD>    <TD>SCK</TD>      <TD>Out</TD></TR>
 * </TABLE></CENTER>
 * @warning Cette fonction appelle sei() afin de valider les interruptions.
 */
void rfInit (void);

/**
 * @brief Effectue toutes les tâches du firmware
 *
 * Cette fonction effectue toutes les tâches nécessaires au fonctionnement du 
 * firmware. \n
 * Pour le hub : 
 * - choix du canal utilisé par le réseau et changement en cas de brouillage
 * - processus d'intégration ou d'exclusion des capteurs (BIND)
 * - réception et envoi des informations aux capteurs
 * .
 * Pour le capteur :
 * - Intégration du réseau au démarrage et en cas de changement de canal.
 * - réception et envoi des informations au hub
 * - Endormissement et reveil.
 * .
 * Cette fonction \b doit \b être appellée à chaque itération de la boucle 
 * infinie du programme principal.
 */
void rfProcessAll (void);

/**
 * @brief Endormir le microcontrôleur pendant une tranche de temps de 125 ms.
 * Cette fonction peut être appelée afin de placer le microcontrôleur en veille
 * (mode Power-down) pendant une tranche de temps de 125 ms.\n
 * Le réveil est provoqué par une interruption du chien de grade.
 */
void avrSleep (void);

/**
 * @}
 */

/* ========================================================================== */
#endif  /* _IDWARF_H_ defined */
