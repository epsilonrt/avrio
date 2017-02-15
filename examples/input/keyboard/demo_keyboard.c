/**
 * @file
 * @brief Demo clavier matriciel (keyb)
 *
 * Le module switch permet de gérer un clavier organisé sous forme d'une matrice XY.
 *
 * Ce programme affiche les touches tapées sur le clavier matriciel sur la
 * liaison série :
 *
 * 0
 * 2
 * ...
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-keyboard.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <ctype.h>
#include <avrio/tc.h>
#include <avrio/keyb.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* main ===================================================================== */
int
main (void) {

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard
  puts ("Press key...");

  /* Initialisation Clavier */
  vKeybInit ();

  for (;;) {

    // Test si une touche est appuyée
    if (usKeybHit () != 0) {
      char c;

      // lecture de la touche
      c = cKeybGetChar ();

      if (isprint (c)) {
        // Si la touche est affichable, on l'affiche
        putchar (c);
      }
      else {
        // sinon, on affiche son code hexa
        printf ("0x%02X", c);
      }
    }
  }

  return 0;
}
/* ========================================================================== */
