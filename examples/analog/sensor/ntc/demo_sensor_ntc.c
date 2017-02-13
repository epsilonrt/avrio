/**
 * @brief Demo capteur analogique de température (CTN)
 *
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023),
 * la résistance de la CTN et la température sur la liaison série (UART0):
 *
 * ADC0,Res,Temp
 * 621,11681,20.91
 * ....
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-adc.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/adc.h>
#include <avrio/ntc.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"
#define R 18000 // Résistance fixe du pont divisieur 18k/1%

/* main ===================================================================== */
int
main (void) {
  uint16_t usAdc; // pour valeur ADC
  double dRes, dValue;  // pour la valeur de résistance et température

  /*
   * Le capteur utilisé est une CTN Murata NXFT15 10k. Il s'agit d'un composant
   * non linéaire mais à très faible coût.
   * Il est possible de linéariser la réponse en utilisant l'algorithme de
   * Steinhart-Hart tel qu'expliqué sur https://github.com/epsilonrt/ntc
   * La CTN est montée avec une résistance de 18k/1% pour constituer un pont
   * diviseur de tension de la façon suivante:
   * [Masse] --- [R:18k-1%] ---|--- [CTN] --- [AREF]
   *                           |
   *                          ADC0
   * On peut déduire la résistance de la CTN en mesurant la tension ADC0:
   * - V = R / (R + CTN) * Vref donc CTN = R ( Vref / V - 1)
   * - comme ADC = V * 1024 / Vref, il vient CTN = R * ( 1024 / ADC - 1)
   * Lorsqu'on connait la résistance, on en déduit la température à l'aide
   * de la fonction dNtcResToTemp() qui utilise l'algorithme de Steinhart-Hart.
   */
  
  /*
   * Coefficients de Steinhart-Hart de la CTN utilisée
   * Murata NXFT15 10k (Mouser 
   * Ces coefficients ont été calculés à partir du tableau fourni dans le
   * datasheet à l'aide du programme ntc-coeff (https://github.com/epsilonrt/ntc)
   */
  double dShCoeff[] = {
    9.310296797541951e-04,
    2.308343095769287e-04,
    3.001370069362199e-06,
    5.407975166655454e-08
  };

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcInternal); // Tension de référence interne (1.1V pour Arduino UNO)

  printf ("ADC0,Res,Temp\n");
  for (;;) {

    // lecture avec moyennage sur 8 valeurs
    usAdc = usAdcReadAverage  (0, 8);
    // Calcul de la résistance de la CTN
    dRes = R * ( 1024 / (double)usAdc - 1);
    // Conversion de la résistance de la CTN en température
    dValue = dNtcResToTemp(dRes, dShCoeff);
    // Affichage des valeurs
    printf ("%u,%.0f,%.02f\n", usAdc, dRes, dValue);

    delay_ms (500);
  }

  return 0;
}

/* ========================================================================== */
