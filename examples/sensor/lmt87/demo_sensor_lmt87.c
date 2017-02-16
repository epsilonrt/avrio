/**
 * @file
 * Demo capteur analogique de température (LMT87)
 *
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * la température correspondant à cette valeur brute sur la liaison série (UART0):
 * 
 * ADC0,Temp
 * 0707,25.24
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
  double dValue;  // pour la valeur de la température
  xAdcSensor xSensor;
  
  /*
   * Repère d'étalonnage utilisé pour le test
   *
   * Le capteur est un modèle LMT87 de Texas Instruments. Il est connecté à la
   * broche ADC0 à travers un pont diviseur de tension de rapport 1/3.
   * C'est un capteur linéaire.
   * On souhaite mesurer la température entre -40 et +80 °C, le datasheet nous
   * les valeurs de tension en sortie, pour Vref=1.1V, le quantum de l'ADC vaut 
   * LSB=1.1/1024=1.07mV:
   * Vout(-40) = 3160mV donc Vadc0 = 3160/3 = 1053mV et ADC0 = 981 LSB 
   * Vout(+80) = 1539mV donc Vadc0 = 1539/3 =  513mV et ADC0 = 477 LSB 
   * Ce qui donne le repère xSetting = (-40, 80, 981, 477)
   */
  xAdcSensorSetting xSetting = ADC_SENSOR_SETTING_LINEAR(-40, 80, 981, 477);

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vLedInit(); // la LED1 est basculée à mesure
  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcInternal); // Tension de référence interne (1.1V pour Arduino UNO)
  /*
   * Capteur branché sur la voie 0 (ADC0) à travers un pont diviseur de
   * rapport 1/3, Moyennage sur 32 mesures
   */
  vAdcSensorInit (&xSensor, &xSetting, ADC_SENSOR_LINEAR, 0, 32);

  printf ("ADC0,Temp\n");
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
