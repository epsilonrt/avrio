/**
 * @file
 * Demo I2C Esclave USI (module twi-usi)
 *
 * Ce programme réalise un circuit esclave I2C qui peut être adressé par un
 * maître I2C (comme un Raspberry Pi). \n
 * Il utilise le module USI présent sur les MCU AtTiny.
 * Le maître va écrire sur le bus un octet à l'adresse OWN_ADDRESS, l'esclave
 * mémorise cet octet et bascule la LED1. Le maître peut alors lire un octet
 * à cette même adresse, sa valeur devrait être la même que l'octet écrit
 * précédement et la LED1 devrait rebasculer.
 *
 * Il est prévu pour un MCU AtTiny et certains AtMega disposant de USI, 
 * le fichier avrio-board-twi.h dans le répertoire courant pourra être adapté.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/twi-usi.h>

/* constants ================================================================ */
// Adresse de notre circuit sur le bus I2C (alignée à droite)
#define OWN_ADDRESS (0x10)

/* private variables ======================================================== */
static volatile uint8_t ucLastByte;

/* bLedOn permet de valider le basculement de l'état de la LED.
 * La LED va basculer dans les cas suivants:
 * - à chaque écriture du maitre
 * - à chaque lecture du maitre non précédée d'une ecriture
 * De fait, une lecture précédée d'une écriture produit un seul basculement
 * pour les 2 opérations.
 */
static volatile bool bLedOn = true;

/* private functions ======================================================== */
/* -----------------------------------------------------------------------------
 * Gestionnaire de tansmission esclave
 * Exécuté sous interruption dès que le maître demande à lire un octet et que le
 * buffer de transmission est vide.
 */
void 
vTxHandler (void) {

  /* Stocke le dernier octet dans le buffer de transmission afin qu'il soit
   * lu par le maître
   */
  vTwiUsiSlaveWrite (ucLastByte);
  
  if (bLedOn) {
  
    vLedToggle (LED_LED1);
  }
  else {
  
    bLedOn = true;
  }
}

/* main ===================================================================== */
int
main (void) {
  
  vLedInit ();
  vTwiUsiSlaveInit (OWN_ADDRESS << 1);
  vTwiUsiSlaveRegisterTxHandler (vTxHandler);
  sei(); // Valide les interruptions.

  for (;;) {

    // Scrute si des octets sont dans le buffer de réception
    if (xTwiUsiSlaveCharIsReceived()) {
    
      // Si oui, mémorise l'octet et bascule l'état de la LED
      ucLastByte = ucTwiUsiSlaveRead();
      vLedToggle (LED_LED1);
      bLedOn = false;
    }
  }
  return 0;
}

/* ========================================================================== */
