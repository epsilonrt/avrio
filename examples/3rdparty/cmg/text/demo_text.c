/**
 * CMG Text Demo
 * @author Christian Merkle. Copyright © 2oo6 - 2oo8 All rights reserved.
 */
#include "CMG/CMG.h"
#include <avrio/button.h>
#include <avrio/serial.h>
#include <avrio/delay.h>
#include <avrio/term.h>
#include <avr/pgmspace.h>
#include <stdio.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 115200

/* public variables ========================================================= */
// If these fonts files do not exist, you must create them in the
// GMGFonts directory. You can do this using  GMGFontConverter !
extern cmg_FontRawData    g_Font_Tahoma_7[];
cmg_Font g_My_Font;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int
iMenu (void) {
  int iChoice;

  do {
    fputs_P(
      PSTR( "\n\n*** Menu ***\n"
            "1-\tTest Echo Serial\n"
            "2-\tTest Orientation\n"
            "3-\tSet Contrast\n"
            "4-\tSet Backlight\n"
            "Choice ? " ), stderr);
    iChoice = iTermGetDec (stdin, 1);
  } while (iChoice > 4);
  return iChoice;
}

// -----------------------------------------------------------------------------
void
vTestOrientation (void) {
  cmg_mu8 ucOrientation;

  for ( ucOrientation = DISPLAYORIENTATION_NORMAL;
        ucOrientation <= (  DISPLAYORIENTATION_XYSWAP  +
                            DISPLAYORIENTATION_XMIRROR +
                            DISPLAYORIENTATION_YMIRROR);
        ucOrientation++) {

    CMG_SetDisplayOrientation (ucOrientation);
    CMG_TEXT_Clear();
    printf_P (PSTR("FACE BOOK ! %d"), ucOrientation);
    while (xButGet(BUTTON_BUTTON1) == 0)
      ;
    delay_ms (150);
  }
}

// -----------------------------------------------------------------------------
void
vTestEchoSerial (void) {
  char cChar;

  CMG_TEXT_Clear();
  fputs_P (PSTR("\n\nTapez un texte à afficher sur le LCD\n"), stderr);
  do {
    cChar = getchar();
    putchar(cChar);
  } while (cChar != 0x1B);
  fputc('M',stderr);
}

// -----------------------------------------------------------------------------
void
vSetContrast (void) {
  int iConstrast;

  do {
    fputs_P(PSTR("\nContrast (0-255) ? "), stderr);
    iConstrast = iTermGetDec (stdin, 3);
  } while ((iConstrast < 0) || (iConstrast > 255));
  CMG_SetContrast (iConstrast);
}

// -----------------------------------------------------------------------------
void
vSetBacklight (void) {

}

// -----------------------------------------------------------------------------
int
main (void) {
  cmg_Result  iResult;
  int iChoice;

  vButInit();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stderr = &xSerialPort;
  stdin  = &xSerialPort;
  stdout = &g_TEXT_Stream;
  fputs_P (PSTR("\n\nTest unitaire LCD CMG\n"), stderr);


  iResult = CMG_TEXT_DrawAndTextInit();
  if (iResult != CMG_OK) {

    fprintf_P (stderr, PSTR(" *** Erreur CMG Init: 0x%02X ***\n\n"), iResult);
    return -1;
  }

  CMG_SetContrast (230);
  CMG_TEXT_Font_Create (&g_My_Font, g_Font_Tahoma_7, 1);
  CMG_TEXT_Font_Set (&g_My_Font);
  CMG_SetDisplayOrientation (DISPLAYORIENTATION_ROTATE180);

  for (;;) {

    iChoice = iMenu();
    switch (iChoice) {
      case 1:
        vTestEchoSerial();
        break;
      case 2:
        vTestOrientation();
        break;
      case 3:
        vSetContrast();
        break;
      case 4:
        vSetBacklight();
        break;
    }
  }
  return 0;
}

