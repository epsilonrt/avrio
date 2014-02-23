/**
 * @file avrio-board-afsk.h
 * @brief AFSK modem hardware-specific definitions.
 * @author Francesco SacAFSK_ADC_CHi <batt@develer.com>
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 */
#ifndef _AVRIO_BOARD_AFSK_H_
#define _AVRIO_BOARD_AFSK_H_
/* ========================================================================== */
#include <avr/io.h>
#include <avrio/defs.h>

// Voie ADC utilisée pour l'entrée de démodulation
#define AFSK_ADC_CH     3

// Configuration DAC 4 bits
#define AFSK_DAC_LSB    PB3
#define AFSK_DAC_DDR    DDRD
#define AFSK_DAC_PORT   PORTD
#define AFSK_DAC_MASK   (0x0F<<AFSK_DAC_LSB)

// Broche PTT (Push To Talk) ou Led transmission
#define AFSK_TXEN       PB1
#define AFSK_TXEN_DDR   DDRB
#define AFSK_TXEN_PORT  PORTB

/* -----------------------------------------------------------------------------
 * Cette fonction initialise le convertisseur analogique-numérique et les
 * éléments qu'il utilise.
 */
static inline void
vAfskAdcInit (void) {

  /* Set prescaler to clk/8, CTC, top = ICR1 */
  TCCR1A = 0;
  TCCR1B = _BV(CS11) | _BV(WGM13) | _BV(WGM12);

  /* Set max value to obtain a 9600Hz freq */
  ICR1 = ((F_CPU / 8) / 9600) - 1;

  /* Set reference to AVCC (5V), select CH */
  ADMUX = _BV(REFS0) | AFSK_ADC_CH;

  DDRC  &= ~_BV(AFSK_ADC_CH);
  PORTC &= ~_BV(AFSK_ADC_CH);
  DIDR0 |=  _BV(AFSK_ADC_CH);

  /* Set autotrigger on Timer1 Input capture flag */
  ADCSRB = _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0);

  /* Enable ADC, autotrigger, clk/16, IRQ enabled */
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2);
}

/* -----------------------------------------------------------------------------
 * Cette fonction renvoie la valeur de la dernière conversion du convertisseur
 * analogique-numérique et peut si nécessaire acquiter son interruption.
 */
static inline uint16_t
usAfskAdcRead (void) {

  TIFR1 = _BV(ICF1);
  return ADC;
}

/* -----------------------------------------------------------------------------
 * Cette fonction initialise le convertisseur numérique-analogique et les
 * éléments qu'il utilise.
 */
static inline void
vAfskDacInit (void) {

  AFSK_DAC_DDR  |= AFSK_DAC_MASK;
  AFSK_TXEN_DDR |= _BV(AFSK_TXEN);
}

/* -----------------------------------------------------------------------------
 * Cette fonction écrit dans le convertisseur numérique-analogique. Le DAC est
 * utilisé pour générer une sinusoide qui évolue entre 0 et 255.
 * @param value Valeur à écrire en sortie du convertisseur numérique-analogique.
 */
static inline void
vAfskDacWrite (uint8_t value) {

  AFSK_DAC_PORT = (value & 0xF0) >> 1;
}

/* -----------------------------------------------------------------------------
 * La transmission radio est généralement half-duplex. Cette fonction active
 * la partie transmission (PTT). Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
static inline void
vAfskTxEnable (void) {

  AFSK_TXEN_PORT |= _BV(AFSK_TXEN);
}

/* -----------------------------------------------------------------------------
 * La transmission radio est généralement half-duplex. Cette fonction désactive
 * la partie transmission (PTT). Dans le cas où cette fonction n'est pas utile,
 * elle devra être implémentée sous forme d'une fonction inline vide.
 */
static inline void
vAfskTxDisable (void) {

  AFSK_TXEN_PORT &= ~_BV(AFSK_TXEN);
}

/* -----------------------------------------------------------------------------
 *
 *                 Partie utilisée pour la mise au point (debug)
 *
 * ---------------------------------------------------------------------------*/
// Mettre AFSK_DEBUG à 1 pour valider le debug
#define AFSK_DEBUG      0

/* -----------------------------------------------------------------------------
 * Active la broche chargée d'indiquer l'entrée dans la routine d'interruption
 * Fonction à laisser vide si non utilisée.
 */
static inline void
vAfskDebugIrqOn (void) {

#if AFSK_DEBUG
  // Mettre votre code ici si nécessaire
  PORTC |= _BV(0);
#endif
}

/* -----------------------------------------------------------------------------
 * Désactive la broche de debug routine d'interruption (sortie interruption)
 * Fonction à laisser vide si non utilisée.
 */
static inline void
vAfskDebugIrqOff (void) {

#if AFSK_DEBUG
  // Mettre votre code ici si nécessaire
  PORTC &= ~_BV(0);
#endif
}

/* -----------------------------------------------------------------------------
 * Active la broche chargée d'indiquer différentes actions du module
 * Fonction à laisser vide si non utilisée.
 */
static inline void
vAfskDebugActOn (void) {

#if AFSK_DEBUG
  // Mettre votre code ici si nécessaire
  PORTC |= _BV(1);
#endif
}

/* -----------------------------------------------------------------------------
 * Désactive la broche chargée d'indiquer différentes actions du module
 * Fonction à laisser vide si non utilisée.
 */
static inline void
vAfskDebugActOff (void) {

#if AFSK_DEBUG
  // Mettre votre code ici si nécessaire
  PORTC &= ~_BV(1);
#endif
}

/* -----------------------------------------------------------------------------
 * Initialise la partie matérielle utilisée éventuellement pour
 * la mise au point du module.
 */
static inline void
vAfskDebugInit (void) {

#if AFSK_DEBUG
  // Mettre votre code ici si nécessaire
  DDRC |= _BV(1) | _BV(0);
#endif
  vAfskDebugIrqOff();
  vAfskDebugActOff();
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_AFSK_H_ */
