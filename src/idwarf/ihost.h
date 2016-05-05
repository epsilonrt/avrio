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
 * @file ihost.h
 * @brief Hôte WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20120227 - Initial version by epsilonRT
 * --------------------------------------------------------------------------
 *               
 *  Copyright 2004, Cypress Semiconductor Corporation.
 * 
 *  This software is owned by Cypress Semiconductor Corporation (Cypress)
 *  and is protected by and subject to worldwide patent protection (United
 *  States and foreign), United States copyright laws and international
 *  treaty provisions. Cypress hereby grants to licensee a personal,
 *  non-exclusive, non-transferable license to copy, use, modify, create
 *  derivative works of, and compile the Cypress Source Code and derivative
 *  works for the sole purpose of creating custom software in support of
 *  licensee product to be used only in conjunction with a Cypress integrated
 *  circuit as specified in the applicable agreement. Any reproduction,
 *  modification, translation, compilation, or representation of this
 *  software except as specified above is prohibited without the express
 *  written permission of Cypress.
 * 
 *  Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *  WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *  Cypress reserves the right to make changes without further notice to the
 *  materials described herein. Cypress does not assume any liability arising
 *  out of the application or use of any product or circuit described herein.
 *  Cypress does not authorize its products for use as critical components in
 *  life-support systems where a malfunction or failure may reasonably be
 *  expected to result in significant injury to the user. The inclusion of
 *  Cypress' product in a life-support systems application implies that the
 *  manufacturer assumes all risk of such use and in doing so indemnifies
 *  Cypress against all charges.
 * 
 *  Use may be limited by and subject to the applicable Cypress software
 *  license agreement.
 * 
 * --------------------------------------------------------------------------
 */
#ifndef _IHOST_H_
#define _IHOST_H_

/**
 * @addtogroup idwarf_hub
 * @{
 */
 
/**
 * @enum COMMAND_STATUS
 * @brief Code renvoyés par les commandes envoyées au hub
 */ 
typedef enum
{
  CMD_STATUS_SUCCESS,                 // 0x00
  CMD_STATUS_UNKNOWN_CMD,             // Unused
  CMD_STATUS_UNKNOWN_ID,
  CMD_STATUS_BUFFER_OVERFLOW,
  CMD_STATUS_PRIOR_PAYLOAD_LOST,
  CMD_STATUS_PN_CODE_INDEX_INVALID,   // 0x05
  CMD_STATUS_CHANNEL_INVALID,
  CMD_STATUS_MSG_QUEUED,
  CMD_STATUS_INVALID_CHANNEL          // 0x08
} COMMAND_STATUS;

/**
 * @}
 */
/* ========================================================================== */
#endif /* _IHOST_H_ defined */
