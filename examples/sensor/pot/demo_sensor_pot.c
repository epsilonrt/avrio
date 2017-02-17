/**
 * @file
 * Demo capteur analogique angulaire (potentiomère)
 *
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * l'angle correspondant à cette valeur brute sur la liaison série (UART0):
 * 
 * ADC0,Angle
 * 367,96.86
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
  double dValue;  // pour la valeur de l'angle
  xAdcSensor xSensor;
  
  /*
   * Repère d'étalonnage utilisé pour le test
   *
   * Le potentiomètre est un modèle linéaire monotour 0 à 270° connecté entre
   * GND et AREF dont le curseur est connecté à une voie de l'ADC.
   * à zéro (angle = 0°) l'ADC fourni 0, au max. (angle = 270°) l'ADC fourni 1023.
   * Ce qui donne le repère xSetting = (0, 270, 0, 1023)
   */
  xAdcSensorSetting xSetting = ADC_SENSOR_SETTING_LINEAR(0, 270, 0, 1023);

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vLedInit(); // la LED1 est basculée à mesure
  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcInternal); // Tension de référence interne (1.1V pour Arduino UNO)
  /*
   * Curseur du potentiomètre branché sur la voie 0 (ADC0)
   * Moyennage sur 32 mesures
   */
  vAdcSensorInit (&xSensor, &xSetting, ADC_SENSOR_LINEAR, 0, 32);

  printf ("ADC0,Angle\n");
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
