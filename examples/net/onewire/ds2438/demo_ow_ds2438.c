/**
 * @file
 * Demo DS2438
 *
 * Ce programme recherche les circuits DS2438 de surveillance batterie sur
 * le bus 1-wire puis effectue la lecture des données brutes dans les registres
 * des circuits trouvés. \n
 * Si \c SERIAL_LOG_ENABLE est défini, les valeurs sont affichées sur la liaison
 * série, dans ce cas, l'implémentation choisie pour la gestion du bus 1-wire ne
 * peut pas utiliser l'UART (donc \c OW_DRIVER_POLLED_PIO est obligatoire). \n
 * Si \c FLOAT_MES_ENABLE est défini, la conversion des valeurs brutes en valeurs
 * réelles est effectuée (et leur affichage si \c SERIAL_LOG_ENABLE défini) en
 * utilisant RSENS comme résistance de mesure de courant. \n
 * Voilà ce que cela donne pour 2 DS2438 sans convertion en valeurs réelles:
 * @code
    #,id
    0,26A2FA17020000B9
    1,26BEF517020000F0
    2 DS2438 devices found

    v0,i0,t0,f0,c0,d0,v1,i1,t1,f1,c1,d1
    420,-2,5496,0,1,15,522,-5,5600,0,0,138
    420,-1,5480,0,1,15,521,-6,5584,0,0,138
 * @endcode
 *
 * Il est prévu pour une carte mto-template, le fichier avrio-board-ow.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/ow-ds2438.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/assert.h>
#include <avrio/tc.h>
#include <avrio/osc.h>

/* constants ================================================================ */
#define SERIAL_LOG_ENABLE //< Valide l'affichage des valeurs sur liaison série
// La validation de SERIAL_LOG_ENABLE et FLOAT_MES_ENABLE est impossible sur tiny
// (flash memory overflow)
//#define FLOAT_MES_ENABLE // Valide la convertion valeurs brutes en valeurs réelles

#define BAUDRATE 115200
#define PORT "tty0" // UART0
#define MAX_DEVICES 2 // Nombre max de DS2438 sur le bus 1-wire
#define RSENS 25E-3 // Valeur de la résistance de mesure de courant

#if defined(SERIAL_LOG_ENABLE) && defined(OW_DRIVER_POLLED_UART)
// On ne peut pas utiliser l'UART pour le 1-wire et la liaison série !
#error Unable to use UART for serial bus and 1-wire bus !
#endif

/* private variables ======================================================== */
// Tableau des circuits trouvés sur le bus 1-wire
static xOwDevice devices[MAX_DEVICES];
// Tableau des DS2438 trouvés sur le bus 1-wire
static xOwDevice * ds2438[MAX_DEVICES];
// Données instantanées brutes
static xDs2438DataRaw raw;
// Capacités brutes
static xDs2438CapacityRaw craw;

#ifdef FLOAT_MES_ENABLE
// Données instantanées réelles
static xDs2438DataMes mes;
// Capacités réelles
static xDs2438CapacityMes cmes;
#endif

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

  // Recherche des circuits sur le bus 1-wire
  do {
    n = iOwSearchDevices (devices, MAX_DEVICES);
  }
  while (n < 0);

  // n contient le nombre de circuits sur le bus 1-wire
  PDEBUG ("#,id\n");

  for (uint8_t i = 0; i < n; i++) {
    xOwDevice * d = &devices[i];

    // On recherche les circuits DS2438 dans la liste des circuits sur le bus 1-wire
    if (bOwIsPartOfFamily (d, DS2438_FAMILY_ID)) {

      // c'est un DS2438 !
      ds2438[nd] = d; // on l'ajoute dans notre tableau
      // on le configure...
      ret = iDs2438Configure (d, DS2438_DEFAULT_CONFIG);
      assert (ret == 0);

      // puis on affiche son identifiant 64bits
      PDEBUG ("%d,", nd);
      for (int8_t j = 0; j < 8; j++) {

        PDEBUG ("%02X", d->id[j]);
      }
      putchar ('\n');
      nd++;
    }
  }
  PDEBUG ("%d DS2438 devices found\n\n", nd);

  // Affichage des entêtes de colonnes
  for (uint8_t i = 0; i < nd; i++) {

    PDEBUG ("v%d,i%d,t%d,f%d,c%d,d%d", i, i, i, i, i, i);
    putchar ( (i < (nd - 1)) ? ',' : '\n');
  }
  
  for (;;) {

    for (uint8_t i = 0; i < nd; i++) {

      // On démarre toutes les convertions
      ret = iDs2438StartConvert (ds2438[i], eDs2438ConvAll);
      assert (ret == 0);

      // Lecture des données instantanées brutes
      ret = iDs2438DataGetRaw (ds2438[i], &raw);
      assert (ret == 0);

      // Lecture des capacités brutes
      ret = iDs2438CapacityGetRaw (ds2438[i], &craw);
      assert (ret == 0);

#ifdef FLOAT_MES_ENABLE
      // convertion en valeurs réelles puis affichage
      vDs2438CapacityRawToMes (&cmes, RSENS,  &craw);
      vDs2438DataRawToMes (&mes, RSENS,  &raw);
      PDEBUG ("%g,%g,%g,%g,%g,%g", mes.voltage, mes.current,
              mes.temp, cmes.remaining, cmes.charging, cmes.discharging);
#else
      // Affichage
      PDEBUG ("%d,%d,%d,%d,%d,%d", raw.voltage, raw.current,
              raw.temp, craw.ica, craw.cca, craw.dca);
#endif
      putchar ( (i < (nd - 1)) ? ',' : '\n');
    }
    
    // On bascule LED1 à chaque boucle
    vLedToggle (LED_LED1);
    delay_ms (5000);
  }

  return 0;
}
/* ========================================================================== */
