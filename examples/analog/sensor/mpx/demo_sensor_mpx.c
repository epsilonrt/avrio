/**
 * @brief Demo capteur analogique de pression (MPX4115AP)
 *
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * la pression correspondant à cette valeur brute sur la liaison série (UART0):
 * 
 * ADC0,Pression
 * 839,1015.51
 * ....
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-adc.h dans
 * le répertoire courant devra être adapté pour une autre carte.
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
  double dValue;  // pour la valeur de la pression
  xAdcSensor xSensor;
  
  /*
   * Repère d'étalonnage utilisé pour le test
   *
   * Le capteur est un modèle MPX4115AP de Freescale, sa tension de sortie est:
   * Vout = Vcc*(.009*P-.095) avec P pression en kPa
   * il s'agit bien d'un capteur linéaire.
   * Le capteur mesure entre 150 hPa (15 kPa) et 1150 hPa (115 kPa).
   * Pour Vcc=Vref=5V, le quantum de l'ADC vaut LSB=5/1024=4.88mV, ce qui donne:
   * Vout(15)  = 200mV  = 41 LSB
   * Vout(115) = 4700mV = 963 LSB
   * Ce qui donne le repère xSetting = (150, 1150, 41, 963)
   */
  xAdcSensorSetting xSetting = ADC_SENSOR_SETTING_LINEAR(150, 1150, 41, 963);

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vLedInit(); // la LED1 est basculée à mesure
  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcVcc); // Tension de référence AVcc (5V pour Arduino UNO)
  /*
   * Capteur branché sur la voie 0 (ADC0)
   * Moyennage sur 32 mesures
   */
  vAdcSensorInit (&xSensor, &xSetting, ADC_SENSOR_LINEAR, 0, 32);

  printf ("ADC0,Pression\n");
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
