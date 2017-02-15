/**
 * @brief Demo broche d'interruption et semaphore (irq,semaphore)
 *
 * Le module irq permet de gérer les broches d'interruptions INT.
 *
 * Ce programme affiche un point sur la liaison série à chaque front descendant
 * sur la broche INT0 :
 *
 *        0123456789
 *      0 ..........
 *     10 ..........
 *     20 ....
 *
 * La broche INT0 est configurée en entrée avec pull-up. Un bouton poussoir
 * (ou tout autre dispositif similaire) y est connecté.
 * Un sémaphore est utilisé pour compter le nombre de fronts descendants dans
 * une routine d'interruption (pour synchroniser l'affichage du comptage qui
 * ne peut se faire sous interruption !).
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-irq.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <avrio/irq.h>
#include <avrio/semaphore.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* private variables ======================================================== */
// Variable sémaphore utilisée pour la synchronisation
static xSem s;

/* private functions ======================================================== */
// Routine d'interruption de broche INT
void
vIrqService (int8_t i) {

  // on incrémente le sémaphore pour signaler l'interruption
  vSemPost (&s);
}

/* main ===================================================================== */
int
main (void) {
  uint32_t ulCount = 0; // le compteur dans l'espace utilisateur

  vLedInit (); // La LED1 est allumée lorsque le MCU est éveillé
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  // On attache la routine à INT0 (front descendant)
  vIrqAttach (INT0, vIrqService, eIrqFalling);
  // Initialsation du sémaphore à 0
  vSemInit (&s, 0);
  // On valide les interruptions
  sei();

  // Affichage de l'entête
  printf ("\n         0123456789");
  for (;;) {

    /*
     * xSemTryWait() retourne 0 lorsque le sémaphore à pu être décrémenté
     * c'est à dire quand une interruption a eu lieu
     */
    while (xSemTryWait (&s) == 0) {

      vLedSet (LED_LED1); // La LED1 est allumée lorsque le MCU est éveillé
      // Tous les 10 interruptions, on va à la ligne et on affiche le nombre
      if ( (ulCount % 10) == 0) {

        printf ("\n%8ld ", ulCount);
      }
      // On incrémente le compteur utilisateur et on affiche le point
      ulCount++;
      putchar ('.');
    }

    vLedClear (LED_LED1); // on éteint la LED1
  }
  return 0;
}
/* ========================================================================== */
