/**
 * @file
 * Demo Scan bus1-wire
 *
 * Ce programme recherche les circuits sur le bus 1-wire.
 * Si \c SERIAL_LOG_ENABLE est défini, les identifiants des circuits sont 
 * affichés sur la liaison série, dans ce cas, l'implémentation choisie pour 
 * la gestion du bus 1-wire ne peut pas utiliser l'UART (donc 
 * \c OW_DRIVER_POLLED_PIO est obligatoire). \n
 * Voilà ce que cela donne si 2 circuits DS2438 sont détectés:
 * @code
 
    2 devices found
    #,id
    0,26A2FA17020000B9
    1,26BEF517020000F0
 * @endcode
 *
 * Il est prévu pour une carte mto-template, le fichier avrio-board-ow.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/ow.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/assert.h>
#include <avrio/tc.h>
#include <avrio/osc.h>

/* constants ================================================================ */
#define SERIAL_LOG_ENABLE //< Valide l'affichage des valeurs sur liaison série
#define BAUDRATE 115200
#define PORT "tty0" // UART0
#define MAX_DEVICES 2 // Nombre max de circuits sur le bus 1-wire

#if defined(SERIAL_LOG_ENABLE) && defined(OW_DRIVER_POLLED_UART)
// On ne peut pas utiliser l'UART pour le 1-wire et la liaison série !
#error Unable to use UART for serial bus and 1-wire bus !
#endif

/* private variables ======================================================== */
// Tableau des circuits trouvés sur le bus 1-wire
static xOwDevice devices[MAX_DEVICES];

#ifdef SERIAL_LOG_ENABLE
#define PDEBUG(fmt,...) printf_P(PSTR(fmt),##__VA_ARGS__)
// Configuration du port série
xSerialIos settings = {
  .baud = BAUDRATE, // vitesse en bauds
  .dbits = 8, // 8 bits de données
  .parity = 'N', // pas de parité
  .sbits = 1, // 1 bit de stop
  .flow = SERIAL_FLOW_NONE, // pas de contrôle de flux
  .eol = SERIAL_CRLF // fin de ligne en CR+LF
};
#else
// PDEBUG ne fait rien si pas de liaison série
#define PDEBUG(fmt,...)
#endif

/* main ===================================================================== */
int
main (void) {
  int n;

#ifdef SERIAL_LOG_ENABLE
  // calibration oscillateur nécessaire si SERIAL_LOG_ENABLE et BAUDRATE > 19200
  iOscCalibrateFromEE (OSCCAL_EEADDR);
  // Ouverture du port série en entrée-sortie non bloquant
  FILE * serial_port = xFileOpen (PORT, O_WR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
#endif

  vLedInit(); // LED1 bascule à chaque lecture des données
  vOwInit(); // Init du bus 1-wire
  sei(); // valide les interruptions


  for (;;) {

    // Recherche des circuits sur le bus 1-wire
    do {
      n = iOwSearchDevices (devices, MAX_DEVICES);
    }
    while (n < 0);
    // n contient le nombre de circuits sur le bus 1-wire
    PDEBUG ("\n%d devices found\n", n);

    PDEBUG ("#,id\n");
    for (uint8_t i = 0; i < n; i++) {
      xOwDevice * d = &devices[i];

      // puis on affiche son identifiant 64bits
      PDEBUG ("%d,", i);
      for (int8_t j = 0; j < 8; j++) {

        PDEBUG ("%02X", d->id[j]);
      }
      putchar ('\n');
    }
    
    // On bascule LED1 à chaque boucle
    vLedToggle (LED_LED1);
    delay_ms (5000);
  }


  return 0;
}
/* ========================================================================== */
