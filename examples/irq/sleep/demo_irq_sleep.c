/**
 * @file
 * Demo broche d'interruption et sommeil (irq, sleep)
 *
 * Le module irq permet de gérer les broches d'interruptions INT.
 * Les MCU AVR sont équipés d'un mode de sommeil (sleep) qui permet
 * de réduire la consommation à quelques micro-ampères (voir nano !).
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
 * Le microcontroleur est endormi et réveillé par les interruptions ce qui
 * réduit au minimum sa consommation.
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-irq.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <avrio/irq.h>
#include <avrio/semaphore.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avr/sleep.h>
#include <avr/power.h>

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

  /*
   * On dévalide tous les modules afin d'économiser de l'énergie...
   * Puis on valide uniquement les modules utilisés. La liste peut être
   * consultée sur dans la doc. de avr-libc:
   * http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
   */
  power_all_disable();
  //power_twi_enable();
  //power_spi_enable();
  //power_adc_enable();
  //power_timer0_enable();
  //power_timer1_enable();
  //power_timer2_enable();
  //power_usi_enable();
  power_usart0_enable();

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
  /* On choisit le mode du sommeil
   * Mode possibles (voir datasheet du MCU):
   * - SLEEP_MODE_IDLE
   * - SLEEP_MODE_PWR_DOWN
   * - SLEEP_MODE_PWR_SAVE
   * - SLEEP_MODE_ADC
   * - SLEEP_MODE_STANDBY
   * - SLEEP_MODE_EXT_STANDBY
   */
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  // On valide les interruptions
  sei();

  // Affichage de l'entête
  printf ("\n         0123456789");
  for (;;) {

    vLedSet (LED_LED1); // La LED1 est allumée lorsque le MCU est éveillé
    /*
     * xSemTryWait() retourne 0 lorsque le sémaphore à pu être décrémenté
     * c'est à dire quand une interruption a eu lieu
     */
    while (xSemTryWait (&s) == 0) {

      // Tous les 10 interruptions, on va à la ligne et on affiche le nombre
      if ( (ulCount % 10) == 0) {

        printf ("\n%8ld ", ulCount);
      }
      // On incrémente le compteur utilisateur et on affiche le point
      ulCount++;
      putchar ('.');
    }

    // Il est temps de faire dodo....
    vLedClear (LED_LED1); // on éteint la LED1
    cli(); // on désactive les interruptions car la procédure ne doit pas être interrompu (atomique)
    // On enclenche la procédure de sommeil
    sleep_enable();
    sleep_bod_disable(); // on désactive le 
    sei(); // on valide les interruptions (qui vont réveiller le MCU)
    sleep_cpu();
    // On vient d'être réveillé...
    sleep_disable(); // on désactive le sleep, allez au boulot !
  }
  return 0;
}
/* ========================================================================== */
