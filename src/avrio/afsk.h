/**
 * @file afsk.h
 * @brief Modem AFSK 1200 Bd HDLC
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
 *  généralement utilisé comme couche physique pour le module \ref ax25_module. \n
 *  A la compilation, il est possible de dévalider la partie transmission en
 *  ajoutant AFSK_TX_DISABLE à la variable AVRIO_CONFIG dans le Makefile. De la
 *  même façon, il est possible de dévalider la réception à l'aide de
 *  AFSK_RX_DISABLE (mais pas les 2 ensemble...). \n
 *  Lorsque la partie réception est validée, le module utilise le convertisseur
 *  analogique-numérique ADC pour la démodulation ainsi que le TIMER1 pour le
 *  cadencement de l'échantillonnage en réception et en émission. \n
 *  Lorsque la partie réception n'est pas validée, le module utilise un timer
 *  8 bits pour le cadencement de l'échantillonnage en émission. \n
 *  En émission, le module utilise un convertisseur numérique-analogique DAC à
 *  4 bits, constitué de 4 résistances ou d'un réseau R-2R relié à des broches
 *  de ports. \n
 *  La partie matérielle peut être modifier dans le fichier avrio-board-afsk.h
 *  décrite dans \ref afsk_board
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

/* internal public functions ================================================ */

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * ===========================================================================*/


/* internal public functions ================================================ */

/**
 * Initialise le module Afsk
 *
 * Après cet appel, les interruptions doivent être validées afin que le module
 * puisse fonctionner. Cela peut se faire l'aide de sei().
 *
 * @param ucMode Mode de fonctionnement (Voir \ref afsk_flag). \n
 * 0 pour un fonctionnement par défaut (bloquant).
 */
void vAfskInit (uint8_t ucMode);

/**
 * Indique si le module est en cours de transmission
 */
bool bAfskSending (void);

/**
 * Indique si le module est en cours de réception
 */
bool bAfskReceiving (void);

/**
 * Indique si le module est synchronisé en réception
 */
bool bAfskDcd (void);

/**
 * @brief Modifie la tonalité en sortie du modulateur
 *
 * Cette fonction est prévue uniquement pour des raisons de mise au point
 * (vérification  de la fréquence des signaux en sortie)
 *
 * @param ucPhaseInc incrément de phase correspondant à la tonalité souhaitée. \n
 * La macro AFSK_PHASE_INC() permet de calculer cette valeur à partir de la
 * fréquence. \n
 * Il est obligatoire d'utiliser les valeurs de fréquences pré-définies
 * AFSK_MARK_FREQ et AFSK_SPACE_FREQ si on souhaite utiliser vAfskToneToggle()
 */
void vAfskToneSet (uint16_t usTxPhaseInc);

/**
 * @brief Bascule la tonalité en sortie du modulateur
 *
 * Cette fonction permet de passer d'une tonalité MARK à une tonalité SPACE et
 * inversement. ELle est prévue uniquement pour des raisons de mise au point.
 */
void vAfskToneToggle (void);


/**
 * @defgroup afsk_cfg Configuration
 * La configuration du module est effectuée dans le fichier avrio-cfg-afsk.h
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 * @{
 */


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
 * @brief Longueur préambule trame
 *
 * Configuration de la longueur du préambule en ms avant de débuter la
 * transmission de la trame. Le nombre arrondi de flags HDLC est ajouté avant
 * la trame afin de permettre la synchronisation correcte du récepteur.
 */
#define CONFIG_AFSK_PREAMBLE_LEN

/**
 * @brief Longueur fin de trame
 *
 * Configuration de la longueur de la queue de trame en ms avant de terminer la
 * transmission de la trame. Le nombre arrondi de flags HDLC est ajouté après
 * la trame afin de permettre la synchronisation correcte du récepteur.
 */
#define CONFIG_AFSK_TRAILER_LEN


/** @} */

/**
 * @defgroup afsk_board Driver
 * La configuration de la partie matérielle est effectuée dans le fichier
 * avrio-board-afsk.h \n
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 * Les fonctions de cette section doivent être implémentées par l'utilisateur en
 * fonction du matériel utilisé soit sous forme de fonction inline, soit sous
 * forme de fonction normales et dans ce cas, le fichier les contenant doit être
 * ajouté à la compilation.
 * @{
 */

/* constants ================================================================ */
/**
 * @brief Vitesse de modulation en baud
 *
 * Cette valeur ne devrait pas être modifiée !
 */
#define AFSK_BAUDRATE 1200L

/**
 * @brief Fréquence de la tonalité MARK
 *
 * Cette valeur ne devrait pas être modifiée !
 */
#define AFSK_MARK_FREQ     1200L

/**
 * @brief Fréquence de la tonalité SPACE
 *
 * Cette valeur ne devrait pas être modifiée !
 */
#define AFSK_SPACE_FREQ    2200L

/**
 * @brief Nombre d'échantillons de sinusoide par temps de bit
 *
 * Il définit la finesse de la sinusoide mais aussi la cadence de l'interruption
 * afin de maintenir une valeur moyenne nulle, ce nombre doit être une
 * une puissance de 2 (2, 4,8 ,16...). \n
 * Si la réception est validée, cette valeur est fixée à 8 sans possibilité
 * de la modifier.
 */
#define AFSK_SAMPLES_PER_BIT 8L

/**
 * @brief Nombre de bits du DAC utilisé pour la modulation
 *
 * A modifier en fonction du DAC utilisé
 */
#define AFSK_DAC_RESOLUTION 4

/**
 * @brief Vecteur interruption du timer modulateur
 *
 * A modifier en fonction de la source d'interruption par le modulateur.
 * Ce vecteur ne sera utilisé que si la réception est dévalidée, dans le cas
 * contraire c'est obligatoirement ADC_vect qui est utilisé par le module.
 */
#define AFSK_TXONLY_vect

/**
 * @brief Initialisation du driver
 *
 * Cette fonction initialise le driver et les éléments qu'il utilise. \n
 * Cette fonction devra être implémentée ou modifiée afin de correspondre au
 * matériel utilisé.
 */
void vAfskHwInit (void);

/**
 * @brief Ecriture DAC
 *
 * Cette fonction écrit dans le convertisseur numérique-analogique. Le DAC est
 * utilisé pour générer une sinusoide en sortie du modulateur. \n
 * Cette fonction devra être implémentée ou modifiée afin de correspondre au
 * matériel utilisé.
 *
 * @param usValue Valeur à écrire en sortie du convertisseur numérique-analogique.
 */
void vAfskHwDacWrite (uint16_t usValue);

/**
 * @brief Validation transmetteur
 *
 * La transmission radio est généralement half-duplex. Cette fonction active
 * la partie transmission (PTT). Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
void vAfskHwTxEnable (void);

/**
 * @brief Dévalidation transmetteur
 *
 * La transmission radio est généralement half-duplex. Cette fonction désactive
 * la partie transmission (PTT). Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
void vAfskHwTxDisable (void);


/**
 * @brief Lecture ADC
 *
 * Cette fonction est utilisée par le module pour lire le convertisseur
 * analogique-numérique pour la visualisation de la détection de porteuse. \n
 * Cette fonction devra être implémentée ou modifiée afin de correspondre au
 * matériel utilisé.
 *
 * @return Valeur de la dernière conversion sous forme d'un entier signé sur
 * 8 bits (-127 à +128)
 */
int8_t iAfskHwAdcRead (void);

/**
 * @brief Détection de présence porteuse
 *
 * Cette fonction est utilisée pour la visualisation de la détection de porteuse.
 * Elle sera appelée dès que le module détecte le début d'une transmission à
 * la bonne fréquence. Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
void vAfskHwDcdOn (void);

/**
 * @brief Détection absence porteuse
 *
 * Cette fonction est utilisée pour la visualisation de l'absence de porteuse.
 * Elle sera appelée dès que le module ne plus la porteuse.
 * Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
void vAfskHwDcdOff (void);

/** @} */

/* ========================================================================== */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include <string.h>
#include <util/atomic.h>
#include <avrio/queue.h>
#include "avrio-board-afsk.h"

#if defined(AFSK_TX_DISABLE) && defined(AFSK_RX_DISABLE)
#error "AFSK_TX_DISABLE and AFSK_RX_DISABLE can not be defined at the same time !"
#endif

#if !defined(AFSK_RX_DISABLE) && (AFSK_SAMPLES_PER_BIT != 8)
#warning "AFSK_RX_DISABLE not defined: AFSK_SAMPLES_PER_BIT was redefined to 8"
#undef AFSK_SAMPLES_PER_BIT
#define AFSK_SAMPLES_PER_BIT 8L
#endif

/* constants ================================================================ */
// Nombre d'échantillon de la sinusoide par le module
#define AFSK_SINWAVE_LEN 512L

// Codage sinus entre 0 et 255
#define AFSK_SINWAVE_RESOLUTION  8

// Macro pour la mise à l'échelle de la valeur envoyée au DAC
#if  AFSK_SINWAVE_RESOLUTION > AFSK_DAC_RESOLUTION
#define AFSK_DAC_VALUE(_v)  ((_v)>>(AFSK_SINWAVE_RESOLUTION-AFSK_DAC_RESOLUTION))
#elif AFSK_SINWAVE_RESOLUTION < AFSK_DAC_RESOLUTION
#define AFSK_DAC_VALUE(_v)  ((_v)<<(AFSK_DAC_RESOLUTION-AFSK_SINWAVE_RESOLUTION))
#else
#define AFSK_DAC_VALUE(_v)  (_v)
#endif

/* macros =================================================================== */
#define AFSK_PHASE_INC(_freq) (((_freq)*AFSK_SINWAVE_LEN)/AFSK_BAUDRATE/AFSK_SAMPLES_PER_BIT)

// Incrément de phase pour la MARK
#define AFSK_MARK_INC   AFSK_PHASE_INC(AFSK_MARK_FREQ)

// Incrément de phase pour la SPACE
#define AFSK_SPACE_INC  AFSK_PHASE_INC(AFSK_SPACE_FREQ)

/* structures =============================================================== */
typedef struct xAfsk {

#ifndef AFSK_TX_DISABLE
  // Transmetteur
  uint16_t usTxPhase;    // Phase de la sinusoide
  uint16_t usTxPhaseInc; // Incrément de phase de la sinusoide
  uint8_t ucTxSampleCnt; // Compteur du nombre d'échantillons par temps de bit
  uint8_t ucTxByte;      // Octet en cours de transmission
  uint8_t ucTxBit;       // Masque sur le bit en cours de transmission
  uint8_t ucTxStuffCnt;  // Compteur du nombre de bit successifs à 1
  uint8_t ucPreambleCnt; // Compteur du nombre de HDLC_FLAG à tramsmettre avant la trame
  uint8_t ucTrailerCnt;  // Compteur du nombre de HDLC_FLAG à tramsmettre après la trame

  xQueue xTxFifo; // Pile des octets à transmettre
  uint8_t ucTxBuffer[CONFIG_AFSK_TX_BUFLEN]; // Buffer de la pile de transmission
#endif

#ifndef AFSK_RX_DISABLE
  // Récepteur
  uint8_t ucRxBitCnt;
  uint8_t ucRxBitStream;
  uint8_t ucRxByte;
  uint8_t ucCarrierCnt;

  xQueue xRxFifo; // Pile des octets reçus
  uint8_t ucRxBuffer[CONFIG_AFSK_TX_BUFLEN]; // Buffer de la pile de réception

  xQueue xSampleFifo; // Pile des échantillons
  int8_t iSampleBuffer[AFSK_SAMPLES_PER_BIT / 2 + 1];
#endif

  // Common
  int iError;
  uint8_t ucMode;
  union {
    volatile uint8_t ucFlag;
    struct {
      volatile uint8_t bTxEnable      :1; // Transmission en cours
      volatile uint8_t bStuffEnable   :1; // Le bit stuffing est activé
      volatile uint8_t bFrameStarted  :1; // Début de trame détectée
      volatile uint8_t bDcd           :1; // La porteuse correcte a été détectée
      volatile uint8_t bTestEnable    :1; // Le mode test est activé
    };
  };

} xAfsk;

/* public variables ========================================================= */
extern xAfsk af; // for inline funcs only !

/* inline public functions ================================================ */

// -----------------------------------------------------------------------------
INLINE void
vAfskInit (uint8_t ucMode) {

  memset (&af, 0, sizeof(af));
  af.ucMode = ucMode;
  vAfskDebugInit();
  vAfskHwInit();

#ifndef AFSK_TX_DISABLE
  vQueueSetBuffer (&af.xTxFifo, af.ucTxBuffer, sizeof(af.ucTxBuffer));
  af.usTxPhaseInc = AFSK_MARK_INC;
  vAfskHwDacWrite (AFSK_DAC_VALUE(128)); // sin(0) -> VREF_DAC/2
#endif

#ifndef AFSK_RX_DISABLE
  vQueueSetBuffer (&af.xRxFifo, af.ucRxBuffer, sizeof(af.ucRxBuffer));
  vQueueSetBuffer (&af.xSampleFifo, (uint8_t *)af.iSampleBuffer, sizeof(af.iSampleBuffer));

  /* Fill sample FIFO with 0 */
  for (int i = 0; i < AFSK_SAMPLES_PER_BIT / 2; i++)
    vQueuePush(&af.xSampleFifo, 0);
#endif
}

#ifndef AFSK_RX_DISABLE
// -----------------------------------------------------------------------------
INLINE bool
bAfskReceiving (void) {

  bool bValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    bValue = (af.bFrameStarted ? true : false);
  }
  return bValue;
}

// -----------------------------------------------------------------------------
INLINE bool
bAfskDcd (void) {
  bool bValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    bValue = (af.bFrameStarted ? true : false);
  }
  return bValue;
}
#endif

#ifndef AFSK_TX_DISABLE
// -----------------------------------------------------------------------------
INLINE bool
bAfskSending (void) {

  bool bValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    bValue = (af.bTxEnable ? true : false);
  }
  return bValue;
}

// -----------------------------------------------------------------------------
INLINE void
vAfskToneSet (uint16_t usTxPhaseInc) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    af.usTxPhaseInc = usTxPhaseInc;
    af.bTestEnable = 1;
    af.bTxEnable = 1;
  }
}

// -----------------------------------------------------------------------------
INLINE void
vAfskToneToggle (void) {

  (af.usTxPhaseInc == AFSK_MARK_INC) ?  vAfskToneSet(AFSK_SPACE_INC) :
                                      vAfskToneSet(AFSK_MARK_INC);
}
#endif // AFSK_TX_DISABLE not defined

/* ========================================================================== */
#endif /* __DOXYGEN__ not defined */
/**
 *  @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_AFSK_H_ not defined */
