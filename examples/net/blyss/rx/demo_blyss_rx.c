/**
 * @file
 * @brief Demo réception Blyss (module blyss)
 *
 * Ce programme allume et éteint une led à distance par liaison Blyss T0
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-blyss.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/blyss.h>

/* constants ================================================================ */
// Canal Blyss (bouton télécommande)
#define MYCHAN 4

/* main ===================================================================== */
int
main (void) {
  // Trame reçue et trame précédente
  xBlyssFrame f, fprev;

  vLedInit();

  // Init. des trames
  vBlyssFrameInit (&f, NULL);
  vBlyssFrameInit (&fprev, NULL);
  // Init. du module Blyss
  vBlyssInit ();
  sei(); // le module Blyss utilise les interruptions

  for (;;) {

    if (bBlyssReceive (&f)) {

      // Une trame a été reçue
      if (!bBlyssFrameMatch (&f, &fprev)) { // c'est une nouvelle trame...

        uint8_t c = ucBlyssFrameChannel (&f);

        if ( (c == MYCHAN) || (c == BLYSS_BROADCAST)) {
          
          // la trame correspond à notre canal (ou à l'appel général)
          if (bBlyssFrameState (&f)) {
            
            vLedSet (LED_LED1);
          }
          else {

            vLedClear (LED_LED1);
          }
        }
        // On copie la trame dans la précédente
        vBlyssFrameCopy (&fprev, &f);
      }
    }
  }
  return 0;
}
/* ========================================================================== */
