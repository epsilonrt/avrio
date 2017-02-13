#include <avrio/melody.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avr/pgmspace.h>

/* private variables ======================================================== */
/*
 * Ce tableau est généré à partir d'un fichier MIDI et du logiciel
 * BrickOs Music Studio (format BrickOS 2.0)
 */
static const xNote pxMyMelody[] PROGMEM = {
  {29, 12}, {PITCH_PAUSE, 4}, {29, 12}, {PITCH_PAUSE, 4}, {29, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 25},
  {PITCH_PAUSE, 8}, {36, 12}, {PITCH_PAUSE, 4}, {36, 12}, {PITCH_PAUSE, 4},
  {36, 12}, {PITCH_PAUSE, 4}, {37, 8}, {PITCH_PAUSE, 4}, {32, 4}, {28, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 25},
  {PITCH_PAUSE, 8}, {41, 12}, {PITCH_PAUSE, 4}, {29, 8}, {PITCH_PAUSE, 4},
  {29, 4}, {41, 8}, {PITCH_PAUSE, 8}, {40, 8}, {PITCH_PAUSE, 4}, {39, 4},
  {38, 4}, {37, 4}, {38, 8}, {PITCH_PAUSE, 8}, {30, 4}, {PITCH_PAUSE, 4},
  {35, 8}, {PITCH_PAUSE, 8}, {34, 8}, {PITCH_PAUSE, 4}, {33, 4}, {32, 4},
  {31, 4}, {32, 8}, {PITCH_PAUSE, 8}, {25, 4}, {PITCH_PAUSE, 4}, {28, 8},
  {PITCH_PAUSE, 8}, {25, 8}, {PITCH_PAUSE, 4}, {28, 4}, {32, 12},
  {PITCH_PAUSE, 4}, {29, 8}, {PITCH_PAUSE, 4}, {32, 4}, {36, 25},
  {PITCH_PAUSE, 8}, {41, 12}, {PITCH_PAUSE, 4}, {29, 8}, {PITCH_PAUSE, 4},
  {29, 4}, {41, 8}, {PITCH_PAUSE, 8}, {40, 8}, {PITCH_PAUSE, 4}, {39, 4},
  {38, 4}, {37, 4}, {38, 8}, {PITCH_PAUSE, 8}, {30, 4}, {PITCH_PAUSE, 4},
  {35, 8}, {PITCH_PAUSE, 8}, {34, 8}, {PITCH_PAUSE, 4}, {33, 4}, {32, 4},
  {31, 4}, {32, 8}, {PITCH_PAUSE, 8}, {25, 4}, {PITCH_PAUSE, 4}, {28, 8},
  {PITCH_PAUSE, 8}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 25}, {PITCH_END,
                                                                   0}
};

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vMelodyInit (30, 0);

  /* On dispose de 4 sons systèmes
   * MELODY_BEEP:    bip simple
   * MELODY_WELCOME: mélodie de bienvenue
   * MELODY_WARNING: mélodie attirant l'attention
   * MELODY_ERROR:   mélodie d'erreur
   */
  vMelodyPlaySystem (MELODY_WELCOME);
  delay_ms (1000);

  for (;;) {

    vMelodyPlayFlash (pxMyMelody);

    while (xMelodyIsPlay()) {

      // Pendant la diffusion de la mélodie, on peut faire autre chose...
      vLedToggle (LED_LED1);
      delay_ms (100);
    }
  }
  return 0;
}
/* ========================================================================== */
