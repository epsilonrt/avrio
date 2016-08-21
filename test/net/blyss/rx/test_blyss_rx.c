/*
 * Test module blyss en réception
 * Affiche les trames reçues sur la liaison série :
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
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/delay.h>
#include <avrio/blyss.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define TERMINAL_BAUDRATE 115200
#define TERMINAL_PORT     "tty0"

/* main ===================================================================== */
int
main (void) {
  int echo = 0;
  xBlyssFrame f, fprev;
  xSerialIos xTermIos = SERIAL_SETTINGS (TERMINAL_BAUDRATE);

  DDRB |= _BV (2);
  FILE * tc = xFileOpen (TERMINAL_PORT, O_WRONLY, &xTermIos);
  stdout = tc;
  stderr = tc;

  vBlyssFrameInit (&f, NULL);
  vBlyssFrameInit (&fprev, NULL);
  vBlyssInit ();
  sei();
  printf_P (PSTR ("** Blyss Receiver Test **\n"));

  for (;;) {

    if (bBlyssReceive (&f)) {

      if (bBlyssFrameMatch (&f, &fprev)) {
        
        /* Trame echo */
        printf_P (PSTR("Echo %d\n"), ++echo);
      }
      else {
        
        /* Nouvelle trame */
        putchar('\n');
        vBlyssPrintFrame (&f);
        echo = 0;
        vBlyssFrameCopy (&fprev, &f);
      }
    }
  }
  return 0;
}
/* ========================================================================== */
