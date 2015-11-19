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
#include "tc_rs485_private.h"

#if AVRIO_TC_FLAVOUR == TC_FLAVOUR_RS485
/* ========================================================================== */

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vTcPrivateTxEn (bool bTxEn) {

  if ( (int8_t) bTxEn != iTcTxEn) {
    // Modifie l'état du l'USART uniquement si il est différent
    if (bTxEn) {

      // Invalide la réception
      vRxIrqDisable();
      vRxDisable();
      vRxEnClear ();
      // Valide la transmission
      vTxEnSet ();
      vTxEnable();
      vTxUdreIrqEnable();
    }
    else {

      // Invalide la transmission
      // delay_ms(1);
      vTxIrqDisable();
      vTxDisable();
      vTxEnClear ();
      // Valide la réception
      vRxEnSet();
      vRxEnable();
      vRxClearError();
      vRxIrqEnable();
    }
    iTcTxEn = bTxEn;
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivateRxEn (bool bRxEn) {

  vTcPrivateTxEn (!bRxEn);
}

/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR == TC_FLAVOUR_RS485 */
