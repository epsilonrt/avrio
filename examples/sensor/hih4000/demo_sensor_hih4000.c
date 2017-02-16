/**
 * @file
 * Demo capteur analogique d'humidité (HIH4030)
 *
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * l'humidité correspondant à cette valeur brute sur la liaison série (UART0):
 * 
 * ADC0,Humidite
 * 549,56.08
 * ....
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-adc.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/adc.h>
#include <avrio/adc_sensor.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* main ===================================================================== */
int
main (void) {
  uint16_t usAdc; // pour valeur ADC
  double dValue;  // pour la valeur de l'humidité
  xAdcSensor xSensor;
  
  /*
   * Repère d'étalonnage utilisé pour le test
   *
   * Le capteur est un modèle HIH4030 de Honeywell. Il est connecté à la
   * broche ADC0. C'est un capteur linéaire.
   * Le datasheet nous donne les valeurs de tension en sortie:
   * V1 = Vout(0%)    = 958mV
   * V2 = Vout(75.3%) = 3268mV
   * comme Vref=5V, le quantum de l'ADC vaut LSB=5/1024=4.88mV donc :
   * V1 = Vout(0%)    = 958mV  et ADC = 196 LSB
   * V2 = Vout(75.3%) = 3268mV et ADC = 670 LSB
   * Ce qui donne le repère xSetting = (0, 75.3, 196, 670)
   */
  xAdcSensorSetting xSetting = ADC_SENSOR_SETTING_LINEAR(0, 75.3, 196, 670);

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vLedInit(); // la LED1 est basculée à mesure
  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcVcc); // Tension de référence AVcc (5V pour Arduino UNO)
  /*
   * Le capteur est branché sur la voie 0 (ADC0)
   * Moyennage sur 32 mesures
   */
  vAdcSensorInit (&xSensor, &xSetting, ADC_SENSOR_LINEAR, 0, 32);

  printf ("ADC0,Humidite\n");
  for (;;) {

    /*
     * Lecture de la valeur brute en sortie de l'ADC :
     * Si on n'a pas besoin de usAdc, on peut utiliser dAdcSensorGetValue()
     * à la place de usAdcSensorGetRaw() et dAdcSensorRawToValue()
     */
    usAdc = usAdcSensorGetRaw (&xSensor);
    // Conversion de la valeur brute en grandeur mesurée.
    dValue = dAdcSensorRawToValue (&xSensor, usAdc);
    // Affichage des valeurs
    printf ("%u,%.02f\n", usAdc, dValue);

    vLedToggle (LED_LED1); // Bascule LED1 après chaque mesure
    delay_ms (500);
  }

  return 0;
}

/* ========================================================================== */
