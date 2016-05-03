/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file tc.h
 * @brief Liaison série
 */
#ifndef _AVRIO_TC_H_
#define _AVRIO_TC_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <avr/interrupt.h>
#include <avrio/file.h>
#include <avrio/serialios.h>

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup tc_module Liaison série
 *
 *  Ce port permet de gérer la communication sur une liaison série asynchrone
 *  à l'aide du coupleur USART.
 *  @{
 *  @example tc/demo_tc.c
 *  Renvoie chaque caractère reçu sur la liaison série en basculant la LED1.
 */

/* constants ================================================================ */

/**
 * @enum eTcError
 * @brief Codes d'erreur
 */
typedef enum {
  eTcRxParityError    = 0x0001,  /**< Erreur de parité */
  eTcRxFormatError    = 0x0002,  /**< Erreur de format */
  eTcRxOverflowError  = 0x0004,  /**< Débordement de la pile de réception */
  eTcTxOverflowError  = 0x0008,  /**< Débordement de la pile de transmission */
  eTcParityUnknown    = 0x0010,
  eTcDataBitsUnknown  = 0x0020,
  eTcStopBitsUnknown  = 0x0040,
  eTcFlowCtlUnknown   = 0x0080,
} eTcError;

/* constants ================================================================ */
#define TC_FLAVOUR_POLL   0x01
#define TC_FLAVOUR_IRQ    0x02

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_TC_H_ */
