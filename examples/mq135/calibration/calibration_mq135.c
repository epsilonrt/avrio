/*
 * Test capteur de témpérature et humidité HIH6130
 *
 * Mesure la température et l'humidité toutes les secondes et affiche le
 * résultat sur le LCD.
 */
#define __ASSERT_USE_STDERR
#include <stdio.h>
#include <stdlib.h>
#include <avrio/assert.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/hih6130.h>
#include <avrio/mq135.h>
#include <avrio/serial_sw.h>

/* constants ================================================================ */
#define RL 4.53           // RL
#define ATMO_CO2  400.23  // Teneur globale en CO² de l'atmosphère au 26/06/15
/*
 * Nombre de capteurs à étalonner
 * Capteur 1 sur ADC0, Capteur de 2 sur ADC1, Capteur de 2 sur ADC2
 */
#define MQ135_QUANTITY 3
//#define VIEW_CORRECTED

/* private variables ======================================================== */
static xAdcSensor xMyMq135[MQ135_QUANTITY];
static xAdcSensorSetting xMyMq135Setting;
static xMq135Calibration xMyMq135Calibration = MQ135_CALIBRATION (RL, 0, ATMO_CO2);

/* main ===================================================================== */
int
main (void) {
  eHih6130Error ret;

  /* mes: Variable structure pour stocker les mesures
   * - température en dixième de degré (mes.iTemp)
   * - humidité en dixième de %RH (mes.iHum)
   */
  static  xHih6130Data mes;
  static double dRZero, dTemp, dHum;

  vLedInit(); // Le temps d'allumage de LED_D3 correspondra au temps de mesure

  /*
   * Init du bus I2C en mode maître à 400 kHz utilisé par le capteur et le LCD
   */
  vTwiInit ();
  eTwiSetSpeed (400);
  /*
   * Init capteur HIH6130
   */
  eHih6130Init (0);

  /*
   * Init ADC
   */
  vAdcInit();
  // Horloge ADC à 2 MHz, une conversion prend 6.5 us
  vAdcSetDiv (4);
  // On utilise la tension d'alimentation par connexion interne
  vAdcSetRef (eAdcVcc);

  // La liaison L1 est connectée au terminal
  vSerialSwInit ();
  stdout = &xSerialSwPort;

  // Valide les interruptions (utilisées par le module SerialSw)
  sei();
  printf ("MQ135 Calibration\n");
  printf ("T\tH\t");

  /*
   * Init du contexte du capteur MQ135
   * Le capteur est un capteur MQ135 connecté sur ADC0
   * la valeur mesurée est moyennée sur 16 termes
   */
  vMq135Init (&xMyMq135Setting, &xMyMq135Calibration);
  for (uint8_t i = 0; i < MQ135_QUANTITY; i++) {

    vAdcSensorInit (&xMyMq135[i], &xMyMq135Setting, ADC_SENSOR_NLINEAR, i, 64);
    printf ("R%d\t", i);
  }
#ifdef VIEW_CORRECTED
  for (uint8_t i = 0; i < MQ135_QUANTITY; i++) {

    printf ("Rc%d\t", i);
  }
#endif
  putchar('\n');

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
    printf ("%.1f\t%.1f\t", dTemp, dHum);

    // Valeurs non corrigées
    for (uint8_t i = 0; i < MQ135_QUANTITY; i++) {

      dRZero = dMq135CalibrateRZero (&xMyMq135[i]);
      printf ("%.2f\t", dRZero);
    }
#ifdef VIEW_CORRECTED
    // Valeurs corrigées
    vMq135SetCorrection (&xMyMq135[0], dTemp, dHum);
    for (uint8_t i = 0; i < MQ135_QUANTITY; i++) {

      dRZero = dMq135CalibrateRZero (&xMyMq135[i]);
      printf ("%.2f\t", dRZero);
    }
    vMq135SetCorrection (&xMyMq135[0], NAN, NAN);
#endif
    putchar ('\n');
    delay_ms (1000);
  }
  return 0;
}
/* ========================================================================== */
