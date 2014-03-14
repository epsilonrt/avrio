/**
 * @file demo_adc_sensor.c
 * @brief Exemple d'utilisation de capteur linéaire analogique
 *
 * Exemple qui affiche l'angle correspondant à un potentiomètre monotour
 * sur la liaison série.
 *
 * @author Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140306 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/adc.h>
#include <avrio/adc_sensor.h>

/* constants ================================================================ */
#define TEST_TITLE "\nTest unitaire Capteur Analogique\n"

/*
 * Baudrate de la liaison série
 */
#define TEST_BAUDRATE 38400

/*
 * Calage utilisé pour le test
 *
 * Le potentiomètre est un modèle linéaire monotour 0 à 270° connecté entre
 * GND et AREF dont le curseur est connecté à une voie de l'ADC.
 * A zéro (angle = 0°) l'ADC fourni 0, au max. (angle = 270°) l'ADC fourni 1023.
 */
#define TEST_SETTING ADC_SENSOR_SETTING(0.0, 270.0, 0.0, 1023.0)

/* internal public functions ================================================ */
void vStdOutInit (void);

/* main ===================================================================== */
int
main (void) {
  uint16_t usRaw;
  double dValue;
  xAdcSensor xSensor;
  xAdcSensorSetting xSetting = TEST_SETTING;

  vLedInit();
  vStdOutInit();
  vAdcInit();
  /*
   * Potar sur ADC0
   * 32 mesures pour le moyennage
   */
  vAdcSensorInit (&xSensor, &xSetting, 0, 32);

  for (;;) {

    /*
     * Si on n'a pas besoin de usRaw, on peut utiliser dAdcSensorGetValue()
     * à la place de usAdcSensorGetRaw() et dAdcSensorRawToValue()
     */
    usRaw = usAdcSensorGetRaw (&xSensor);
    dValue = dAdcSensorRawToValue (&xSensor, usRaw);
    printf ("Raw %05u Value %.02f\n", usRaw, dValue);

    vLedToggle (LED_LED1);
    delay_ms (500);
  }

  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vStdOutInit (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf (TEST_TITLE);
}

/* ========================================================================== */
