/*
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef _AVRIO_BOARD_AFSK_KIWI_H_
#define _AVRIO_BOARD_AFSK_KIWI_H_
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
 * Configuration de la taille du buffer de réception utilisé par le module
 */
#define CONFIG_AFSK_RX_BUFLEN 64

/* =============================================================================
 *
 *                           Driver du module AFSK
 *
 * ===========================================================================*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrio/led.h>

/* private constants ======================================================== */
// Voie ADC utilisée pour l'entrée de démodulation
#define AFSK_ADC_SIGNAL     3

// Broche de validation du récepteur
#define AFSK_RXEN       PD3
#define AFSK_RXEN_PORT  PORTD
#define AFSK_RXEN_DDR   DDRD

// Pré-division timer détecteur de front
#define TDETECTOR_DIV   8

/* public constants ========================================================= */
/*
 * Vecteur interruption du démodulateur
 */
#define AFSK_vect    ADC_vect

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
INLINE void
vAfskHwDcdOn (void) {

  vLedSet (LED_RX);
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwDcdOff (void) {

  vLedClear (LED_RX);
}

/* -----------------------------------------------------------------------------
 * External
 * Cette fonction renvoie la valeur de la dernière conversion du convertisseur
 * analogique-numérique
 */
static inline int8_t
iAfskHwSignalRead (void) {

  // Mettre votre code ici si nécessaire
  TIFR1 |= _BV(ICF1);
  return ((int16_t)(ADCH) - 128);
}

// -----------------------------------------------------------------------------
// Internal
INLINE void
vAfskHwRxEnable (void) {
#if defined(AFSK_RXEN) && defined(AFSK_RXEN_PORT) && defined(AFSK_RXEN_DDR)
  AFSK_RXEN_PORT |= _BV(AFSK_RXEN);
#endif
}

// -----------------------------------------------------------------------------
// Internal
INLINE void
vAfskHwRxDisable (void) {

#if defined(AFSK_RXEN) && defined(AFSK_RXEN_PORT) && defined(AFSK_RXEN_DDR)
  AFSK_RXEN_PORT &= ~_BV(AFSK_RXEN);
#endif
}

/* -----------------------------------------------------------------------------
 * Internal
 * Cette fonction initialise le convertisseur analogique-numérique et les
 * éléments qu'il utilise.
 */
static inline void
vAfskHwRxInit (void) {

  /* Set prescaler to clk/8, CTC, top = ICR1 */
  TCCR1A = 0;
  TCCR1B = _BV(CS11) | _BV(WGM13) | _BV(WGM12);

  /* Set max value to obtain a 9600Hz freq */
  ICR1 = ((F_CPU / TDETECTOR_DIV) / 9600) - 1;

  /* Set reference to AVCC (5V), ADC left adjusted, select CH */
  ADMUX = _BV(REFS0) | _BV(ADLAR) | AFSK_ADC_SIGNAL;

  DDRC  &= ~_BV(AFSK_ADC_SIGNAL);
  PORTC &= ~_BV(AFSK_ADC_SIGNAL);
  DIDR0 |=  _BV(AFSK_ADC_SIGNAL);

  /* Set autotrigger on Timer1 Input capture flag */
  ADCSRB = _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0);

  /* Enable ADC, autotrigger, clk/16, IRQ enabled */
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2);

#if defined(AFSK_RXEN) && defined(AFSK_RXEN_PORT) && defined(AFSK_RXEN_DDR)
  /* Enable RXEN */
  AFSK_RXEN_DDR  |= _BV(AFSK_RXEN);
  vAfskHwRxEnable();
#endif
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwInit (void) {

  vLedInit();
  vAfskHwRxInit();
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
#define DEBUG_PIN_DATA    5
#define DEBUG_PIN_SCK     6
#define DEBUG_PIN_EDGE    7

/* -----------------------------------------------------------------------------
 * Désactive la broche d'horloge des bits transmis
 */
INLINE void
vAfskDebugSckOff (void) {

  // Mettre votre code ici si nécessaire
  PORTD &= ~_BV (DEBUG_PIN_SCK);
}

/* -----------------------------------------------------------------------------
 * Bascule la broche d'horloge des bits transmis
 */
INLINE void
vAfskDebugSckToggle (void) {

  // Mettre votre code ici si nécessaire
  PORTD ^= _BV (DEBUG_PIN_SCK);
}

/* ================================= External ================================*/

/* -----------------------------------------------------------------------------
 * Appelé lorsque le bit reçu est MARK (1200 Hz -> 1L)
 */
INLINE void
vAfskDebugMark (void) {

  // Mettre votre code ici si nécessaire
  vLedSet (LED_LED2);
}

/* -----------------------------------------------------------------------------
 * Appelé lorsque le bit reçu est SPACE (2200 Hz -> 0L)
 */
INLINE void
vAfskDebugSpace (void) {

  // Mettre votre code ici si nécessaire
  vLedClear (LED_LED2);
}

/* -----------------------------------------------------------------------------
 * Bascule la broche de détection des bits reçus
 */
INLINE void
vAfskDebugEdgeOn (void) {

  // Mettre votre code ici si nécessaire
  PORTD |= _BV (DEBUG_PIN_EDGE);
}

/* -----------------------------------------------------------------------------
 * Bascule la broche de détection des bits reçus
 */
INLINE void
vAfskDebugEdgeOff (void) {

  // Mettre votre code ici si nécessaire
  PORTD &= ~_BV (DEBUG_PIN_EDGE);
}


/* -----------------------------------------------------------------------------
 * Active la broche chargée d'indiquer la valeur du bit
 */
INLINE void
vAfskDebugData (bool bBit) {

  // Mettre votre code ici si nécessaire
  if (bBit) {
    PORTD |= _BV (DEBUG_PIN_DATA);
  }
  else {
    PORTD &= ~_BV (DEBUG_PIN_DATA);
  }
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
INLINE void
vAfskDebugInit (void) {

  // Mettre votre code ici si nécessaire
  DDRD |= _BV (DEBUG_PIN_DATA) | _BV (DEBUG_PIN_SCK) | _BV (DEBUG_PIN_EDGE);
  PORTD &= ~_BV (DEBUG_PIN_DATA);
  vAfskDebugSckOff();
}
#else
/* ================================= External ================================*/
#define vAfskDebugInit()
#define vAfskDebugData(v)
#define vAfskDebugEdgeOn()
#define vAfskDebugEdgeOff()
#define vAfskDebugMark()
#define vAfskDebugSpace()
#endif

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_BOARD_AFSK_KIWI_H_ not defined */

