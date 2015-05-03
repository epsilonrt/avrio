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
 * @file demo_adc_sensor.c
 * @brief Exemple d'utilisation de capteur linéaire analogique
 *
 * Exemple qui affiche l'angle correspondant à un potentiomètre monotour
 * sur la liaison série.
 *
 * Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
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
#define TEST_SETTING ADC_SENSOR_SETTING_LINEAR(0.0, 270.0, 0.0, 1023.0)

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
  vAdcSensorInit (&xSensor, &xSetting, ADC_SENSOR_LINEAR, 0, 32);

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
