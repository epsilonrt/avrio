/**
 * @file
 * @brief Demo micro-switch (irq, sleep, watchdog)
 *
 * Le module switch permet de gérer les micro-interrupteurs.
 *
 * Ce programme affiche l'état des interrupteurs en hexadécimal sur la liaison 
 * série :
 * 
 * 0
 * 2
 * ...
 * 
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-switch.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <avrio/switch.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* main ===================================================================== */
int
main (void) {
  xSwitch  xPrev = 0;
  xSwitch  xCurrent;

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard
  
  vSwitchInit ();

  for (;;) {

    xCurrent = xSwitchGet();
    if (xCurrent != xPrev) {

      xPrev = xCurrent;
      printf ("%X\n", xCurrent);
    }
  }
  return 0;
}
/* ========================================================================== */
