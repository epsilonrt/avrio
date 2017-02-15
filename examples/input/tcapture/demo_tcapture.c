/**
 * @file
 * @brief Demo entrée de capture Timer 16 bits (tcpature)
 *
 * Le module switch permet de gérer les entrées de capture ICP de timer 16 bits.
 *
 * Ce programme affiche la période et la fréquence d'un signal connecté à 
 * la broche ICP1 (IO8 sur Arduino UNO) :
 *
 * count,period,freq
 * 73,3.65e-05,27397.26
 * ...
 *
 * Il est prévu pour une carte Arduino UNO, mais fonctionnera sur tout MCU
 * équipé d'un timer 16 bits.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/tcapture.h>

/* constants ================================================================ */
#define ICP TCAPTURE1 // choix du timer, ici TIMER1
#define BAUDRATE 115200
#define PORT "tty0"

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  // Init. du module
  vTCaptureInit();
  // Capture entre fronts montants
  vTCaptureSetEvent (ICP, eTCaptureRising);
  // Active le filtrage de bruit en entrée
  vTCaptureSetNoiseFilter (ICP, true);
  /* Règle la prédivision d'horloge qui fixe la résolution de mesure
   * sur un Arduino à 16 MHz, une division d'horloge par 64 donne une résolution
   * de 4 µs
   */
  vTCaptureSetPrescaler (ICP, eTCaptureDiv8);
  // il faut valider les interruptions car le module les utilisent
  sei();
  // Affichage de l'entête
  printf ("\ncount,period,freq\n");

  for (;;) {

    // Test si une nouvelle mesure est disponible
    if (bTCaptureIsComplete (ICP)) {
      double f,T;
      uint16_t count;
      
      // lecture du nombre de période d'horloge du timer (avec prédivision)
      count = usTCaptureValue (ICP);
      // Conversion en période
      T = dTCaptureValueToTime (ICP, count);
      // Conversion en fréquence
      f = dTCaptureValueToFreq (ICP, count);
      printf ("%u,%.2e,%.2f\n", count, T, f);
      delay_ms (100);
      vTCaptureStart (ICP);
    }
    vLedToggle (LED_LED1);
  }
  return 0;
}
/* ========================================================================== */
