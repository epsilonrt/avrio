/*
 * Test Afsk en transmission
 *
 * Envoie une trame de façon périodique.
 *
 */
#include <string.h>
#include <avr/interrupt.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/crc.h>
#include <avrio/afsk.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Période d'envoi des trames en ms
#define TX_PERIOD_MS  2000

#define TEST_BASE 1
#define TEST_LONG 2
#define TEST_APRS 3

// Il faut choisir le test à effectuer avant de compiler...
#define TEST TEST_APRS

#define LONG_MSG "ABCDEFGHIJKL"

#if TEST == TEST_APRS
/* ========================================================================== */
#define AX25_CALL_LEN     6
#define AX25_CTRL_UI      0x03
#define AX25_PID_NOLAYER3 0xF0
#define AX25_CRC_CORRECT  0xF0B8

// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Station destination de la trame envoyée
#define APRS_DST "TLM100"
// Station source de la trame envoyée
#define APRS_SRC "NOCALL"
// Information APRS envoyé
#define APRS_MSG ">HELLO %05u"

/* private variables ======================================================== */
static uint16_t usCrc;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (!test) {

    for (;;) {

      vLedSet (LED_RX);
      delay_ms (25);
      vLedClear (LED_RX);
      delay_ms (75);
    }
  }
}

// -----------------------------------------------------------------------------
static void
vPutChar(uint8_t c) {

  if (c == HDLC_FLAG || c == HDLC_RESET || c == HDLC_ESC) {

    // Ajout d'un caractère d'échappement si le code correspond à un flag HDLC
    fputc (HDLC_ESC, &xAfskPort);
  }
  usCrc = usCrcCcittUpdate(c, usCrc);
  fputc(c, &xAfskPort);
}

// -----------------------------------------------------------------------------
static void
vSendCall (const char *call, bool last) {
  unsigned len = MIN(AX25_CALL_LEN, strlen(call));
  uint8_t c;

  for (unsigned i = 0; i < len; i++) {

    c = call[i];
    vPutChar(c << 1);
  }

  /* Fill with spaces the rest of the CALL if it's shorter */
  if (len < AX25_CALL_LEN) {

    for (unsigned i = 0; i < AX25_CALL_LEN - len; i++) {

      vPutChar(' ' << 1);
    }
  }
  /* The bit7 "has-been-repeated" flag is not implemented here */
  /* Bits6:5 should be set to 1 for all SSIDs (0x60) */
  /* The bit0 of last call SSID should be set to 1 */
  uint8_t ssid = 0x60 | (last ? 0x01 : 0);
  vPutChar(ssid);
}

// -----------------------------------------------------------------------------
void
vSendFrame (const void *_buf, size_t len) {
  const uint8_t *buf = (const uint8_t *)_buf;

  usCrc = CRC_CCITT_INIT_VAL;

  fputc(HDLC_FLAG, &xAfskPort);
  vSendCall (APRS_DST, false);
  vSendCall (APRS_SRC, true);
  vPutChar(AX25_CTRL_UI);
  vPutChar(AX25_PID_NOLAYER3);

  while (len--)
    vPutChar(*buf++);

  /*
   * According to AX25 protocol,
   * CRC is sent in reverse order!
   */
  uint8_t crcl = (usCrc & 0xff) ^ 0xff;
  uint8_t crch = (usCrc >> 8) ^ 0xff;
  vPutChar(crcl);
  vPutChar(crch);

  vAssert(usCrc == AX25_CRC_CORRECT);
  fputc (HDLC_FLAG, &xAfskPort);
}
#endif

/* internal public functions ================================================ */
int
main(void) {

  vLedInit();
  vAfskInit (AFSK_MODE_NOBLOCK);
  sei();

  for (;;) {

#if TEST == TEST_BASE
    // Test 1: Test de base, envoi de 0x55 ('U')
    // Il faut éventuellement modifier CONFIG_AFSK_PREAMBLE_LEN et/ou
    // CONFIG_AFSK_TRAILER_LEN dans board-afsk.h
    fputc ('U', &xAfskPort);
#elif TEST == TEST_LONG
    // Test 2: Test de base, envoi de plusieurs octets
    // Il faut éventuellement modifier CONFIG_AFSK_PREAMBLE_LEN et/ou
    // CONFIG_AFSK_TRAILER_LEN dans board-afsk.h
    fputs (LONG_MSG, &xAfskPort);
#elif TEST == TEST_APRS
    // Test 3: Envoi d'une trame APRS
    static uint16_t usCount;
    char msg[15];
    // Numérotation de la trame afin de suivre la perte éventuelle à la
    // réception
    snprintf (msg, sizeof(msg), APRS_MSG, usCount++);
    vSendFrame (msg, strlen(msg));
#else
#error "TEST not defined"
#endif

    vLedToggle (LED_RX);
    delay_ms (TX_PERIOD_MS);
  }
  return 0;
}
/* ========================================================================== */
