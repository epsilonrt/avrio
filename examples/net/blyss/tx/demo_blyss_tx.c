/**
 * @file
 * Demo transmission de commandes Blyss (module blyss)
 *
 * Ce programme permet la transmission de commande Blyss à partie d'ordre
 * donnés sur la liaison série (Réalisation d'une télécommande) :
 *
 *   ** Demo Transmission Blyss **
 *   0..8 : Canal
 *     M  : Marche
 *     A  : Arret
 *     B  : Boucle
 *
 *   Canal 4 -> Loop On Off On Off On Off On Off On Off On On
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-blyss.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <stdio.h>
#include <avrio/delay.h>
#include <avrio/blyss.h>
#include <avrio/tc.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define BAUDRATE  115200
#define PORT      "tty0"
// Délai en ms entre basculement en mode boucle
#define LOOP_TIME 5000

/* private variables ======================================================== */
static const uint8_t my_id[] = {
  //0x79, 0x5F, 0x78 // Micro émetteur @SkyWodd
  //0x39, 0x21, 0xA8 // Télécommande @SkyWodd
  //0x0E, 0xCB, 0xE8 // Détecteur IR @skywodd
  0x05, 0x08, 0x24 // Télécommande 8 canaux Ref. 582883
};

/* main ===================================================================== */
int
main (void) {
  int c, count;
  int channel = -1;
  bool state = false; // état ON/OFF
  bool loop = false;  // mode boucle
  bool txreq = false; // demande de transmission
  xBlyssFrame f; // trame

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  sei();

  // Affiche le menu
  printf_P (PSTR ("\n\n** Demo Transmission Blyss **\n"
                  "0..8 : Canal\n"
                  "  M  : Marche\n"
                  "  A  : Arret\n"
                  "  B  : Boucle\n"));

  // Init. du module Blyss
  vBlyssInit ();
  // Init. trame
  vBlyssFrameInit (&f, my_id);

  for (;;) {

    if ( (c = getchar()) != EOF) {
      
      // Touche appuyée
      switch (c) {

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
          // Modification du canal
          channel = c - '0'; // Conversion ASCII -> Binaire
          // Affichage
          printf_P (PSTR ("\nCanal %u -> "), channel);
          // Modification de la trame
          vBlyssFrameSetChannel (&f, channel);
          break;

        case 'A':
        case 'a':
          if (channel != -1) {
            
            // Commande Arrêt demandée
            state = false;
            loop = false;
            txreq = true;
          }
          break;

        case 'M':
        case 'm':
          if (channel != -1) {
            
            // Commande Marche demandée
            state = true;
            txreq = true;
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


    if (txreq) {
      // Transmission demandée

      // On met à jour la trame avec l'état
      vBlyssFrameSetState (&f, state);
      // On transmet la trame
      vBlyssSend (&f, 4);
      // transmission effectuée
      txreq = false;
      printf ("%s ", state ? "On" : "Off");
    }

    if (loop) {
      // mode boucle

      // Test si compteur à zéro et décomptage
      if (count-- == 0) { // si compteur à zéro

        count = LOOP_TIME / 100; // réinitialise le compteur
        state = !state; // bascule l'état On/Off
        txreq = true; // demande la transmission
      }
      delay_ms (100);
    }
  }
  return 0;
}
/* ========================================================================== */
