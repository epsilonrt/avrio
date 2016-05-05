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
 * @file ifram.c
 * @brief Driver FRAM - SRAM version
 *
 * Revision History ------------------------------------------------------------
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
