/**
 * @file fram.h
 * @brief FRAM Driver
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120229 - Initial version by epsilonRT
 */
#ifndef _IDWARF_FRAM_H_
#define _IDWARF_FRAM_H_

#include "idwarf.h"
#include "iprotocol.h"

/* constants ================================================================ */
#define FRAM_LENGTH (25 * IDWARF_BACKDATA_SIZE)

/* internal public functions ================================================ */

//--------------------------------------------------------------------------
//
//  Function:    FramInit
//
//  Description: Initialize the Fram to the value passed in.
//
//  Inputs:      Every location of FRAM will have Value written to it.
//
//--------------------------------------------------------------------------
void FramInit (U8 Value);

//--------------------------------------------------------------------------
//
//  Function:    FramReadByte
//
//  Description: Reads a single byte from Fram
//
//  Inputs:      2-byte address of memory location to read
//
//--------------------------------------------------------------------------
U8 FramReadByte (U16 Addr);

//--------------------------------------------------------------------------
//
//  Function:    FramReadBytes
//
//  Description: Reads the number of bytes specified in count from Fram
//               into data
//
//  Inputs:      Addr: 2-byte address of memory location to read
//               pData: pointer to array to store read bytes
//               Count: number of bytes to read
//
//--------------------------------------------------------------------------
void FramReadBytes (U16 Addr, U8 * pData, U8 Count);

//--------------------------------------------------------------------------
//
//  Function:    FramWriteByte
//
//  Description: Writes a single byte to Fram
//
//  Inputs:      Addr: 2-byte address of memory location to write
//               Value: byte to be written to memory
//
//--------------------------------------------------------------------------
void FramWriteByte (U16 Addr, U8 Value);

//--------------------------------------------------------------------------
//
//  Function:    FramWriteBytes
//
//  Description: Writes the number of bytes specified in count to Fram
//               from data
//
//  Inputs:      Addr: 2-byte address of memory location to write
//               pData: pointer to array of bytes to be stored
//               Count: number of bytes to write
//
//--------------------------------------------------------------------------
void FramWriteBytes (U16 Addr, U8 * pData, U8 Count);

/* structures =============================================================== */
//--------------------------------------
typedef union _FRAM_STATUS
{
    struct
    {   // LSB
        // These follow the Data Packet format
        unsigned Retry      : 5;    // Number of TX attempts before giving up. 0 = infinate
        unsigned Enabled    : 1;    // 1 = Device is Enabled
        unsigned DataToSend : 1;    // 1 = Back channel data to send
        unsigned Notify     : 1;    // 1 = Notify Host when data sent.
        // MSB
    } sFlags;
    
    U8 Clear;
    
} FRAM_STATUS;

//--------------------------------------
typedef struct _FRAM_STATUS_EX
{   // LSB
    unsigned SingleByteDeviceId : 1;    // 1 = 8 bit Device Id, 0 = 16 bit Device Id (Not implemented in this release)
    unsigned HubSeqn            : 1;    // Data sequence bit
    unsigned SensorSeqn         : 1;    // Data sequence bit
    unsigned Reserved           : 5;
    // MSB
    
} FRAM_STATUS_EX;

//--------------------------------------
typedef struct _FRAM_STRUCT
{
    FRAM_STATUS     sStatus;
    U8              u8Length; // Number of bytes in u8BackChannelData
    FRAM_STATUS_EX  sStatusEx;
    U8              u8BackChannelData[9];
    MID             sMid;

} FRAM_STRUCT;

/* ========================================================================== */
#endif  /* _IDWARF_FRAM_H_ not defined */
