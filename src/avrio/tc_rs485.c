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

#if defined(AVRIO_TC_ENABLE) && (AVRIO_TC_FLAVOUR == TC_FLAVOUR_RS485)
/* ========================================================================== */

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vTcPrivTxEn (bool bTxEn, xTcPort * p) {
  xTcIrqDcb * d = pxTcIrqDcb(p);

  if ( (int8_t) bTxEn != d->txen) {
    // Modifie l'état du l'USART uniquement si il est différent
    if (bTxEn) {

      // Invalide la réception
      vUartDisableRxIrq(p);
      vUartDisableRx(p);
      vRxenClear (p);
      // Valide la transmission
      vTxenSet (p);
      vUartEnableTx(p);
      vUartEnableUdreIrq(p);
    }
    else {

      // Invalide la transmission
      // delay_ms(1);
      vUartDisableTxIrqs(p);
      vUartDisableTx(p);
      vTxenClear (p);
      // Valide la réception
      vRxenSet(p);
      vUartEnableRx(p);
      vUartClearRxError(p);
      vUartEnableRxIrq(p);
    }
    d->txen = bTxEn;
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivRxEn (bool bRxEn, xTcPort * p) {

  vTcPrivTxEn (!bRxEn, p);
}

/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR == TC_FLAVOUR_RS485 */
