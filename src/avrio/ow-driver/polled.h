/**
 * Copyright © 2017 epsilonRT. All rights reserved.
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
 * @brief
 */
#ifndef _AVRIO_OW_DRIVER_POLLED_H_
#define _AVRIO_OW_DRIVER_POLLED_H_

#include <avrio/defs.h>
#include "common.h"

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup ow_driver
 * @{
 *
 *  @defgroup ow_driver_polled Driver en scrutation
 *  @{
 */
/* macros =================================================================== */
/* constants ================================================================ */
#define     OW_UART_WRITE1     0xff    //!< UART Write 1 bit pattern.
#define     OW_UART_WRITE0     0x00    //!< UART Write 0 bit pattern.
#define     OW_UART_READ_BIT   0xff    //!< UART Read bit pattern.
#define     OW_UART_RESET      0xf0    //!< UART Reset bit pattern.

/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
/* internal public functions ================================================ */
/**
 * @brief Ecriture d'un bit à 1 sur le bus
 * 
 * Génère la séquence nécessaire à l'envoi d'un bit à 1
 */
void vOwBitWrite1(void);

/**
 * @brief Ecriture d'un bit à 0 sur le bus
 * 
 * Génère la séquence nécessaire à l'envoi d'un bit à 0
 */
void vOwBitWrite0(void);

/** @brief  Read a bit from the bus. 
 *
 *  Generates the waveform for reception of a bit on the 1-Wire(R) bus.
 *
 *  \return A bitmask of the buses where a '1' was read.
 */
/**
 * @brief Lecture d'un bit sur le bus
 * 
 * Génère la séquence nécessaire à la lecture d'un bit
 */
bool vOwBitRead(void);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_OW_DRIVER_POLLED_H_ */
