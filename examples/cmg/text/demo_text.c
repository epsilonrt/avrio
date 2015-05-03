/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file demo_text.c
 * @brief CMG Text Demo
 * @author Christian Merkle. Copyright © 2oo6 - 2oo8 All rights reserved.
 *
 * Revision History ------------------------------------------------------------
 *    20120307 - Initial version by epsilonRT
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

