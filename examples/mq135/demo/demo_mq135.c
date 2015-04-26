/**
 * @file test_mq135.c
 * @brief Test capteur de témpérature et humidité HIH6130 et MQ135
 *
 * Mesure la température et l'humidité et du taux de CO² toutes les secondes
 * et affiche le résultat sur le LCD.
 *
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#define __ASSERT_USE_STDERR
#include <stdio.h>
#include <stdlib.h>
#include <avrio/assert.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/hih6130.h>
#include <avrio/lcd.h>
#include <avrio/mq135.h>

/* constants ================================================================ */
#define MQ135_ADC 2       // Voie ADC du capteur
#define MQ135_RL  4.53    // RL en kilo-Ohms
#define MQ135_R0  75.61   // Résistance R0 à l'étalonnage
#define ATMO_CO2  400.23  // Teneur globale en CO² à l'étalonnage

/* private variables ======================================================== */
static xAdcSensor xMyMq135;
static xAdcSensorSetting xMyMq135Setting;
/*
 * Etalonnage de mon capteur analogique MQ135
 */
static xMq135Calibration xMyMq135Calibration =
  MQ135_CALIBRATION (MQ135_RL, MQ135_R0, ATMO_CO2);

/* main ===================================================================== */
int
main (void) {
  eHih6130Error ret;

  /* mes: Variable structure pour stocker les mesures
   * - température en dixième de degré (mes.iTemp)
   * - humidité en dixième de %RH (mes.iHum)
   */
  static  xHih6130Data mes;
  static uint16_t usRaw;
  static double dGasPpm, dGasCorrPpm;
  static double dTemp, dHum;

  vLedInit(); // Le temps d'allumage de LED_D3 correspondra au temps de mesure

  /*
   * Init du bus I2C en mode maître à 400 kHz utilisé par le capteur et le LCD
   */
  vTwiInit ();
  eTwiSetSpeed (400);

  /*
   * Init LCD, c'est la sortie standard et d'erreur
   * Dans le cas de la carte XNODE, l'afficheur est connecté par I2C
   */
  iLcdInit();
  ucLcdBacklightSet (32); // 0 à 63
  ucLcdContrastSet (16);  // 0 à 31
  stdout = &xLcd;
  stderr = &xLcd;

  /*
   * Init ADC
   */
  vAdcInit();
  // Horloge ADC à 2 MHz, une conversion prend 6.5 us
  vAdcSetDiv (4);
  // On utilise la tension d'alimentation par connexion interne
  vAdcSetRef (eAdcVcc);

  /*
   * Init capteur HIH6130
   */
  ret = eHih6130Init (0);
  assert (ret == HIH6130_SUCCESS);

  /*
   * Init du contexte du capteur MQ135
   * Le capteur est un capteur MQ135 connecté sur ADC0
   * la valeur mesurée est moyennée sur 16 termes
   */
  vMq135Init (&xMyMq135Setting, &xMyMq135Calibration);
  vAdcSensorInit (&xMyMq135, &xMyMq135Setting, ADC_SENSOR_NLINEAR, 1, 16);

  for (;;) {

    vLedSet (LED_D3);

    // Démarre la mesure (la mesure dure ~ 40 ms)
    ret = eHih6130Start();
    assert (ret == HIH6130_SUCCESS);

    do {  // Boucle d'attente de fin de mesure

      // Lecture
      ret = eHih6130Read (&mes);

      // Vérif absence d'erreur
      assert (ret >= HIH6130_SUCCESS);
    }
    while (ret == HIH6130_BUSY);
    vLedClear (LED_D3);

    dTemp = mes.iTemp / 10.0;
    dHum  = mes.iHum  / 10.0;
    usRaw = usAdcSensorGetRaw (&xMyMq135);
    dGasPpm = dMq135RawToPPM (&xMyMq135, usRaw);
    dGasCorrPpm = dMq135RawToCorrectedPPM  (&xMyMq135, usRaw, dTemp, dHum);

    vLcdClear();
    printf ("T=%.1f H=%.1f\nP=%.1f C=%.1f", dTemp, dHum, dGasPpm, dGasCorrPpm);
    delay_ms (1000);
  }
  return 0;
}
/* ========================================================================== */
