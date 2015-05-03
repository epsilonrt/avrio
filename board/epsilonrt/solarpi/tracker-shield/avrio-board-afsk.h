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
 * @file avrio-board-afsk.h
 * @brief Configuration et driver du module AFSK
 * Copyright (c) 2013 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20131208 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_AFSK_H_
#define _AVRIO_BOARD_AFSK_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
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
 * Constante à utiliser pour la configuration du module dans le fichier
 * avrio-cfg-afsk.h grâce à la macro CONFIG_AFSK_FILTER
 */
#define AFSK_BUTTERWORTH  0
#define AFSK_CHEBYSHEV    1
#define AFSK_FIR          2

/*
 * Configuration du filtre numérique à utiliser par le module
 */
#define CONFIG_AFSK_FILTER AFSK_CHEBYSHEV


/*
 * Configuration de la taille du buffer de réception utilisé par le module
 */
#define CONFIG_AFSK_RX_BUFLEN 64

/*
 * Configuration de la taille du buffer d'émission utilisé par le module
 */
#define CONFIG_AFSK_TX_BUFLEN 64

/*
 * Configuration du nombre de flags HDLC en début de trame
 * Le nombre de flags est ajouté en début de trame afin de synchroniser le
 * récepteur. La valeur minimale est de 1.
 */
#define CONFIG_AFSK_PREAMBLE_LEN 32

/*
 * Configuration du nombre de flags HDLC en fin de trame
 * Le nombre de flags est ajouté en fin de trame afin de synchroniser le
 * récepteur. La valeur minimale est de 1.
 */
#define CONFIG_AFSK_TRAILER_LEN 8

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
#define AFSK_ADC_CH     3

// Configuration DAC 4 bits
#define AFSK_DAC_LSB    PB3
#define AFSK_DAC_DDR    DDRD
#define AFSK_DAC_PORT   PORTD
#define AFSK_DAC_MASK   (0x0F<<AFSK_DAC_LSB)

// Pré-division timer modulateur
#define TMODEM_DIV      8

// Pré-division timer détecteur de front
#define TDETECTOR_DIV   8

/* public constants ========================================================= */

/*
 * Nombre de bits du DAC
 * A modifier si vous n'utilisez pas le DAC 4 bits GPIO à résistances
 */
#define AFSK_DAC_RESOLUTION 4

/*
 * Vecteur interruption du timer modulateur
 *
 * Cette routine d'interruption est chargée de générer le signal en sortie
 * du modulateur.
 *
 * La modification de ce vecteur nécessite une modification de la fonction
 * vAfskHwTxInit()
 */
#define AFSK_TXONLY_vect    TIMER0_COMPA_vect

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

#ifdef AFSK_RX_DISABLE
#define AFSK_vect    AFSK_TXONLY_vect
#else
#define AFSK_vect    ADC_vect
#endif

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

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwDcdOn (void) {

  // Mettre votre code ici si nécessaire
  vLedSet (LED_RX);
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwDcdOff (void) {

  // Mettre votre code ici si nécessaire
  vLedClear (LED_RX);
}

/* -----------------------------------------------------------------------------
 * External
 * Cette fonction renvoie la valeur de la dernière conversion du convertisseur
 * analogique-numérique
 */
static inline int8_t
iAfskHwAdcRead (void) {

  // Mettre votre code ici si nécessaire
  TIFR1 |= _BV(ICF1);
  return ((int16_t)(ADCH) - 128);
}

/* -----------------------------------------------------------------------------
 * Internal
 * Cette fonction initialise le convertisseur analogique-numérique et les
 * éléments qu'il utilise.
 */
static inline void
vAfskHwRxInit (void) {

  // Mettre votre code ici si nécessaire
#ifndef AFSK_RX_DISABLE
  /* Set prescaler to clk/8, CTC, top = ICR1 */
  TCCR1A = 0;
  TCCR1B = _BV(CS11) | _BV(WGM13) | _BV(WGM12);

  /* Set max value to obtain a 9600Hz freq */
  ICR1 = ((F_CPU / TDETECTOR_DIV) / 9600) - 1;

  /* Set reference to AVCC (5V), ADC left adjusted, select CH */
  ADMUX = _BV(REFS0) | _BV(ADLAR) | AFSK_ADC_CH;

  DDRC  &= ~_BV(AFSK_ADC_CH);
  PORTC &= ~_BV(AFSK_ADC_CH);
  DIDR0 |=  _BV(AFSK_ADC_CH);

  /* Set autotrigger on Timer1 Input capture flag */
  ADCSRB = _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0);

  /* Enable ADC, autotrigger, clk/16, IRQ enabled */
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2);
#endif  // AFSK_RX_DISABLE not defined
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

#ifndef AFSK_TX_DISABLE
  // Mettre votre code ici si nécessaire
  vAfskHwDacInit();

#ifdef AFSK_RX_DISABLE
  // Si le démodulateur n'est pas validé, on utilise un timer 8 bits afin de
  // libérer le Timer1 et l'ADC
  TCCR0A = _BV(WGM01);  // Mode CTC
  TIMSK0 = _BV(OCIE0A);
  OCR0A = F_CPU/AFSK_BAUDRATE/AFSK_SAMPLES_PER_BIT/TMODEM_DIV-1;
  TCCR0B = _BV(CS01);   // TIMER0_DIV = 8
#endif  // AFSK_RX_DISABLE defined
#endif  // AFSK_TX_DISABLE not defined
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAfskHwInit (void) {

  // Mettre votre code ici si nécessaire
  vLedInit();
  vAfskHwRxInit();
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
#define DEBUG_PIN_SCK     0
#define DEBUG_PIN_DATA    1
#define DEBUG_PIN_EDGE    2

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
 * Active la broche chargée d'indiquer la valeur du bit transmis
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
 * Bascule la broche de détection des bits reçus
 */
static inline void
vAfskDebugEdgeOn (void) {

  // Mettre votre code ici si nécessaire
  PORTC |= _BV(DEBUG_PIN_EDGE);
}

/* -----------------------------------------------------------------------------
 * Bascule la broche de détection des bits reçus
 */
static inline void
vAfskDebugEdgeOff (void) {

  // Mettre votre code ici si nécessaire
  PORTC &= ~_BV(DEBUG_PIN_EDGE);
}

/* -----------------------------------------------------------------------------
 * Initialise la partie matérielle utilisée éventuellement pour
 * la mise au point du module.
 */
static inline void
vAfskDebugInit (void) {

  // Mettre votre code ici si nécessaire
  DDRC |= _BV(DEBUG_PIN_DATA) | _BV(DEBUG_PIN_SCK) |  _BV(DEBUG_PIN_EDGE) ;
  PORTC &= ~_BV(DEBUG_PIN_DATA);
  vAfskDebugSckOff();
}
#else
/* ================================= External ================================*/
#define vAfskDebugInit()
#define vAfskDebugData(v)
#define vAfskDebugEdgeOn()
#define vAfskDebugEdgeOff()
#endif

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_BOARD_AFSK_H_ not defined */

