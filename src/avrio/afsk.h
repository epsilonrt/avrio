/**
 * @file afsk.h
 * @brief Modem AFSK 1200 Bd
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140214 - Initial version from BeRTOS
 */
#ifndef _AVRIO_AFSK_H_
#define _AVRIO_AFSK_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>
#include <avrio/hdlc.h>

/**
 *  @addtogroup radio_group
 *  @{
 *  @defgroup afsk_module Modem AFSK 1200
 *  Ce module réalise un modem AFSK 1200 bauds utilisant HDLC. \n Il sera
 *  gèralement utilisé comme couche physique pour le module \ref ax25_module.
 *  @{
 *    @example aprs/demo_aprs.c
 *    Envoie une trame Ax25/APRS de façon périodique et affiche les trames
 *    reçues sur la liaison série. A chaque réception de trame valide, la led
 *    APRS_LED est basculée.
 */

/* constants ================================================================ */

/**
 * @defgroup afsk_error Codes d'erreur
 * Si la fonction fgetc() renvoie EOF, la variable globale errno définie dans le
 * fichier <erno.h> permet de savoir si une erreur s'est produite. \n
 * @{
 */

/**
 * @brief Buffer réception plein
 *
 * Ce code indique que des octets ont été reçus et qu'ils n'ont pu être
 * mémorisés car le buffer de réception était plein. Cela peut être dû à une
 * taille de buffer insuffisante ou une période d'appel de fgetc() trop longue.
 */
#define AFSK_RXFIFO_OVERRUN -2
/** @} */

/**
 * @defgroup afsk_flag Mode de fonctionnement
 * La fonction vAfskInit() prends en paramètre une variable mode de
 * fonctionnement composé de flags.
 * @{
 */

/**
 * @brief Mode non bloquant
 *
 * Ce flag valide la réception non bloquante
 */
#define AFSK_MODE_NOBLOCK 0x80
/** @} */

/* internal public functions ================================================ */

/**
 * Initialise le module Afsk
 *
 * @param mode Mode de fonctionnement (Voir \ref afsk_flag). \n
 * 0 pour un fonctionnement par défaut (bloquant).
 */
void vAfskInit (int mode);

/**
 * Indique si le module est en cours de transmission
 */
bool bAfskSending (void);

/* public variables ========================================================= */
/**
 * @brief Fichier permettant l'utilisation du modem avec les fonctions de la libc.
 *
 * Pour utiliser le modem comme entrée et sortie pour le module Ax25, on mettra
 * au début du main() :
 * @code
 * int main(void) {
 *   xAx25 ax25;
 *   vAfskInit ();
 *   vAx25Init (&ax25, &xAfskPort, &xAfskPort, vAx25RxCB);
 * ...
 * @endcode
 */
extern FILE xAfskPort;

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * ===========================================================================*/
/**
 * @defgroup afsk_cfg Configuration
 * @{
 * La configuration du module est effectuée dans le fichier avrio-cfg-afsk.h
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 *
 * @defgroup afsk_filter Type de filtre
 * Constantes à utiliser pour la configuration du module dans le fichier
 * avrio-cfg-afsk.h grâce à la macro \c CONFIG_AFSK_FILTER
 * @{
 */

/**
 * @brief Filtre de Butterworth
 */
#define AFSK_BUTTERWORTH  0

/**
 * @brief Filtre de Chebyshev
 */
#define AFSK_CHEBYSHEV    1
/** @} */

/**
 * @brief Filtre
 *
 * Configuration du filtre numérique à utiliser par le module,
 * Voir \ref afsk_filter
 */
#define CONFIG_AFSK_FILTER

/**
 * @brief Taille buffer réception
 *
 * Configuration de la taille du buffer de réception utilisé par le module
 */
#define CONFIG_AFSK_RX_BUFLEN

/**
 * @brief Taille buffer émission
 *
 * Configuration de la taille du buffer d'émission utilisé par le module
 */
#define CONFIG_AFSK_TX_BUFLEN

/**
 * @brief Fréquence échantillonage DAC
 *
 * Confguration de la fréquence d'échantillonage du convertisseur numérique-
 * analogique en Hz.
 */
#define CONFIG_AFSK_DAC_SAMPLERATE

/**
 * @brief Timeout réception
 *
 * Configuration du timeout de réception en ms (-1 pour le désactiver).
 */
#define CONFIG_AFSK_RXTIMEOUT

/**
 * @brief Longueur préambule
 *
 * Configuration de la longueur du préambule en ms avant de débuter la
 * transmission de la trame. Le nombre arrondi de flags HDLC est ajouté avant
 * la trame afin de permettre la synchronisation correcte du récepteur.
 */
#define CONFIG_AFSK_PREAMBLE_LEN

/**
 * @brief Longueur queue de trame
 *
 * Configuration de la longueur de la queue de trame en ms avant de terminer la
 * transmission de la trame. Le nombre arrondi de flags HDLC est ajouté après
 * la trame afin de permettre la synchronisation correcte du récepteur.
 */
#define CONFIG_AFSK_TRAILER_LEN

/**
 * @defgroup afsk_board Driver
 * La configuration de la partie matérielle est effectuée dans le fichier
 * avrio-board-afsk.h \n
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 * Le module utilise les resources suivantes :
 * - ADC
 * - Timer1
 * - DAC connecté sur des broches IO
 * .
 * Les fonctions de cette section doivent être implémentée par l'utilisateur en
 * fonction du matériel utilisé soit sous forme de fonction inline, soit sous
 * forme de fonction normales et dans ce cas, le fichier les contenant doit être
 * ajouté à la compilation.
 * @{
 */

/**
 * @brief Initialisation ADC
 *
 * Cette fonction initialise le convertisseur analogique-numérique et les
 * éléments qu'il utilise.
 */
void vAfskAdcInit (void);

/**
 * @brief Lecture ADC
 *
 * Cette fonction renvoie la valeur de la dernière conversion du convertisseur
 * analogique-numérique et peut si nécessaire acquiter son interruption.
 */
uint16_t usAfskAdcRead (void);

/**
 * @brief Initialisation DAC
 *
 * Cette fonction initialise le convertisseur numérique-analogique et les
 * éléments qu'il utilise.
 */
void vAfskDacInit (void);

/**
 * @brief Lecture ADC
 *
 * Cette fonction écrit dans le convertisseur numérique-analogique. Le DAC est
 * utilisé pour générer une sinusoide qui évolue entre 0 et 255.
 *
 * @param value Valeur à écrire en sortie du convertisseur numérique-analogique.
 */
vAfskDacWrite (uint8_t value);

/**
 * @brief Validation transmetteur
 *
 * La transmission radio est généralement half-duplex. Cette fonction active
 * la partie transmission (PTT). Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
void vAfskTxEnable (void);

/**
 * @brief Dévalidation transmetteur
 *
 * La transmission radio est généralement half-duplex. Cette fonction désactive
 * la partie transmission (PTT). Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
void vAfskTxDisable (void);
/**
 *  @}
 * @}
 */
/* ========================================================================== */
#endif /* __DOXYGEN__ defined */

/**
 *  @}
 * @}
 */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

#endif /* _AVRIO_AFSK_H_ */
