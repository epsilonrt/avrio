#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avrio/led.h>
#include <avrio/eefile.h>
#include <avrio/assert.h>
#include <avrio/serial.h>


/* constants ================================================================ */
#define SERIAL_BAUDRATE 38400
#define SERIAL_IOS (SERIAL_DEFAULT + SERIAL_WR)

/* private variables ======================================================== */
static uint8_t ucBufferEe[EEPROM_SIZE] EEMEM;
static char buf1[32];
static char buf2[32];
static const char message[] = "Hello World #%d\n";

/* main ===================================================================== */
int
main (int argc, char **argv) {
  int r, s;

  vLedInit();
  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_IOS);
  stdout = &xSerialPort;
  stderr = &xSerialPort;

  printf ("** EEFILE Test **\n"
          "EEPROM size: %d bytes\n"
          "Writes lines to EEFile:\n",
          EEPROM_SIZE);
          
  FILE * f = xEeFileOpen (ucBufferEe, sizeof(ucBufferEe), O_WRONLY);
  assert (f);
  
  for (int i = 0; i < 16; i++) {
    
    s = sprintf (buf1, message, i);
    vLedSet (LED_LED1);
    r = fprintf (f, message, i);
    vLedClear (LED_LED1);
    assert (r == s);
    printf("%s", buf1);
  }
  printf ("Close the file...");
  r = iEeFileClose(f);
  assert (r == 0);
  printf("Ok\n\n");
  
  printf("Reads lines to EEFile:\n");
  f = xEeFileOpen (ucBufferEe, sizeof(ucBufferEe), O_RDONLY);
  assert (f);
  
  for (int i = 0; i < 16; i++) {
    
    sprintf (buf1, message, i);
    vLedSet (LED_LED1);
    fgets (buf2, sizeof(buf2), f);
    vLedClear (LED_LED1);
    assert (strcmp (buf1, buf2) == 0);
    printf("%s", buf2);
  }
  
  printf ("Close the file...");
  assert (r == 0);
  printf("Ok\n\n");
  return 0;
}
/* ========================================================================== */
