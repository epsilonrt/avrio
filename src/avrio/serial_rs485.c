/**
 * @file serial_rs485.c
 * @brief Liaison série RS485 half-duplex sous interruption
 *
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
 */
#include "serial_rs485_private.h"

#if AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485
/* ========================================================================== */

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vSerialPrivateTxEn (bool bTxEn) {

  if (bTxEn) {

    // Invalide la réception
    vRxIrqDisable();
    vRxDisable();
    vRxEnClear ();
    // Valide la transmission
    vTxEnSet ();
    vTxEnable();
    vTxIrqEnable();
  }
  else {

    // Invalide la transmission
    vTxIrqDisable();
    vTxDisable();
    vTxEnClear ();
    // Valide la réception
    vRxEnSet();
    vRxEnable();
    vRxClearError();
    vRxIrqEnable();
  }
}

// -----------------------------------------------------------------------------
void
vSerialPrivateRxEn (bool bRxEn) {
  
  vSerialPrivateTxEn (!bRxEn);
}

/* ========================================================================== */
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485 */
