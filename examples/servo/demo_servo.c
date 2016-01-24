/*
 * Demo servomoteur
 * Au démarrage, tous les servos reçoivent un signal périodique avec une durée
 * d'impulsion de 1500us.
 * On peut faire varier la durée d'impulsion entre 800us et 2200us par pas de 20us.
 * Chaque appui sur le bouton poussoir augmente la durée de 20us, arrivé au max
 * la durée est ramenée au min.
 * Le fichier avrio-board-servo.h est configuré pour une carte xnet-node, il 
 * faudra l'adapter en fonction de la carte cible utilisé...
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/button.h>
#include <avrio/servo.h>

/* constants ================================================================ */
#define DEFAULT_PULSE 1500
#define MIN_PULSE     800
#define MAX_PULSE     2200
#define STEP_PULSE    20

/* main ===================================================================== */
int
main (void) {
  uint16_t usPulse = DEFAULT_PULSE;

  vLedInit ();
  vButInit ();
  vServoInit (); // Toutes les voies sont validées

  // Règlage des largeurs d'impulsion par défaut
  vLedSet (LED_LED1);
  for (uint8_t ucServo = 0; ucServo < ucServoChannels(); ucServo++) {

    vServoSetPulse (ucServo, DEFAULT_PULSE);
  }

  for (;;) {

    if (xButGet (BUTTON_BUTTON1)) {
      // BP1 appuyé
      while (xButGet (BUTTON_BUTTON1))
        ; // Attente relâchement BP1
      usPulse += STEP_PULSE; // Augmente la largeur d'un pas
      // Si la largeur est trop grande, on revient au min
      if (usPulse > MAX_PULSE) {
        usPulse = MIN_PULSE;
      }
      // On envoie le nouveau réglage aux servos
      for (uint8_t ucServo = 0; ucServo < ucServoChannels(); ucServo++) {

        vServoSetPulse (ucServo, usPulse);
      }
      // On bascule l'état de la LED1
      vLedToggle (LED_LED1);
    }
  }
  return 0;
}

/* ========================================================================== */
