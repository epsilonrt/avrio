#include <avrio/term.h>
#include <avrio/serial.h>

// Vitesse de transmission de la liaison série
#define TEST_BAUDRATE 115200

/* main ===================================================================== */
int
main (void) {
  int iWord; char cByte;
  
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  stdin  = &xSerialPort;

  for (;;) {

    fputs_P(PSTR("\nHex Word ? 0x"), stdout);
    iWord = iTermGetHex (stdin, 0);
    fputs_P(PSTR("\nWord =0x"), stdout);
    vTermPutHexWord (iWord, stdout); 
   
    fputs_P(PSTR("\nHex Byte ? 0x"), stdout);
    cByte = iTermGetHex (stdin, 2);
    fputs_P(PSTR("\nByte =0x"), stdout);
    vTermPutHexByte (cByte, stdout);    
  }
  return 0;
}
/* ========================================================================== */
