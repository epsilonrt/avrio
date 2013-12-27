/**
 * @file ifram.c
 * @brief Driver FRAM - SRAM version
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120229 - Initial version by epsilonRT
 */
#if defined(HUB)

#include "fram.h"

/* private variables ======================================================== */
static U8 myFram[FRAM_LENGTH];

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
void 
FramInit (U8 Value) {
  U8 * pFram = myFram;
  
  do {
  
    *pFram++ = Value;
  } while (pFram != (myFram + FRAM_LENGTH));
}

//--------------------------------------------------------------------------
//
//  Function:    FramReadByte
//
//  Description: Reads a single byte from Fram
//
//  Inputs:      2-byte address of memory location to read
//
//--------------------------------------------------------------------------
U8 
FramReadByte (U16 Addr) {

  return myFram[Addr];
}

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
void 
FramReadBytes (U16 Addr, U8 * pData, U8 Count) {
  U8 * pFram = &myFram[Addr];
  
  while (Count--) {
    
    *pData++ = *pFram++;
  }
}

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
void 
FramWriteByte (U16 Addr, U8 Value) {

  myFram[Addr] = Value;
}

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
void 
FramWriteBytes (U16 Addr, U8 * pData, U8 Count) {

  U8 * pFram = &myFram[Addr];
  
  while (Count--) {
    
    *pFram++ = *pData++;
  }
}

#endif /* HUB defined */
/* ========================================================================== */
