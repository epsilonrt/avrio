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
 * @enum eTcDataBits
 * @brief Nombre de bits de données
 */
typedef enum {
  TC_DATABIT_5 = 5,
  TC_DATABIT_6 = 6,
  TC_DATABIT_7 = 7,
  TC_DATABIT_8 = 8,
  TC_DATABIT_9 = 9,
  TC_DATABIT_UNKNOWN = -1
} eTcDataBits;

/**
 * @enum eTcParity
 * @brief Parité
 */
typedef enum {
  TC_PARITY_NONE = 'N',
  TC_PARITY_EVEN = 'E',
  TC_PARITY_ODD  = 'O',
  TC_PARITY_SPACE = 'S',
  TC_PARITY_MARK = 'M',
  TC_PARITY_UNKNOWN = -1
} eTcParity;

/**
 * @enum eTcStopBits
 * @brief Nombre de bits de stop
 */
typedef enum {
  TC_STOPBIT_ONE = 1,
  TC_STOPBIT_TWO = 2,
  TC_STOPBIT_ONEHALF = 3,
  TC_STOPBIT_UNKNOWN
} eTcStopBits;

/**
 * @enum eTcFlow
 * @brief Type de contrôle de flux
 */
typedef enum {

  TC_FLOWCTL_NONE = ' ',
  TC_FLOWCTL_RTSCTS = 'H',
  TC_FLOWCTL_XONXOFF = 'S',
  TC_FLOWCTL_UNKNOWN = -1
} eTcFlow;

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

/* structures =============================================================== */
/**
 * Configuration d'un port série
 */
typedef struct xTcIos {
  long baud; /**< Vitesse de transmission, négative si erreur */
  eTcDataBits dbits; /**< Bits de données */
  eTcParity parity; /**< Parité */
  eTcStopBits sbits;/**< Bits de stop */
  eTcFlow flowctl;/**< Contrôle de flux */
} xTcIos;

#define TC_SETTINGS(baud) { .baud = baud, .dbits = 8, .parity = 'N', \
                            .sbits = 1, .flowctl = TC_FLOWCTL_NONE }
/**
 *   @}
 * @}
 */

/* internal public functions ================================================ */
#include <stdarg.h>
FILE * xTcOpen (const char * port, int flag, xTcIos * ios);
// int iTcClose (FILE * f);
// int iTcIoCtl (FILE * f, int c, va_list ap);

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_TC_H_ */
