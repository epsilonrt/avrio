/** 
 * @file
 * Démonstration afficheur LCD
 * 
 * AvrIO prend en charge un grand nombre d'afficheurs LCD de modèle différents
 * (HD44780, PCF2119, ST7032...) et de port différents (PIO, I2C...)
 * Il suffit de le configurer dans le fichier avrio-board-lcd.h
 * Dans cette démonstration, un afficheur LCD HD44780 sur PIO 4 bits est utilisé.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>


/* main ===================================================================== */
int
main (void) {
  int i;

  vLedInit();
  delay_ms (200);
  iLcdInit();
  // Le LCD est défini comme sortie standard (printf, putchar ...)
  stdout = &xLcd;

  for (;;) {

    vLcdClear ();
    printf ("Hello World !\n");

    for (i=9; i >=0; i--) {

      printf ("%d", i);
      delay_ms (500);
    }

    vLcdGotoXY (13, 0);
    printf ("Go!");
    delay_ms (5000);
  }
  return 0;
}
/* ========================================================================== */
