/**
 * @file
 * Demo Capteur I2C (TWS)
 *
 * Ce programme simule un capteur I2C. Il utilise le module TWS présent sur 
 * les MCU AtTiny20/40/441/841/1634. \n
 * Le maître peut faire un accès en lecture simple, ou en lecture-écriture de
 * façon à accèder à un registre particulier. Chaque valeur mesurée est stockée
 * dans un registre 16 bits, octet de poids faible en premier. Le programme
 * procède à une incrémentation d'une des valeurs toutes les 2 secondes.
 */
#include <util/atomic.h>

#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tws.h>

/* constants ================================================================ */
// Adresse de notre circuit sur le bus I2C (alignée à droite)
#define OWN_ADDRESS (0x10)
// Nous simulons un capteur fournissant 5 valeurs mesurée, chaque valeur mesurée 
// est sur 16 bits.
#define NOF_REGISTERS 5

/* private variables ======================================================== */
// buffer de stockage pour les 5 valeurs
static volatile uint8_t buffer[NOF_REGISTERS * 2];
// index de lecture du buffer
static volatile uint8_t index;

/* main ===================================================================== */
int
main (void) {
  uint8_t i;

  vLedInit ();
  for (i = 0; i < NOF_REGISTERS; i++) {

    // l'octet de poids fort vaut 1 pour le premier capteur, 2 pour le 2ème ...
    // c'est bidon mais cela permet de dissocier les capteurs à la lecture
    buffer [i * 2 + 1] = i + 1;
  }

  vTwsInit (OWN_ADDRESS << 1);
  // On valide les interruptions
  sei();

  for (;;) {

    if (i >= NOF_REGISTERS) {

      // si l'index dépasse le nombre de registres, il faut le remettre à 0
      i = 0;
    }
    
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      // la mise à jour nécessite de ne pas être interrompu
      buffer[i * 2]++;
    }
    i++;
    
    delay_ms (2000);
  }

  return 0;
}

// -----------------------------------------------------------------------------
// Accès en lecture du maître
// Fonction exécutée sous interruption, il faut être très bref !
uint8_t
ucTwsToMaster (void) {

  if (index >= sizeof (buffer)) {

    // si l'index dépasse la taille du buffer, il faut le remettre à 0
    index = 0;
  }
  if ((index & 1) == 0) {
    
    // Basculement de la led si index est pair
    vLedToggle (LED_LED1);
  }
  return buffer[index++];
}

// -----------------------------------------------------------------------------
// Accès en écriture du maître
// Fonction exécutée sous interruption, il faut être très bref !
// Cette fonction correspond à une modification du numéro de registre à lire
// qui correspond à la variable index dans cet exemple.
void
ucTwsFromMaster (uint8_t data) {

  index = data;
}
/* ========================================================================== */
