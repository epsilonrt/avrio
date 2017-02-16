/**
 * @file
 * Demo RTC DS1339
 *
 * Ce programme affiche l'heure et la date et permet de mettre à l'heure une
 * RTC
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-rtc.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/rtc.h>
#include <avrio/rtc_ds1339.h>
#include <avrio/term.h>
#include <avrio/tc.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define BAUDRATE  115200
#define PORT      "tty0"
#define DS1339_ADDR (int)DS1339_BASE

/* private variables ======================================================== */
static xRtcTime xSetTime, xGetTime;

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
static void
prvvSetRtc (void) {

  printf_P (PSTR ("\nDate (DDMMYYYY) ? "));
  xSetTime.ucDate = (uint8_t) iTermGetDec (stdin, 2);

  putchar ('/');
  xSetTime.ucMonth = (uint8_t) iTermGetDec (stdin, 2);

  putchar ('/');
  xSetTime.usYear = (uint16_t) iTermGetDec (stdin, 4);

  printf_P (PSTR ("\nJour (1=Dim..7=Sam) ? "));
  xSetTime.ucWeekDay = (uint8_t) iTermGetDec (stdin, 1);

  printf_P (PSTR ("\nHeure (HHMMSS) ? "));
  xSetTime.ucHour = (uint8_t) iTermGetDec (stdin, 2);

  putchar (':');
  xSetTime.ucMinute = (uint8_t) iTermGetDec (stdin, 2);

  putchar (':');
  xSetTime.ucSecond = (uint8_t) iTermGetDec (stdin, 2);
  putchar ('\n');

  vAssert (iRtcSetTime (&xSetTime) == 0);
  vAssert (iRtcGetTime (&xGetTime) == 0);
  vAssert (memcmp (&xSetTime, &xGetTime, sizeof (xRtcTime)) == 0);
}

// -----------------------------------------------------------------------------
static void
prvvPrintRtc (void) {

  vAssert (iRtcGetTime (&xGetTime) == 0);
  vAssert (iRtcPrintDate (&xGetTime) > 0);
  printf_P (PSTR (" - "));
  vAssert (iRtcPrintTime (&xGetTime) > 0);
  putchar ('\n');
}

/* main ===================================================================== */
int
main (void) {
    int c;
  eTwiStatus eError;
  uint8_t ucWaitTime = 10; // Temps d'attente en secondes

  vLedInit ();
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  sei();
  printf_P (PSTR ("\n\nExemple RTC DS1339\n"));

  vTwiInit ();
  eError = eTwiSetSpeed (100);
  vAssert (eError == TWI_SUCCESS);

  vAssert (iRtcInit (DS1339_ADDR) == 0);

  printf_P (PSTR ("Date courante: "));
  prvvPrintRtc();
  printf_P (PSTR ("Modification Date ? (y/n) "));
  while (ucWaitTime--) {
    uint8_t ucCount = 10;

    do {
      c = getchar();
      // Boucle en attente appui d'une touche
      delay_ms (100);
    }
    while ( (ucCount--) && (c == EOF));

    if (c != EOF) {

      if ( (c == 'y') || (c == 'Y')) {

        prvvSetRtc();
      }
      break;
    }
    putchar ('.');
  }

  putchar ('\n');
  
  for (;;) {

    c = getchar();
    if (c != EOF) {

      prvvSetRtc();
    }
    prvvPrintRtc();
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
