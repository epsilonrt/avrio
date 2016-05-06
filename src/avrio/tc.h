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
 * @addtogroup net_group
 * @{
 *
 *  @defgroup serial_module Liaison série
 *
 *  Ce module permet de gérer la communication sur une liaison série asynchrone
 *  à l'aide du coupleur USART. \n
 *  On accède à une liaison série par l'intermédiaire de l'interface FILE
 *  étendue définie dans file.h. Les liaisons série matérielles sont désignées
 *  par tty0 pour l'USART 0, tty1 pour l'USART 1 ....
 *  @{
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
