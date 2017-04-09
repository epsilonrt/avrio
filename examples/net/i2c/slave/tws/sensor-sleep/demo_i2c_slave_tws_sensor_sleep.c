/**
 * @file
 * Test Capteur I2C (TWS) avec sommeil 
 *
 * Ce programme simule un capteur I2C. Il utilise le module TWS présent sur 
 * les MCU AtTiny20/40/441/841/1634. \n
 * Le maître peut faire un accès en lecture simple, ou en lecture-écriture de
 * façon à accèder à un registre particulier. Chaque valeur mesurée est stockée
 * dans un registre 16 bits, octet de poids faible en premier. \n 
 * Le programme utilise le chien de garde pour cadencer les séquences de mise
 * en sommeil. \n
 * Sans traffic I2C, le MCU est réveillé toutes les 8 secondes. \n
 * Le timer chien de garde est remis à zéro à chaque échange I2C avec un délai
 * de 15ms. Le MCU sera donc endormi 15ms après le dernier échange I2C pour 8s. \n
 * Une incrémentation de tous les registres capteurs est effectuée toutes les 4 
 * interruptions chien de garde (simulation d'une évolution capteur). \n
 * LED1 est allumée lorsque le MCU est réveillé. \n
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/mutex.h>
#include <avrio/tws.h>

/* constants ================================================================ */
// Adresse de notre circuit sur le bus I2C (alignée à droite)
#define OWN_ADDRESS (0x10)
// Nous simulons 5 capteurs, chaque valeur mesurée est sur 16 bits
// Dans cet exemple nous mesurons les voies 0 à 4 de l'ADC
#define NOF_REGISTERS 5

/* private variables ======================================================== */
// buffer de stockage des valeurs pour les 5 capteurs
static volatile uint8_t buffer[NOF_REGISTERS * 2];
// index de lecture du buffer
static volatile uint8_t rd_index;
// mutex permet de détecter un déclenchement watch-dog depuis la fonction main()
static xMutex mutex = MUTEX_LOCK;

/* private functions ======================================================== */
// Routine d'interruption du watchdog
ISR (WDT_vect) {

  // Dévérouille le mutex pour indiquer au main() qu'on peut s'endormir
  vMutexUnlock (&mutex);
}

/* main ===================================================================== */
int
main (void) {
  uint8_t wdt_count = 0;

  /*
   * On dévalide tous les modules afin d'économiser de l'énergie...
   * Puis on valide uniquement les modules utilisés. La liste peut être
   * consultée sur dans la doc. de avr-libc:
   * http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
   */
  power_all_disable();
  power_twi_enable();
  //power_spi_enable();
  //power_adc_enable();
  //power_timer0_enable();
  //power_timer1_enable();
  //power_timer2_enable();
  //power_usart0_enable();
  //power_usart1_enable();

  vLedInit ();
  vLedSet (LED_LED1); // on est réveillé...
  // Init des registres I2C capteur
  for (uint8_t i = 0; i < NOF_REGISTERS; i++) {

    // l'octet de poids fort vaut 1 pour le premier capteur, 2 pour le 2ème ...
    // c'est bidon mais cela permet de dissocier les capteurs à la lecture
    buffer [i * 2 + 1] = i + 1;
  }

  // Init du module TWS
  vTwsInit (OWN_ADDRESS << 1);

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
  // On démarre le watchdog avec un délai de 8s.
  wdt_enable (WDTO_8S);
  WDTCSR |= _BV (WDIE); // on valide le mode interruption du watchdog

  // On valide les interruptions
  sei();

  for (;;) {

    if (xMutexTryLock (&mutex) == 0) {

      // Il y a eu une interruption watch-dog, on le redémarre 
      wdt_enable (WDTO_8S);
      WDTCSR |= _BV (WDIE); // on valide le mode interruption du watchdog

      wdt_count++;
      if (wdt_count % 4 == 0) {

        // Mise à jour des octets LSB des registres
        for (uint8_t i = 0; i < sizeof (buffer); i += 2) {
          uint8_t r = buffer[i];
          
          r++;
          ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
            buffer[i] = r; // la modification des registres doit être protégée
            // d'un accès concurent par une routine d'interruption
          }
        }
      }

      vLedClear (LED_LED1); // Il est temps de faire dodo....
      
      cli(); // on désactive les interruptions car la procédure ne doit pas être interrompue (atomique)
      // On enclenche la procédure de sommeil
      sleep_enable();
      sei(); // on valide les interruptions (qui vont réveiller le MCU)
      
      sleep_cpu(); // dodo
      // On vient d'être réveillé...
      sleep_disable(); // on désactive le sleep, allez au boulot !
      vLedSet (LED_LED1); // on est réveillé...
    }
  }

  return 0;
}

// -----------------------------------------------------------------------------
// Accès en lecture du maître
// Fonction exécutée sous interruption, il faut être très bref !
uint8_t
ucTwsToMaster (void) {

  wdt_enable (WDTO_15MS);
  WDTCSR |= _BV (WDIE); // on valide le mode interruption du watchdog

  if (rd_index >= sizeof (buffer)) {

    // si l'index dépasse la taille du buffer, il faut le remettre à 0
    rd_index = 0;
  }
  return buffer[rd_index++];
}

// -----------------------------------------------------------------------------
// Accès en écriture du maître
// Fonction exécutée sous interruption, il faut être très bref !
// Cette fonction correspond à une modification du numéro de registre à lire
// qui correspond à la variable index dans cet exemple.
void
ucTwsFromMaster (uint8_t data) {

  wdt_reset();
  rd_index = data;
}

// -----------------------------------------------------------------------------
// Permet de dévalider le chien de garde au reset
// cf http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
void get_mcusr (void) \
__attribute__ ( (naked)) \
__attribute__ ( (section (".init3")));
void get_mcusr (void) {
  MCUSR = 0;
  wdt_disable();
}

/* ========================================================================== */
