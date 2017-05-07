/**
 * @file
 * Demo DS1820
 *
 * Ce programme recherche les circuits DS1820 de mesure de température sur
 * le bus 1-wire puis effectue la lecture des données brutes dans les registres
 * des circuits trouvés. \n
 * Si \c SERIAL_LOG_ENABLE est défini, les valeurs sont affichées sur la liaison
 * série, dans ce cas, l'implémentation choisie pour la gestion du bus 1-wire ne
 * peut pas utiliser l'UART (donc \c OW_DRIVER_POLLED_PIO est obligatoire). \n
 * Voilà ce que cela donne pour 1 DS18B20:
 * @code
    #,id
    0,28A2D984000002EA
    1 DS1820 devices found

    r0,t0
    1360,85.0
 * @endcode
 *
 * Il est prévu pour une carte mto-template, le fichier avrio-board-ow.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/ow-ds1820.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/assert.h>
#include <avrio/tc.h>
#include <avrio/osc.h>

/* constants ================================================================ */
#define SERIAL_LOG_ENABLE //< Valide l'affichage des valeurs sur liaison série

#define BAUDRATE 115200
#define PORT "tty0" // UART0
#define MAX_DEVICES 4 // Nombre max de circuits sur le bus 1-wire
#define FAMILY_ID DS18B20_FAMILY_ID // DS18B20
#define FAMILY_LSB DS18B20_LSB

#if defined(SERIAL_LOG_ENABLE) && defined(OW_DRIVER_POLLED_UART)
// On ne peut pas utiliser l'UART pour le 1-wire et la liaison série !
#error Unable to use UART for serial bus and 1-wire bus !
#endif

/* private variables ======================================================== */
// Tableau des circuits trouvés sur le bus 1-wire
static xOwDevice devices[MAX_DEVICES];
// Tableau des DS1820 trouvés sur le bus 1-wire
static xOwDevice * ds1820[MAX_DEVICES];

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
  int ret, n, nd = 0;

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

  // n contient le nombre de circuits sur le bus 1-wire
  PDEBUG ("#,id\n");

  do {
    // Recherche des circuits sur le bus 1-wire
    do {
      n = iOwSearchDevices (devices, MAX_DEVICES);
    }
    while (n <= 0);


    for (uint8_t i = 0; i < n; i++) {
      xOwDevice * d = &devices[i];

      // On recherche les circuits DS1820 dans la liste des circuits sur le bus 1-wire
      if (bOwIsPartOfFamily (d, FAMILY_ID)) {

        // c'est un DS1820 !
        ds1820[nd] = d; // on l'ajoute dans notre tableau

        // puis on affiche son identifiant 64bits
        PDEBUG ("%d,", nd);
        for (int8_t j = 0; j < 8; j++) {

          PDEBUG ("%02X", d->id[j]);
        }
        putchar ('\n');
        nd++;
      }
    }
  }
  while (nd == 0);

  PDEBUG ("%d DS1820 devices found\n\n", nd);

  // Affichage des entêtes de colonnes
  for (uint8_t i = 0; i < nd; i++) {

    PDEBUG ("r%d,t%d", i, i);
    putchar ( (i < (nd - 1)) ? ',' : '\n');
  }

  for (;;) {

    for (uint8_t i = 0; i < nd; i++) {

      // Lecture des données instantanées brutes
      ret = iDs1820Read (ds1820[i]);

      // Affichage
      if (ret != DS1820_ERROR) {
        
        PDEBUG ("%d,%.1f", ret, (double) ret * FAMILY_LSB);
      }
      else {
        
        PDEBUG ("Err");
      }
      putchar ( (i < (nd - 1)) ? ',' : '\n');
    }

    // On bascule LED1 à chaque boucle
    vLedToggle (LED_LED1);
    delay_ms (5000);
  }

  return 0;
}
/* ========================================================================== */
