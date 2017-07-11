/**
 * @file
 * Demo esclave USI
 *
 * Ce programme réalise un circuit esclave I2C qui peut être adressé par un
 * maître I2C (comme un Raspberry Pi). \n
 * Il utilise le module USI présent sur les MCU AtTiny.
 * Le maître va écrire sur le bus un octet à l'adresse 0x10, l'esclave
 * mémorise cet octet et bascule la LED1. Le maître peut alors lire un octet
 * à cette même adresse, sa valeur devrait être la même que l'octet écrit
 * précédement.
 */
#include <avrio/led.h>
#include <avrio/usislave.h>

/* constants ================================================================ */
// Notre adresse sur le bus I²C
#define DEVICE_ID (0x10)
// Taille de notre buffer de transmission
#define BUFFER_SIZE 1

/* private variables ======================================================== */
/*
 * Buffer de transmission
 * Ce buffer contient les informations à transmettre au maître
 */
static uint8_t txbuffer[BUFFER_SIZE];

/* private functions ======================================================== */
/*------------------------------------------------------------------------------
 * Fonction de gestion de la réception
 * Cette fonction est appelé automatiquement lors de la réception d'une trame
 * envoyée par la maître.
 * Ici nous recopions les octets reçus dans le buffer de transmission, puis
 * nous basculons la LED1
 */
static void
vRxCallback (const uint8_t *buffer, uint8_t buffer_length) {

  for (uint8_t i = 0; (i < buffer_length) && (i < BUFFER_SIZE); i++) {

    txbuffer[i] = buffer[i];
  }
  vLedToggle (LED_LED1);
}

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  // Initialisation du module sans activation de l'endormissement
  vUsiSlaveInit (DEVICE_ID, 0, txbuffer, BUFFER_SIZE, vRxCallback);
  sei(); // Le module USI utilise les interruptions !

  for (;;) {

    vUsiSlavePoll();
  }
  return 0;
}
/* ========================================================================== */
