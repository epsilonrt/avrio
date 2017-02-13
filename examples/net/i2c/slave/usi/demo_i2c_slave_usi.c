/*
 * Ce test est conçu pour être utilisé avec un celui se trouvant dans le dossier
 * test/net/twi/master/usi-slave:
 * - Test 1
 *   Le maître du bus I2C envoie un octet à l'esclave et vérifie que l'esclave
 *   renvoie un ACK. Il affiche le résulat du test sur le terminal série.
 *   L'esclave mémorise l'octet et bascule l'état de la LED
 * - Test 2
 *   Le maître du bus I2C lit un octet dans l'esclave et vérifie que l'esclave
 *   renvoie un ACK. Il affiche le résulat du test sur le terminal série.
 *   L'esclave renvoie le dernier octet reçu et bascule l'état de la LED
 * 
 * Les 2 tests peuvent être enchainés, soit au coup par coup (appui sur la barre
 * espace côté maître) soit très rapidement de façon à stresser l'esclave.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/twi-usi.h>

/* constants ================================================================ */
// Adresse de notre circuit sur le bus I2C (alignée à gauche)
#define OWN_ADDRESS (0x20)

/* private variables ======================================================== */
static volatile uint8_t ucLastWrite;

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
  vTwiUsiSlaveWrite (ucLastWrite);
  
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
  vTwiUsiSlaveInit (OWN_ADDRESS);
  vTwiUsiSlaveRegisterTxHandler (vTxHandler);
  sei(); // Valide les interruptions.

  for (;;) {

    // Scrute si des octets sont dans le buffer de réception
    if (xTwiUsiSlaveCharIsReceived()) {
    
      // Si oui, mémorise l'octet et bascule l'état de la LED
      ucLastWrite = ucTwiUsiSlaveRead();
      vLedToggle (LED_LED1);
      bLedOn = false;
    }
  }
  return 0;
}

/* ========================================================================== */
