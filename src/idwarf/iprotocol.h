/**
 * @file iprotocol.h
 * @brief Protocole WirelessUSB
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
#ifndef _IPROTOCOL_H_
#define _IPROTOCOL_H_

/**
 * @addtogroup idwarf_module
 * @{
 */

/**
 * @enum PACKET_TYPES
 * @brief Type de paquets transmis sur le réseau
 */
typedef enum
{
    BIND_REQUEST_PACKET  = 0x00,
    BIND_RESPONSE_PACKET = 0x01,
    PING_PACKET          = 0x02,
    ACK_PACKET           = 0x03,
    FIXED_DATA_PACKET    = 0x04, /**< Data packet of fixed length as defined by SIZE_OF_FIXED_DATA_PACKET */
    VARIABLE_DATA_PACKET = 0x05, /**< Data packet of variable length */
    NO_PACKET            = 0xFF
} PACKET_TYPES;


/**
 * @union _MID
 * @typedef MID
 * @brief Identifiant fabricant d'une puce RF
 */
typedef union _MID
{
    U32 u32Mid; /**< AVR is Little endianess ... */
    
    struct
    {
        U8  u8Mid4; /**< MSB */
        U8  u8Mid3;
        U8  u8Mid2;
        U8  u8Mid1; /**< LSB */
    } sByte;
    
    struct
    {
        U8  u8Mid4;     /**< MSB */
        U8  u8Mid3;
        U8  u8ChecksumSeed;
        U8  u8CrcSeed;  /**< LSB */
    } sNid;
    
} MID;

/**
 * @enum PROTOCOL_STATUS
 * @brief Drapeaux d'état du réseau
 */
typedef enum
{
    TX_SYNC_DATA    = 0x01,
    BIND_ACK        = 0x02,
    AUTO_BIND       = 0x04,
    SEEDED_BIND     = 0x08,
    NODE_BOUND      = 0x10,
    BIND_ENABLED    = 0x20,
    BIND_INIT       = 0x40,
    BINDING         = 0x80  

} PROTOCOL_STATUS;

/**
 * @}
 */
/* ========================================================================== */
#endif /* _IPROTOCOL_H_ defined */
