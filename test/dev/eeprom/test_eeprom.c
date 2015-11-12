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
 * @file test_eeprom.c
 * @brief Tests unitaires du module \ref vector_module "Vector".
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avrio/eeprom.h>
#include <avrio/assert.h>
#include <avrio/serial.h>


/* constants ================================================================ */
#define SERIAL_BAUDRATE 38400
#define SERIAL_SETTINGS (SERIAL_DEFAULT + SERIAL_WR)

#define BUFFER_SIZE (EEPROM_SIZE / 2 - 1)

/* private variables ======================================================== */
static uint8_t ucBufferEe[2][BUFFER_SIZE + 1] EEMEM;
static uint8_t ucBuffer[2][BUFFER_SIZE];

/* main ===================================================================== */
int
main (int argc, char **argv) {
  int i, j;
  uint8_t u = 0;

  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_SETTINGS);
  stdout = &xSerialPort;
  stderr = &xSerialPort;

  printf ("** EEPROM Test **\n"
          "EEPROM size: %d bytes\n"
          "Buffer size: %d bytes\n",
          EEPROM_SIZE, EEPROM_SIZE / 2);

  for (i = 0; i < BUFFER_SIZE; i++) {

    ucBuffer[0][i] = u++;
  }

  for (i = 0; i < 2; i++) {

    printf ("\n\nWrite block 0x%04X-0x%04X\n", (BUFFER_SIZE + 1) * i, (BUFFER_SIZE + 1) * (i + 1) - 1);
    vEepromSaveBlock (&ucBuffer[0][0], &ucBufferEe[i][0], BUFFER_SIZE);

    printf ("\nRead block");
    if (iEepromLoadBlock (&ucBuffer[1][0], &ucBufferEe[i][0], BUFFER_SIZE) == 0) {

      u = true;
      for (j = 0; j < BUFFER_SIZE; j++) {

        if ( (j % 32) == 0) {

          putchar ('\n');
        }
        if (ucBuffer[0][j] == ucBuffer[1][j]) {
          
          printf ("%02X", ucBuffer[0][j]);
        }
        else {
          fprintf (stderr, "\nByte error 0x%04X, write %02X, read %02X\n",
                   j, ucBuffer[0][j], ucBuffer[1][j]);
          u = false;
          break;
        }
      }

      if (u) {

        printf ("\n\n%d bytes were read and written successfully\n", (BUFFER_SIZE + 1));
      }
    }
    else {

      fprintf (stderr, "CRC error !\n");
    }
  }

  return 0;
}

/* ========================================================================== */
