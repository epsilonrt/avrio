/**
 * @file adc.c
 * @brief Convertisseur analogique-numérique
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_ADC_ENABLE
/* ========================================================================== */
#  include "avrio-board-adc.h"
#  include "adc.h"

/* macros =================================================================== */
#if defined(PRR) && defined(PRADC)
# define SET_PRADC() do { PRR |= _BV(PRADC); } while (0)
# define CLR_PRADC() do { PRR &= ~_BV(PRADC); } while (0)
#else
# define SET_PRADC()
# define CLR_PRADC()
#endif

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void 
vAdcInit (void) {

  ADMUX  =  ADC_REF;
  vAdcEnable();
}

// -----------------------------------------------------------------------------
void 
vAdcEnable (void) {

  CLR_PRADC();
  ADCSRA = _BV(ADEN) | _BV(ADIF) | (ADC_CLKDIV<<ADPS0);
  //(void) usAdcRead(0); // la première conversion est fausse...
}

// -----------------------------------------------------------------------------
void 
vAdcDisable (void) {

  ADCSRA = 0;
  SET_PRADC();
}

// -----------------------------------------------------------------------------
uint16_t 
usAdcRead (uint8_t ucChannel) {
  
  vAdcSetChannel (ucChannel);
  
  ADCSRA |= _BV(ADSC);
  while ((ADCSRA & _BV(ADIF)) == 0)
    ;
  ADCSRA |= _BV(ADIF);
  return ADC;
}

// -----------------------------------------------------------------------------
uint16_t 
usAdcReadAverage (uint8_t ucChannel) {
  uint16_t usValue = 0;
  uint8_t ucCount = 8;
  
  while (ucCount--) {
  
    usValue += usAdcRead (ucChannel);
  }
  return usValue / 8;
}

#endif /* AVRIO_ADC_ENABLE defined */
/* ========================================================================== */
