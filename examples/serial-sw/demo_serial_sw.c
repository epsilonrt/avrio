/**
 * @file
 * Demo liaison série logicielle (module serial_sw)
 *
 * Ce programme affiche un message sur la liaison série puis passe en mode 
 * attente et affiche des points. Un terminal doit connecté sur la liaison
 * série logicielle. \n
 * Il renvoie sur la liaison série tous les caractères reus (qui seront donc
 * affichés sur le terminal connecté au port série). \n
 * Un appui sur ENTER recommence au début.
 *
 * Il est prévu pour une carte Arduino UNO, la broche de réception RX est sur 
 * PD3,INT1 (IO3 sur Arduino UNO), la broche TX est sur PD4 (IO4 sur Arduino UNO).
 * Le fichier avrio-board-serial-sw.h dans le répertoire courant devra être 
 * adapté pour une autre carte.
 */
#include <avrio/serial_sw.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avr/pgmspace.h> // pout printf_P() et PSTR()

/* main ===================================================================== */
int
main (void) {
  int c; // pour le caractère reçu
  bool bWait; // mode attente
  uint16_t usCount;

  vLedInit();
  /*
   * Init. du module
   * La vitesse de la liaison est fixe, définie par la constante 
   * AVRIO_SERIAL_SW_BAUD dans le Makefile.
   * Le format est 8 bits, 1 bit de stop, sans parité. Il est figé.
   */
  vSerialSwInit ();
  stdout = &xSerialSwPort; // le port série est la sortie standard
  stdin = &xSerialSwPort; // le port série est l'entrée standard standard
  sei(); // valide les interruptions

  for (;;) {

    usCount = 0; // raz du compteur pour affichage des points
    bWait = true; // mode attente, on affiche des points

    // Affiche le message
    // printf_P est une version de printf utilisant une chaine de caractères
    // stockée en mémoire FLASH (pour économiser la mémoire RAM !).
    // La chaine de caractères doit être entourée par PSTR()
    printf_P (PSTR ("\n*** Demo Liaison serie ***\n"));
    printf_P (PSTR ("Appuyez sur une touche (ENTER pour quitter)..."));

    do { // boucle d'attente

      c = getchar (); // lecture du prochain caractère sur la liaison série
      if (c != EOF) { // Si caractère reçu

        if (bWait) { // si nous sommes en mode attente

          putchar ('\n'); // on saute la ligne pour afficher le caractère
          bWait = false; // on quitte le mode attente
        }

        putchar (c); // on affiche le caractère reçu
        vLedToggle (LED_LED1); // on bascule la LED1
      }
      else { // pas de caractère reçu (c = EOF -> End Of File)

        if ( (bWait) && ( (usCount++ % 32768) == 0)) {
          
          // si mode attente, on affiche un point toutes les 32768 boucles
          putchar ('.');
        }
      }
    }
    while (c != '\r');// Tant qu'on a pas reçu un ENTER
  }
  return 0;
}

/* ========================================================================== */
