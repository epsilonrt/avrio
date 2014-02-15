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

#define AFSK_ADC_CH 3
#define AFSK_DAC_LSB 3
#define AFSK_DAC_MASK (0x0F<<AFSK_DAC_LSB)
#define AFSK_TXEN   PB3


/*
 * Activate strobe pin. We use it for debugging purposes. If you don't use it, simply
 * leave empty the following macros
 */
#define AFSK_STROBE_INIT() do { DDRB |= BV(5); } while (0)

/*
 * Set the pin high. This macro is called at the beginning of the interrupt routine
 */
#define AFSK_STROBE_ON()   do { PORTB |= BV(5); } while (0)

/*
 * Set the pin low. This macro is called at the end of the interrupt routine
 */
#define AFSK_STROBE_OFF()  do { PORTB &= ~BV(5); } while (0)


/* -----------------------------------------------------------------------------
 * Initialize the ADC for AFSK needs.
 * The adc should be configured to have a continuos stream of convertions.
 * For every convertion there must be an ISR that read the sample
 * and call vAddAdcSample(), passing the context and the sample.
 */
static inline
vAfskAdcInit (void) {

  AFSK_STROBE_INIT();
  AFSK_STROBE_OFF();

  /* Set prescaler to clk/8 (2 MHz), CTC, top = ICR1 */
  TCCR1A = 0;
  TCCR1B = BV(CS11) | BV(WGM13) | BV(WGM12);

  /* Set max value to obtain a 9600Hz freq */
  ICR1 = ((F_CPU / 8) / 9600) - 1;

  /* Set reference to AVCC (5V), select CH */
  ADMUX = BV(REFS0) | AFSK_ADC_CH;

  DDRC  &= ~BV(AFSK_ADC_CH);
  PORTC &= ~BV(AFSK_ADC_CH);
  DIDR0 |= BV(AFSK_ADC_CH);

  /* Set autotrigger on Timer1 Input capture flag */
  ADCSRB = BV(ADTS2) | BV(ADTS1) | BV(ADTS0);

  /* Enable ADC, autotrigger, 1MHz, IRQ enabled */
  /* We are using the ADC a bit out of specifications otherwise it's not fast enough for our
   * purposes */
  ADCSRA = BV(ADEN) | BV(ADSC) | BV(ADATE) | BV(ADIE) | BV(ADPS2);
}

/* -----------------------------------------------------------------------------
 * Clear the ADC interrupt request flag
 */
static inline void
vAfskAdcIrqClear (void) {

  TIFR1 = BV(ICF1);
}

/* -----------------------------------------------------------------------------
 * Initialize the DAC for AFSK needs.
 * The DAC has to be configured in order to call an ISR for every sample sent.
 * The DAC doesn't have to start the IRQ immediatly but have to wait
 * the AFSK driver to call vAfskTxEnable().
 * The ISR must then call ucNextDacSample() passing the AFSK context.
 */
static inline
vAfskDacInit (void) {

  DDRD |= AFSK_DAC_MASK;
  DDRB |= BV(AFSK_TXEN);
}


/* -----------------------------------------------------------------------------
 * Start DAC convertions
 */
static inline
vAfskDacWrite (uint8_t value) {

  PORTD = (PORTD & ~AFSK_DAC_MASK) | ((value & 0x0F) << AFSK_DAC_LSB);
}

/* -----------------------------------------------------------------------------
 * Start DAC convertions
 */
static inline
vAfskTxEnable (void) {

  PORTB |= BV(AFSK_TXEN);
}

/* -----------------------------------------------------------------------------
 * Stop DAC convertions
 */
static inline
vAfskTxDisable (void) {

  PORTB &= ~BV(AFSK_TXEN);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_AFSK_H_ */
