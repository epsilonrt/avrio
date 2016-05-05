/**
 * @file serial_rs485.c
 * @brief Liaison série RS485 half-duplex sous interruption
 *
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
 */
#include "serial_rs485_private.h"

#if AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485
/* ========================================================================== */

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vSerialPrivateTxEn (bool bTxEn) {

  if ( (int8_t) bTxEn != iSerialTxEn) {
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
    iSerialTxEn = bTxEn;
  }
}

// -----------------------------------------------------------------------------
void
vSerialPrivateRxEn (bool bRxEn) {

  vSerialPrivateTxEn (!bRxEn);
}

/* ========================================================================== */
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485 */
