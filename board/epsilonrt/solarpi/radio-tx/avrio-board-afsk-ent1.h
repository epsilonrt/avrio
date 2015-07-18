/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 */
#ifndef _AVRIO_BOARD_AFSK_ENT1_H_
#define _AVRIO_BOARD_AFSK_ENT1_H_
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* =============================================================================
 *
 *                     Configuration du module AFSK
 *
 * ===========================================================================*/
/*
 * La configuration du module est effectuée dans le fichier avrio-cfg-afsk.h
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 */

/*
 * Configuration de la taille du buffer d'émission utilisé par le module
 */
#define CONFIG_AFSK_TX_BUFLEN 128

/*
 * Configuration du nombre de flags HDLC en début de trame
 * Le nombre de flags est ajouté en début de trame afin de synchroniser le
 * récepteur. La valeur minimale est de 1.
 */
#define CONFIG_AFSK_PREAMBLE_LEN 64

/*
 * Configuration du nombre de flags HDLC en fin de trame
 * Le nombre de flags est ajouté en fin de trame afin de synchroniser le
 * récepteur. La valeur minimale est de 1.
 */
#define CONFIG_AFSK_TRAILER_LEN 16

/* =============================================================================
 *
 *                           Driver du module AFSK
 *
 * ===========================================================================*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrio/led.h>

/* private constants ======================================================== */
// Configuration DAC 1 bit
#define AFSK_DAC_LSB    2
#define AFSK_DAC_DDR    DDRD
#define AFSK_DAC_PORT   PORTD
#define AFSK_DAC_MASK   (0x01<<AFSK_DAC_LSB)

// Pré-division timer modulateur (Fout = 9600 Hz)
#define TMODEM_DIV      8

/* public constants ========================================================= */

/*
 * Nombre de bits du DAC
 * A modifier si vous n'utilisez pas le DAC 4 bits GPIO à résistances
 */
#define AFSK_DAC_RESOLUTION 1

/*
 * Vecteur interruption du timer modulateur
 *
 * Cette routine d'interruption est chargée de générer le signal en sortie
 * du modulateur.
 *
 * La modification de ce vecteur nécessite une modification de la fonction
 * vAfskHwTxInit()
 */
#define AFSK_vect    TIMER0_COMPA_vect

/*
 * Les constantes ci-dessous ne devraient pas être modifiée, si vous le faites,
 * il faudra éventuellement modifier le code source du module afsk...
 * ========================================================================== */
// Vitesse de modulation en baud
#define AFSK_BAUDRATE 1200L
// Fréquence de la tonalité MARK
#define AFSK_MARK_FREQ     1200L
// Fréquence de la tonalité SPACE
#define AFSK_SPACE_FREQ    2200L
// Nombre d'échantillons de sinusoide par temps de bit
#define AFSK_SAMPLES_PER_BIT 8L

/* Fin des constantes à modification limitée ================================ */

/* inline public functions ================================================ */

// -----------------------------------------------------------------------------
// External
// @param usValue valeur à écrire entre 0 et 2^R-1 (R = AFSK_DAC_RESOLUTION)
INLINE void
vAfskHwDacWrite (uint16_t usValue) {

  // Mettre votre code ici si nécessaire
  AFSK_DAC_PORT = (AFSK_DAC_PORT & ~AFSK_DAC_MASK) | (usValue << AFSK_DAC_LSB);
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwTxEnable (void) {

  // Mettre votre code ici si nécessaire
  vLedSet (LED_TX);
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwTxDisable (void) {

  // Mettre votre code ici si nécessaire
  vLedClear (LED_TX);
}

/* -----------------------------------------------------------------------------
 * Internal
 * Cette fonction initialise le DAC et les éléments qu'il utilise
 */
INLINE void
vAfskHwDacInit (void) {

  // Mettre votre code ici si nécessaire
  AFSK_DAC_DDR |= AFSK_DAC_MASK;
}

/* -----------------------------------------------------------------------------
 * Internal
 * Cette fonction initialise le transmetteur et les éléments qu'il utilise
 */
INLINE void
vAfskHwTxInit (void) {

  vAfskHwDacInit();

  TCCR0A = _BV(WGM01);  // Mode CTC
  TIMSK0 = _BV(OCIE0A);
  OCR0A = F_CPU/AFSK_BAUDRATE/AFSK_SAMPLES_PER_BIT/TMODEM_DIV-1;
  TCCR0B = _BV(CS01);   // TIMER0_DIV = 8
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwInit (void) {

  vLedInit();
  vAfskHwTxInit();
}

/* -----------------------------------------------------------------------------
 *
 *                 Partie utilisée pour la mise au point (debug)
 *
 * ---------------------------------------------------------------------------*/
// Mettre AFSK_DEBUG à 1 pour valider le debug
#define AFSK_DEBUG        0

#if AFSK_DEBUG
/* ================================= Internal ================================*/
#define DEBUG_PIN_DATA    2
#define DEBUG_PIN_SCK     3

/* -----------------------------------------------------------------------------
 * Désactive la broche d'horloge des bits transmis
 */
static inline void
vAfskDebugSckOff (void) {

  // Mettre votre code ici si nécessaire
  PORTC &= ~_BV(DEBUG_PIN_SCK);
}

/* -----------------------------------------------------------------------------
 * Bascule la broche d'horloge des bits transmis
 */
static inline void
vAfskDebugSckToggle (void) {

  // Mettre votre code ici si nécessaire
  PORTC ^= _BV(DEBUG_PIN_SCK);
}

/* ================================= External ================================*/

/* -----------------------------------------------------------------------------
 * Active la broche chargée d'indiquer la valeur du bit
 */
static inline void
vAfskDebugData (bool bBit) {

  // Mettre votre code ici si nécessaire
  if (bBit)
    PORTC |= _BV(DEBUG_PIN_DATA);
  else
    PORTC &= ~_BV(DEBUG_PIN_DATA);
  NOP();
  NOP();
  NOP();
  NOP();
  vAfskDebugSckToggle();
}

/* -----------------------------------------------------------------------------
 * Initialise la partie matérielle utilisée éventuellement pour
 * la mise au point du module.
 */
static inline void
vAfskDebugInit (void) {

  // Mettre votre code ici si nécessaire
  DDRC |= _BV(DEBUG_PIN_DATA) | _BV(DEBUG_PIN_SCK);
  PORTC &= ~_BV(DEBUG_PIN_DATA);
  vAfskDebugSckOff();
}
#else
/* ================================= External ================================*/
#define vAfskDebugInit()
#define vAfskDebugData(v)
#endif

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_BOARD_AFSK_ENT1_H_ not defined */

