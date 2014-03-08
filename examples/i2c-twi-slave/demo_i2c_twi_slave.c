/**
 * @file demo_i2c_twi_slave.c
 * @brief Exemple d'utilisation du module I2C TWI en mode esclave
 *
 * Dans cet exemple, notre microcontroleur se comporte comme une mémoire SRAM I2C
 * similaire au PCF8583. Le maître peut y lire et y écrire, conformément au
 * datasheet du PCF8583 http://www.nxp.com/documents/data_sheet/PCF8583.pdf
 * Les fonctions d'horloge RTC du PCF8583 ne sont pas implémentées, il s'agit
 * juste de faire une démonstration !
 * En foncionnement normal, la led clignote lentement. Si une erreur survient,
 * la led flashe rapidement.
 *
 * @author Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/twi.h>
#include <avrio/led.h>
#include <avrio/delay.h>

/* constants ================================================================ */
#define LED_TEST LED_LED1

// Caractéristiques du circuit émulé
#define SLAVE_ADDR  0xA0  // émule un PCF8583
#define RAM_BASE    0x00  // Adresse du premier octet de RAM
#define RAM_SIZE    0x100 // Nombre d'octet de RAM
#define RAM_END     (RAM_BASE + RAM_SIZE - 1)

/* private variables ======================================================== */
static volatile uint8_t ucRamPtr; // Pointeur RAM: contient l'adresse du registre de RAM dans le buffer
static uint8_t ucRamBuffer[RAM_SIZE]; // Buffer RAM: contient les données de la RAM

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
// Vérifies l'absence d'erreur, si ce n'est pas le cas, bloque l'exécution
// en faisant flasher la led.
static void
prvvCheckError (eTwiStatus eError) {

  if (eError < TWI_SUCCESS) {

    for (;;) {

      vLedSet (LED_TEST);
      delay_ms (50);
      vLedClear (LED_TEST);
      delay_ms (150);
    }
  }
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vTwiInit ();
  vTwiSetDeviceAddress (SLAVE_ADDR);

  for (;;) {

    /*
     * Dans cet exemple, il n'y a rien à faire puisque l'écriture et la lecture
     * I2C sont gérées par les gestionnaires ci-dessous.
     */
    vLedToggle (LED_TEST);
    delay_ms (1000);
  }
  return 0;
}

/* internal public functions ================================================ */

/*------------------------------------------------------------------------------
 * Gestionnaire de réception I2C
 * Nous sommes adressé comme esclave en écriture, nous avons reçu une trame
 * qui se trouve dans la pile pxRxPayload.
 * Attention ! Fonction exécutée sous interruption, il faut être bref
 */
eTwiStatus
eTwiSlaveRxCB (xQueue * pxRxPayload, eTwiStatus eStatus) {
  eTwiStatus eError = TWI_SUCCESS;

  switch (eStatus) {

    case TWI_STATUS_GCALL:
      // Ecriture en appel général non implémenté !
      eError = TWI_ERROR_ADDR_NACK;
      break;

    case TWI_STATUS_LCALL:
      // Adressage avec notre adresse esclave
      if (xQueueLength (pxRxPayload) == 1) {

        /*
         * Le premier octet reçu est l'adresse du registre de RAM auquel le
         * maître souhaite accéder. Si la trame en écriture à une longeur de
         * 1 octet, seul l'adresse du registre est modifiée. Ce sera
         * généralement le cas, lors d'un accès "write word address; READ data"
         * permettant au maître de spécifier l'adresse du registre qu'il
         * souhaite lire avant de les lire.
         */
        ucRamPtr = ucQueuePull (pxRxPayload);
      }
      else if (xQueueLength (pxRxPayload) > 1) {

        ucRamPtr = ucQueuePull (pxRxPayload);
        ucRamPtr +=
           xQueuePullAll (pxRxPayload, &ucRamBuffer[ucRamPtr]);
      }
      break;

    default:
      prvvCheckError (eStatus);
      break;
  }
  return eError;
}

/*------------------------------------------------------------------------------
 * Gestionnaire d'émission I2C
 * Nous sommes adressé comme esclave en lecture, nous devons envoyer des
 * données qui devront être mis dans la pile pxTxPayload.
 * Attention ! Fonction exécutée sous interruption, il faut être bref
 */
eTwiStatus
eTwiSlaveTxCB (xQueue * pxTxPayload, eTwiStatus eStatus) {
  eTwiStatus eError = TWI_SUCCESS;

  switch (eStatus) {

    case TWI_STATUS_LCALL:
      // Adressage avec notre adresse esclave
      // Premier octet demandé

      // Pas de break, c'est normal !
    case TWI_STATUS_TXBUFFER_EMPTY:
      // Le buffer de transmission est vide, il faut ajouter des octets
      if (xQueueIsFull (pxTxPayload)) {

        eError = TWI_ERROR_BUFFER_FULL;
      } else {

        // Ajoute l'octet de RAM et incrémente de pointeur de RAM
        vQueuePush (pxTxPayload, ucRamBuffer[ucRamPtr++]);
      }
      break;

    default:
      prvvCheckError (eStatus);
      break;
  }
  return eError;
}
/* ========================================================================== */
