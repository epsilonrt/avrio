/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file eeprom.c
 * @brief Gestion de la mémoire EEPROM.
 */
#include <avrio/eeprom.h>
#include <avrio/crc.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vEepromSaveBlock (const void * pvSrcRam, void * pvDstEem, size_t xSize) {

  eeprom_write_block (pvSrcRam, pvDstEem, xSize);
  eeprom_write_byte (pvDstEem + xSize,
                     ucCrcIButton (CRC_IBUTTON_INIT_VAL, pvSrcRam, xSize));
}

// -----------------------------------------------------------------------------
int
iEepromLoadBlock (void * pvDstRam, const void * pvSrcEem, size_t xSize) {
  uint8_t ucCrc;

  eeprom_read_block (pvDstRam, pvSrcEem, xSize);
  ucCrc = eeprom_read_byte (pvSrcEem + xSize);

  if (ucCrcIButton (CRC_IBUTTON_INIT_VAL, pvDstRam, xSize) != ucCrc) {

    return -1;
  }
  return 0;
}

/* ========================================================================== */
