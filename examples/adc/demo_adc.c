/**
 * Demo Convertisseur Analogique-Numérique (ADC)
 *
 * Ce programme affiche des valeurs en LSB (0 à 1023) des voies ADC de la façon
 * suivante sur la liaison série (UART0):
 * 
 * ADC0,ADC1,ADC2,ADC3,ADC4,ADC5
 * 1023,1023,1023,1023,1023,1023
 * ....
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-adc.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/adc.h>
#include <stdio.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* main ===================================================================== */
int
main (void) {
  uint16_t usAdc; // pour valeur ADC
  char sep = ','; // caractère de séparation entre valeurs

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  vLedInit(); // la LED1 est basculée à série de mesures
  vAdcInit(); // Init de l'ADC
  vAdcSetRef (eAdcInternal); // Tension de référence interne (1.1V pour Arduino UNO)

  // Affiche la ligne d'entête: ADC0,ADC1,...
  for (int i = 0; i < ADC_CHAN_QUANTITY; i++) {

    if (i == (ADC_CHAN_QUANTITY - 1)) {
      
      // saut de ligne après dernière étiquette
      sep = '\n';
    }
    printf ("ADC%u%c", i, sep);
  }

  for (;;) {
    sep = ',';

    // Affiche les valeurs ADC
    for (int i = 0; i < ADC_CHAN_QUANTITY; i++) {

      usAdc  = usAdcReadAverage (i, 8); // lecture avec moyennage sur 8 valeurs
      if (i == (ADC_CHAN_QUANTITY - 1)) {
        
      // saut de ligne après dernière valeur
        sep = '\n';
      }
      printf ("%u%c", usAdc, sep);
    }

    vLedToggle (LED_LED1); // Bascule LED1 après chaque série de mesure
    delay_ms (1000);
  }
  return 0;
}
/* ========================================================================== */
