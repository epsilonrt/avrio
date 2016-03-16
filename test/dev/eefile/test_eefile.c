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
static unsigned int test_count;

/* main ===================================================================== */
int
main (int argc, char **argv) {
  int r, s;
  FILE * f;
  uint32_t ulMagic = 0x12345678, ulWord;

  vLedInit();
  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_IOS);
  stdout = &xSerialPort;
  stderr = &xSerialPort;
  sei();

  printf_P (PSTR ("\n\n** EEFILE Test **\n"
                  "EEPROM size: %d bytes\n\n"
                  "Test %d: Erase previous file > "), EEPROM_SIZE, ++test_count);

  eeprom_write_dword ( (uint32_t *) ucBufferEe, ulMagic); // destroy previous file
  ulWord = eeprom_read_dword ( (uint32_t *) ucBufferEe);
  assert (ulWord == ulMagic);
  printf_P (PSTR ("Success\n\n"));


  printf_P (PSTR ("Test %d: Checks as opening a nonexistent file in read-only returns NULL > "), ++test_count);
  f = xEeFileOpen (ucBufferEe, sizeof (ucBufferEe), O_RDONLY);
  assert (f == NULL);
  printf_P (PSTR ("Success\n\n"));

  for (int n = 0; n < 2; n++) {
    printf_P (PSTR ("Test %d: Open new file for write > "), ++test_count);
    f = xEeFileOpen (ucBufferEe, sizeof (ucBufferEe), O_WRONLY);
    assert (f);
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d: Writing to file...\n"), ++test_count);
    for (int i = 0; i < 16; i++) {

      s = sprintf (buf1, message, i);
      vLedSet (LED_LED1);
      r = fprintf (f, message, i);
      vLedClear (LED_LED1);
      assert (r == s);
      printf_P (PSTR ("%s"), buf1);
    }
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d: Close the file... "), ++test_count);
    r = iEeFileClose (f);
    assert (r == 0);
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d: Open existing file for read > "), ++test_count);
    f = xEeFileOpen (ucBufferEe, sizeof (ucBufferEe), O_RDONLY);
    assert (f);
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d: reading file...\n"), ++test_count);
    for (int i = 0; i < 16; i++) {

      sprintf (buf1, message, i);
      vLedSet (LED_LED1);
      fgets (buf2, sizeof (buf2), f);
      vLedClear (LED_LED1);
      assert (strcmp (buf1, buf2) == 0);
      printf_P (PSTR ("%s"), buf2);
    }
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d: Close the file... "), ++test_count);
    assert (r == 0);
    printf_P (PSTR ("Success\n\n"));

  }

  for (;;); // Stop
  return 0;
}
/* ========================================================================== */
