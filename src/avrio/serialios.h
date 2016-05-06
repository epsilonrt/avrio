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
 * @file 
 * @brief Configuration liaison série
 */
#ifndef _AVRIO_SERIAL_IOS_H_
#define _AVRIO_SERIAL_IOS_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup serial_module
 * @{
 *
 *  @defgroup serial_ios_module Configuration
 *
 *  Ce port permet de gérer la communication sur une liaison série asynchrone
 *  à l'aide du coupleur USART.
 *  @{
 */

/* constants ================================================================ */
/**
 * @brief Erreur de baudrate
 */
#define EBADBAUD (-2)

/**
 * @enum eSerialDataBits
 * @brief Nombre de bits de données
 */
typedef enum {
  SERIAL_DATABIT_5 = 5,
  SERIAL_DATABIT_6 = 6,
  SERIAL_DATABIT_7 = 7,
  SERIAL_DATABIT_8 = 8,
  SERIAL_DATABIT_9 = 9,
  SERIAL_DATABIT_UNKNOWN = -1
} eSerialDataBits;

/**
 * @enum eSerialParity
 * @brief Parité
 */
typedef enum {
  SERIAL_PARITY_NONE = 'N',
  SERIAL_PARITY_EVEN = 'E',
  SERIAL_PARITY_ODD  = 'O',
  SERIAL_PARITY_SPACE = 'S',
  SERIAL_PARITY_MARK = 'M',
  SERIAL_PARITY_UNKNOWN = -1
} eSerialParity;

/**
 * @enum eSerialStopBits
 * @brief Nombre de bits de stop
 */
typedef enum {
  SERIAL_STOPBIT_ONE = 1,
  SERIAL_STOPBIT_TWO = 2,
  SERIAL_STOPBIT_ONEHALF = 3,
  SERIAL_STOPBIT_UNKNOWN
} eSerialStopBits;

/**
 * @enum eSerialFlow
 * @brief Type de contrôle de flux
 */
typedef enum {

  SERIAL_FLOW_NONE = ' ',
  SERIAL_FLOW_RTSCTS = 'H',
  SERIAL_FLOW_XONXOFF = 'S',
  SERIAL_FLOW_UNKNOWN = -1
} eSerialFlow;

/* constants ================================================================ */
#define  SERIAL_BINARY  0 /**< Flux binaire, pas de modification */
#define  SERIAL_CR      0x0D /**< Les LF (0x0A) sont remplacés par des CR (0x0D) */
#define  SERIAL_LF      0x0A /**< Les CR (0x0D) sont remplacés par des LF (0x0A) */
#define  SERIAL_CRLF    (SERIAL_CR + SERIAL_LF) /**< Un CR (0x0D) est ajouté avant chaque LF (0x0A) */

/* structures =============================================================== */
/**
 * Configuration d'un port série
 */
typedef struct xSerialIos {
  long baud; /**< Vitesse de transmission, négative si erreur */
  eSerialDataBits dbits; /**< Bits de données */
  eSerialParity parity; /**< Parité */
  eSerialStopBits sbits;/**< Bits de stop */
  eSerialFlow flow;/**< Contrôle de flux */
  char eol; /**< Fin de ligne, SERIAL_CRLF par défaut */
} xSerialIos;

#define SERIAL_SETTINGS(b) { \
  .baud = b, .dbits = SERIAL_DATABIT_8, .parity = SERIAL_PARITY_NONE, \
  .sbits = SERIAL_STOPBIT_ONE, .flow = SERIAL_FLOW_NONE, .eol =  SERIAL_CRLF }

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_SERIAL_IOS_H_ */
