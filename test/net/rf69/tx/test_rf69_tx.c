/**
 * @file
 * @brief Test de transmission avec un module RFM69
 * 
 * -----------------------------<<WARNING>>------------------------------------- 
 * Un module RFM69 est alimenté en 3.3V et n'est donc pas compatible
 * avec ARDUINO sans adaptation de niveau... on utilisera plutôt une carte xplbee !
 * -----------------------------------------------------------------------------
 */
#include <avrio/delay.h>
#include <avrio/rf69.h>
#include <avrio/tc.h>
#include <avrio/assert.h>
#include <avr/pgmspace.h>
#include <string.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT     "tty0"

#define DIO0_IRQ  INT0
#define NODE_ID   1
#define NET_ID    1

/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */
/* private functions ======================================================== */
/* internal public functions ================================================ */

/* main ===================================================================== */
int
main (void) {
  int t = 1, ret;

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  sei();

  printf_P (PSTR ("\n\n*** RFM69 TX test ***\n\n"));

  printf_P (PSTR ("Test %d - xRf69New(0,0,%d)>\n"), t++, DIO0_IRQ);
  xRf69 * rf = xRf69New (0, 0, DIO0_IRQ);
  assert (rf);
  printf_P (PSTR ("Success\n\n"));

  printf_P (PSTR ("Test %d - iRf69Open (rf,eRf69Band868Mhz,%d,%d)>\n"), t++,
            NODE_ID, NET_ID);
  ret = iRf69Open (rf, eRf69Band868Mhz, NODE_ID, NET_ID);
  assert (ret == 0);
  printf_P (PSTR ("Success\n\n"));

  // NodeId
  // NetId
  // frequency
  // powerlevel/highpower
  // temperature
  // rssi
  // iRf69CanSend / iRf69Send / iRf69SendWithRetry
  // Ajout iRf69Poll()
  
  for (;;) {

    // Your main code should be here,
    // you can delete the two lines below
    // Votre code principal devrait être ici,
    // Vous pouvrez supprimer les deux lignes ci-dessous
  }
  return 0;
}

/* ========================================================================== */
