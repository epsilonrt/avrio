/**
 * @file
 * Demo sniffer de liaison Blyss (module blyss)
 *
 * Ce programme affiche les trames Blyss reçues sur la liaison série :
 * 
 * RF frame          : FE5082411671B                                               
 * RF footprint      : FE - Ok                                                     
 * RF global channel : 5                                                           
 * RF adress         : 0824                                                        
 * RF channel        : 4                                                           
 * Light status      : Off                                                         
 * Rolling code      : 67 - Ok                                                     
 * Token             : 1B                                                          
 * Echo 1                                                                          
 * Echo 2              
 *  
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-blyss.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/delay.h>
#include <avrio/blyss.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT     "tty0"

/* main ===================================================================== */
int
main (void) {
  int echo = 0;
  // Trame reçue et trame précédente
  xBlyssFrame f, fprev;
  
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  // Init. des trames
  vBlyssFrameInit (&f, NULL);
  vBlyssFrameInit (&fprev, NULL);
  // Init. du module Blyss
  vBlyssInit ();
  sei(); // le module Blyss utilise les interruptions
  
  printf_P (PSTR ("\n** Demo Reception Blyss **\n"));

  for (;;) {

    if (bBlyssReceive (&f)) {
      
      // Une trame a été reçue
      if (bBlyssFrameMatch (&f, &fprev)) {
        
        // la trame reçue est identique à la précédente, affiche EchoX
        /* Trame echo */
        printf_P (PSTR("Echo %d\n"), ++echo);
      }
      else {
        
        // c'est une nouvelle trame, on l'affiche...
        putchar('\n');
        vBlyssPrintFrame (&f);
        // On copie la trame dans la précédente
        vBlyssFrameCopy (&fprev, &f);
        // et on remet le compteur d'échos à zéro
        echo = 0;
      }
    }
  }
  return 0;
}
/* ========================================================================== */
