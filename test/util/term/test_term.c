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
 * @file test_term.c
 * @brief Tests unitaires du module \ref term_module "Terminal".
 *
 * @{
   ========================================================================== */
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
