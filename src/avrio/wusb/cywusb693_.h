// --------------------------------------------------------------------------
// 
// Filename: cywusb693_.h
// 
// Description: Contains the radio register definitions for the
// WirelessUSBô family of parts.  CYWUSB693_
// 
// --------------------------------------------------------------------------
// $Archive: /WirelessUSBô/LS/N-to-1/Firmware/Hub/CyWusb693_.h $
// $Modtime: 7/07/04 10:54a $
// $Revision: 1.1 $
// --------------------------------------------------------------------------
// 
// Copyright 2003-2004, Cypress Semiconductor Corporation.
// 
// This software is owned by Cypress Semiconductor Corporation (Cypress)
// and is protected by and subject to worldwide patent protection (United
// States and foreign), United States copyright laws and international
// treaty provisions. Cypress hereby grants to licensee a personal,
// non-exclusive, non-transferable license to copy, use, modify, create
// derivative works of, and compile the Cypress Source Code and derivative
// works for the sole purpose of creating custom software in support of
// licensee product to be used only in conjunction with a Cypress integrated
// circuit as specified in the applicable agreement. Any reproduction,
// modification, translation, compilation, or representation of this
// software except as specified above is prohibited without the express
// written permission of Cypress.
// 
// Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Cypress reserves the right to make changes without further notice to the
// materials described herein. Cypress does not assume any liability arising
// out of the application or use of any product or circuit described herein.
// Cypress does not authorize its products for use as critical components in
// life-support systems where a malfunction or failure may reasonably be
// expected to result in significant injury to the user. The inclusion of
// Cypress' product in a life-support systems application implies that the
// manufacturer assumes all risk of such use and in doing so indemnifies
// Cypress against all charges.
// 
// Use may be limited by and subject to the applicable Cypress software
// license agreement.
// 
// --------------------------------------------------------------------------

#ifndef _CYWUSB693__H_
# define _CYWUSB693__H_

// --------------------------------------
// API Definitions and Types
// --------------------------------------

// --------------------------------------------------------------------------
// CY Radio Register Definitions
// --------------------------------------------------------------------------

// -------------------------------------
# define REG_ID              0x00
# define mVERSION            0x0F
# define mMANUFACTURING      0xF0

// -------------------------------------
# define REG_SYN_A_CNT       0x01
# define mSYN_A_COUNT        0x1F

// -------------------------------------
# define REG_SYN_N_CNT       0x02
# define mSYN_N_COUNT        0x7F

// -------------------------------------
# define REG_CONTROL         0x03
# define bRX_ENABLE          0x80
# define bTX_ENABLE          0x40
# define bPN_CODE_SELECT     0x20
# define bBYPASS_INTERNAL_SYN_LOCK_SIGNAL 0x10
# define bAUTO_PA_DISABLE    0x08
# define bPA_ENABLE          0x04
# define bAUTO_SYN_DISABLE   0x02
# define bSYN_ENABLE         0x01

// -------------------------------------
# define REG_DATA_RATE       0x04
# define b32_CHIP_BITS       0x04
# define b64_CHIP_BITS       0x00
# define bDOUBLE_DATA_RATE   0x02
# define bNORMAL_DATA_RATE   0x00
# define b12X_OVER_SAMPLING  0x01
# define b6X_OVER_SAMPLING   0x00

// -------------------------------------
# define REG_CONFIG          0x05
# define bRX_INVERT          0x10
# define bTX_INVERT          0x08
# define bIRQ_OD             0x02
# define bIRQ_CMOS           0x00
# define bIRQ_ACTIVE_HIGH    0x01
# define bIRQ_ACTIVE_LOW     0x00

// -------------------------------------
# define REG_SERDES_CTL      0x06
# define bSERDES_ENABLE      0x08
# define mEND_OF_FRAME_LEN   0x07

// -------------------------------------
# define REG_RX_INT_EN       0x07
# define bUNDER_B            0x80
# define bOVER_B             0x40
# define bEOF_B              0x20
# define bFULL_B             0x10
# define bUNDER_A            0x08
# define bOVER_A             0x04
# define bEOF_A              0x02
# define bFULL_A             0x01

// -------------------------------------
# define REG_RX_INT_STAT     0x08
# define bRX_VALID_B         0x80
# define bRX_VALID_A         0x08
// Other bit defines are the same as REG_RX_INT_EN

// -------------------------------------
# define REG_RX_DATA_A       0x09

// -------------------------------------
# define REG_RX_VALID_A      0x0A

// -------------------------------------
# define REG_RX_DATA_B       0x0B

// -------------------------------------
# define REG_RX_VALID_B      0x0C

// -------------------------------------
# define REG_TX_INT_EN       0x0D
# define bTX_UNDERFLOW       0x08
# define bTX_OVERFLOW        0x04
# define bTX_DONE            0x02
# define bTX_EMPTY           0x01

// -------------------------------------
# define REG_TX_INT_STAT     0x0E
// Bit defines are the same as REG_TX_INT_EN

// -------------------------------------
# define REG_TX_DATA         0x0F

// -------------------------------------
# define REG_TX_VALID        0x10

// -------------------------------------
# define REG_PN_CODE         0x11
// Address 0x11 is LSB, 0x18 is MSB
// Transmit and receive LSB first

// -------------------------------------
# define REG_THRESHOLD_L     0x19
# define mTHRESHOLD_L        0x7F

// -------------------------------------
# define REG_THRESHOLD_H     0x1A
# define mTHRESHOLD_H        0x7F

// -------------------------------------
// Register 0x1B is Reserved

// -------------------------------------
# define REG_WAKE_EN         0x1C
# define bWAKE_EN            0x01

// -------------------------------------
# define REG_WAKE_STAT       0x1D
# define bWAKE_STATUS        0x01

// -------------------------------------
// Registers 0x1E - 0x1F are Reserved

// -------------------------------------
# define REG_ANALOG_CTL      0x20
# define bAGC_DISABLE        0x40
# define bMIDREAD_ENABLE     0x20
# define bPA_OUTPUT_ENABLE   0x04
# define bPA_INVERT          0x02
# define bRESET              0x01

// -------------------------------------
# define REG_CHANNEL         0x21
# define bA_AND_N            0x80
# define mCHANNEL            0x7F

// -------------------------------------
# define REG_RSSI            0x22
# define bRSSI_VALID         0x20
# define mRSSI               0x1F

// -------------------------------------
# define REG_PA              0x23
# define mPA_BIAS            0x07

// -------------------------------------
# define REG_CRYSTAL_ADJ         0x24
# define bCLOCK_DISABLE      0x40
# define bCLOCK_ENABLE       0x00
# define mCRYSTAL_ADJ        0x3F

// -------------------------------------
// Register 0x25 is Reserved

// -------------------------------------
# define REG_VCO_CAL         0x26
# define bMINUS5_PLUS5       0xC0
# define bMINUS2_PLUS3       0x80

// -------------------------------------
// Registers 0x27 - 0x2D are Reserved

// -------------------------------------
# define REG_PWR_CTL         0x2E
# define bPWR_CTL            0x80

// -------------------------------------
# define REG_CARRIER_DETECT  0x2F
# define bCDET_OVERRIDE      0x80

// -------------------------------------
// Registers 0x30 - 0x31 are Reserved

// -------------------------------------
# define REG_CLOCK_MANUAL    0x32
# define CM_DEFAULT          0x41

// -------------------------------------
# define REG_CLOCK_ENABLE    0x33
# define CE_DEFAULT          0x41

// -------------------------------------
// Registers 0x34 - 0x37 are Reserved

// -------------------------------------
# define REG_SYN_LOCK_CNT    0x38

// -------------------------------------
// Registers 0x39 - 0x3B are Reserved

// -------------------------------------
# define REG_MID             0x3C
// Address 0x3C is LSB, 0x3F is MSB

// --------------------------------------------------------------------------

#endif // _CYWUSB693__H_
