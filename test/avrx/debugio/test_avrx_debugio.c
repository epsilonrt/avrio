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
 * @file test_avrx_debugio.c
 *
 * Revision History ------------------------------------------------------------
 *    20110907 - Initial version by epsilonRT
 * ---
 * @brief Test du driver série pour le debug
 */
#include <avrio/led.h>
#include <avrio/avrx.h>
#include <avrio/serial.h>
#include <avrx/debug.h>

/* constants ================================================================ */
#define BAUDRATE 9600

/* internal public functions ================================================ */
const char Hello[] FLASH = "\nHello World !\nPress keys until CR ? ";
const char Word[] FLASH = "\nWord ? ";
const char Byte[] FLASH = "\nByte ? ";
const char Digit[] FLASH = "\nDigit ? ";
const char Error[] FLASH = "\nError ! ";


AVRX_TASK (task1, 20, 2) {
  char c;
  uint16_t usWord;

  for (;;) {

    c = 'A';
    vDebugPutCR();
    while (c <= 'Z') {

      vDebugPutChar (c++);
      vLedToggle (LED_LED1);
    }

    vDebugPutCR();
    vDebugPutHexWord (0x1234);
    vDebugPutSpace();
    vDebugPutHexByte (0x56);
    vDebugPutSpace();
    vDebugPutHexDigit (0x7);
    vDebugPutSpace();

    vDebugPutString (Hello);
    while (c != 0x0D) {

      c = cDebugGetChar();
      vDebugPutChar (c);
      vLedToggle (LED_LED1);
    }

    vDebugPutString (Word);
    usWord = usDebugGetHexWord();
    if (ucDebugGetError()) {

      vDebugPutString (Error);
    }
    else {

      vDebugPutCR();
      vDebugPutHexWord (usWord);
   }

    vDebugPutString (Byte);
    usWord = ucDebugGetHexByte();
    if (ucDebugGetError()) {

      vDebugPutString (Error);
    }
    else {

      vDebugPutCR();
      vDebugPutHexByte (usWord);
   }

    vDebugPutString (Digit);
    usWord = ucDebugGetHexDigit();
    if (ucDebugGetError()) {

      vDebugPutString (Error);
    }
    else {

      vDebugPutCR();
      vDebugPutHexDigit (usWord);
   }
  }
}

// -----------------------------------------------------------------------------
int
main (void){

  vLedInit();
  vDebugInit (SERIAL_BAUD_X1(BAUDRATE / 100L));

  vAvrXRunTask (task1);
  vAvrXStart ();
}

/* ========================================================================== */
