/**
 * @file
 * Demo capteur analogique de température (PT100)
 *
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * la température correspondant à cette valeur brute sur la liaison série (UART0):
 * 
 * ADC0,Temp
 * 679,21.42
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
   * Le capteur est un modèle PT100 (DIN IEC 751). C'est un capteur linéaire.
   * Il est associé à un générateur de courant constant qui fournit 1mA, la
   * tension résultante est décalée de -75mV et amplifiée de 50. 
   * La sortie de ce montage est connecté à la broche ADC0.
   * Pour une valeur R de la PT100, la tension en ADC0 vaut: 
   * Vout = (R * 1mA - 75mV) * 50
   * Le tableau IEC751nous donne les valeurs de résistance:
   * R1 = R(-60)    = 76.33  et V1 = 66.5mV
   * R2 = R(+60)    = 123.24 et V2 = 2412mV
   * Vref=2.5V (externe), le quantum de l'ADC vaut LSB=2.5/1024=2.44mV donc :
   * V1 = 66.5mV  et ADC =  27 LSB
   * V2 = 2412mV  et ADC = 988 LSB
   * Ce qui donne le repère xSetting = (-60, 60, 27, 988)
   */
  xAdcSensorSetting xSetting = ADC_SENSOR_SETTING_LINEAR(-60, 60, 27, 988);

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vLedInit(); // la LED1 est basculée à mesure
  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcInternal); // Tension de référence externe (2.5V)
  /*
   * Curseur du potentiomètre branché sur la voie 0 (ADC0)
   * Moyennage sur 32 mesures
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
