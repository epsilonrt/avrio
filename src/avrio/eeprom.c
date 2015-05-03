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
 * @file eeprom.c
 * @brief Gestion de la mémoire EEPROM.
 */
#include <util/crc16.h>
#include <avrio/eeprom.h>

/* constants ================================================================ */
#define CRC_INITIAL_VALUE 0x5A

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
// Calcul du CRC d'un bloc d'octet
static uint8_t
prvucComputeCrc (const uint8_t * pucSrc, size_t xSize) {
  uint8_t ucCrc = CRC_INITIAL_VALUE;

  while (xSize--) {

    ucCrc = _crc_ibutton_update (ucCrc, *pucSrc++);
  }

  return ucCrc;
}

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
void
vEepromSaveBlock (const void *pvSrcRam, void *pvDstEem, size_t xSize) {

  eeprom_write_block (pvSrcRam, pvDstEem, xSize);
  eeprom_write_byte (pvDstEem + xSize,
                     prvucComputeCrc ((uint8_t *) pvSrcRam, xSize));
}

// ------------------------------------------------------------------------------
int
iEepromLoadBlock (void *pvDstRam, const void *pvSrcEem, size_t xSize) {
  uint8_t ucCrc;

  eeprom_read_block (pvDstRam, pvSrcEem, xSize);
  ucCrc = eeprom_read_byte (pvSrcEem + xSize);

  if (prvucComputeCrc ((uint8_t *) pvDstRam, xSize) != ucCrc) {

    return -1;
  }
  return 0;
}

/* ========================================================================== */
