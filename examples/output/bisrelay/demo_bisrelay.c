/**
 * @file
 * Demo relais bistable (module bisrelay)
 *
 * Ce programme permet la commande de relais bistable à partir d'ordre
 * donnés sur la liaison série :
 *
 *   ** Demo Relais bistable **
 *   1..3 : Relais
 *     M  : Marche
 *     A  : Arret
 *     B  : Boucle
 *
 *   Relais 2 -> Loop On Off On Off On Off On Off On Off On On
 *
 * Il est prévu pour une carte Arduino UNO équipé de 3 relais connectés de la 
 * façon suivantes :
 * 
 * +--------+-----+-----+
 * | Relais | Set | Clr |
 * +--------+-----+-----+
 * |   1    | IO2 | IO3 |
 * |   2    | IO4 | IO5 |
 * |   3    | IO6 | IO7 |
 * +--------+-----+-----+
 * 
 * le fichier avrio-board-bisrelay.h dans le répertoire courant devra être 
 * adapté pour une autre configuration.
 */
#include <avrio/delay.h>
#include <avrio/bisrelay.h>
#include <avrio/tc.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define BAUDRATE  115200
#define PORT      "tty0"
// Délai en ms entre basculement en mode boucle
#define LOOP_TIME 5000

/* main ===================================================================== */
int
main (void) {
  int c, count;
  int channel = -1;
  bool state = false; // état ON/OFF
  bool loop = false;  // mode boucle
  bool action = false; // demande modification relais

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  sei();

  // Affiche le menu
  printf_P (PSTR ("\n\n** Demo Relais bistable **\n"
                  "1..%d : Relais\n"
                  "  M  : Marche\n"
                  "  A  : Arret\n"
                  "  B  : Boucle\n"), BISRELAY_QUANTITY);

  vBisRelayInit ();

  for (;;) {

    if ( (c = getchar()) != EOF) {

      // Touche appuyée
      switch (c) {

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
          if (c - '0' <= BISRELAY_QUANTITY) {
            // Modification du relais
            channel = c - '0'; // Conversion ASCII -> Binaire
            // Affichage
            printf_P (PSTR ("\nRelais %u -> "), channel);
          }
          break;

        case 'A':
        case 'a':
          if (channel != -1) {

            // Commande Arrêt demandée
            state = false;
            loop = false;
            action = true;
          }
          break;

        case 'M':
        case 'm':
          if (channel != -1) {

            // Commande Marche demandée
            state = true;
            action = true;
            loop = false;
          }
          break;

        case 'B':
        case 'b':
          if (channel != -1) {

            // Commande Boucle demandée
            loop = true;
            count = 0;
            printf_P (PSTR ("Loop "));
          }
          break;

        default:
          break;
      }
    }


    if (action) {
      // modification demandée

      if (state) {
        
        vBisRelaySet (_BV (channel - 1));
      }
      else {
        
        vBisRelayClear (_BV (channel - 1));
      }

      // basculement effectuée
      action = false;
      printf ("%s ", state ? "On" : "Off");
    }

    if (loop) {
      // mode boucle

      // Test si compteur à zéro et décomptage
      if (count-- == 0) { // si compteur à zéro

        count = LOOP_TIME / 100; // réinitialise le compteur
        state = !state; // bascule l'état On/Off
        action = true; // demande la transmission
      }
      delay_ms (100);
    }
  }
  return 0;
}
/* ========================================================================== */
