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
#include "avrio-board-adc.h"
#include <avrio/adc.h>
#include <avrio/delay.h>

#ifndef ADC_AVERAGE_DELAYUS
#define ADC_AVERAGE_DELAYUS 100
#endif

/* macros =================================================================== */
#if defined(PRR) && defined(PRADC)
# define SET_PRADC() do { PRR |= _BV(PRADC); } while (0)
# define CLR_PRADC() do { PRR &= ~_BV(PRADC); } while (0)
#else
# define SET_PRADC()
# define CLR_PRADC()
#endif

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static inline uint16_t
prvusRead (void) {

  ADCSRA |= _BV(ADSC);
  while ((ADCSRA & _BV(ADIF)) == 0)
    ;
  ADCSRA |= _BV(ADIF);
  return ADC;
}

// -----------------------------------------------------------------------------
static inline  uint16_t
prvusAverageRead (uint8_t ucTerms) {
  uint16_t usValue = 0;
  uint8_t ucCount = ucTerms;

  while (ucCount--) {

    usValue += prvusRead();
    delay_us (ADC_AVERAGE_DELAYUS);
  }
  return usValue / ucTerms;
}

#ifdef ADC_SCALE_ENABLE
/* private variables ======================================================== */
static uint8_t ucAdcScale[ADC_CHAN_QUANTITY];
static const uint8_t ucAdcMaxScale[ADC_CHAN_QUANTITY] = ADC_SCALE_MAX_LIST;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vAdcInit (void) {

  ADMUX  =  ADC_REF;
  vAdcEnable();
  vAdcHwInitScale();
  for (uint8_t c = 0; c < ADC_CHAN_QUANTITY; c++) {

    vAdcSetScale (c, 0);
  }
}

// -----------------------------------------------------------------------------
void
vAdcSetScale (uint8_t ucChannel, uint8_t ucScale) {

  if (ucChannel < ADC_CHAN_QUANTITY) {
    uint8_t ucMaxScale = ucAdcMaxScale[ucChannel];

    if ((ucMaxScale) && (ucScale <= ucMaxScale)) {

      ucAdcScale[ucChannel] = ucScale;
      vAdcHwSetScale (ucChannel, ucScale);
    }
  }
}

// -----------------------------------------------------------------------------
uint8_t
ucAdcGetScale (uint8_t ucChannel) {

  if (ucChannel < ADC_CHAN_QUANTITY) {

    return ucAdcScale[ucChannel];
  }
  return 0xFF;
}

// -----------------------------------------------------------------------------
uint8_t
ucAdcGetScaleMax (uint8_t ucChannel) {

  if (ucChannel < ADC_CHAN_QUANTITY) {

    return ucAdcMaxScale[ucChannel];
  }
  return 0xFF;
}

#else
// -----------------------------------------------------------------------------
void
vAdcInit (void) {

  ADMUX  =  ADC_REF;
  vAdcEnable();
}
#endif


#if defined(ADC_AUTOSCALE_ENABLE)
// Version Autoscale

/* public variables ========================================================= */
uint16_t usAdcAutoScaleFlag;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
uint16_t
usAdcRead (uint8_t ucChannel) {
  uint16_t usValue;

  vAdcSetChannel (ucChannel);
  if (usAdcAutoScaleFlag & _BV(ucChannel)) {

    for (uint8_t i = 0; i < ADC_AUTOSCALE_MAXLOOP; i++) {

      usValue = prvusRead();
      if (usValue >= ADC_AUTOSCALE_MAX) {
        if (ucAdcScale[ucChannel] == ucAdcMaxScale[ucChannel]) {

          break;
        }
        else {

          vAdcSetScale (ucChannel, ucAdcScale[ucChannel] + 1);
          delay_ms (100);
        }
      }
      else if (usValue <= ADC_AUTOSCALE_MIN) {
        if (ucAdcScale[ucChannel] == 0) {

          break;
        }
        else {

          vAdcSetScale (ucChannel, ucAdcScale[ucChannel] - 1);
          delay_ms (100);
        }
      }
      else {

        break;
      }
    }
  }
  else {

    usValue = prvusRead();
  }
  return usValue;
}

// -----------------------------------------------------------------------------
uint16_t
usAdcReadAverage (uint8_t ucChannel, uint8_t ucTerms) {
  uint16_t usValue;

  vAdcSetChannel (ucChannel);
  if (usAdcAutoScaleFlag & _BV(ucChannel)) {

    for (uint8_t i = 0; i < ADC_AUTOSCALE_MAXLOOP; i++) {

      usValue = prvusAverageRead (ucTerms);
      if (usValue >= ADC_AUTOSCALE_MAX) {
        if (ucAdcScale[ucChannel] == ucAdcMaxScale[ucChannel]) {

          break;
        }
        else {

          vAdcSetScale (ucChannel, ucAdcScale[ucChannel] + 1);
          delay_ms (100);
        }
      }
      else if (usValue <= ADC_AUTOSCALE_MIN) {

        if (ucAdcScale[ucChannel] == 0) {

          break;
        }
        else {

          vAdcSetScale (ucChannel, ucAdcScale[ucChannel] - 1);
          delay_ms (100);
        }
      }
      else {

        break;
      }
    }
  }
  else {

    usValue = prvusAverageRead (ucTerms);
  }
  return usValue;
}

#else
// Version non Autoscale
// -----------------------------------------------------------------------------
uint16_t
usAdcRead (uint8_t ucChannel) {

  vAdcSetChannel (ucChannel);
  return prvusRead();
}

// -----------------------------------------------------------------------------
uint16_t
usAdcReadAverage (uint8_t ucChannel, uint8_t ucTerms) {

  vAdcSetChannel (ucChannel);
  return prvusAverageRead (ucTerms);
}
#endif

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

#endif /* AVRIO_ADC_ENABLE defined */
/* ========================================================================== */
