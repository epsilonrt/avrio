/*
 * Test module blyss en émission
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/blyss.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define DEFAULT_CHANNEL 4
#define LOOP_TIME 5000
#define TERMINAL_BAUDRATE 115200
#define TERMINAL_PORT     "tty0"

/* private variables ======================================================== */
static const uint8_t my_id[] = {
  //0x79, 0x5F, 0x78 // Micro émetteur @SkyWodd
  //0x39, 0x21, 0xA8 // Télécommande @SkyWodd
  // 0x0E, 0xCB, 0xE8 // Détecteur IR @skywodd
  0x05, 0x08, 0x24 // Télécommande 8 canaux Ref. 582883
};

/* main ===================================================================== */
int
main (void) {
  int c, count;
  uint8_t channel = DEFAULT_CHANNEL;
  bool state = false;
  bool loop = false;
  bool txreq = false;
  xBlyssFrame f;
  xSerialIos xTermIos = SERIAL_SETTINGS (TERMINAL_BAUDRATE);

  vLedInit();
  FILE * tc = xFileOpen (TERMINAL_PORT, O_RDWR | O_NONBLOCK, &xTermIos);
  stdout = tc;
  stderr = tc;
  stdin = tc;
  sei();
  printf_P (PSTR ("** Blyss Transmitter Test **\n"
                  "Channel: %u\n"
                  "Press 0 for OFF, 1 for ON, 2 for Loop...\n"), channel);

  vBlyssInit ();
  vBlyssFrameInit (&f, my_id);
  vBlyssFrameSetChannel (&f, channel);

  for (;;) {

    if ( (c = getchar()) != EOF) {

      switch (c) {
        case '0':
          state = false;
          loop = false;
          txreq = true;
          printf_P (PSTR("\nLight OFF\n"));
          break;
        case '1':
          state = true;
          txreq = true;
          loop = false;
          printf_P (PSTR("\nLight ON\n"));
          break;
        case '2':
          loop = true;
          count = 0;
          printf_P (PSTR("\nLoop ON/OFF\n"));
          break;
        default:
          break;
      }
    }

    if (txreq) {

      vBlyssFrameSetState (&f, state);
      vBlyssSend (&f, 4);
      vBlyssPrintFrame (&f);
      txreq = false;
    }

    if (loop) {
      
      if (count-- == 0) {

        state = !state;
        txreq = true;
        count = LOOP_TIME / 100;
      }
      
      delay_ms (100);
    }
  }
  return 0;
}
/* ========================================================================== */
