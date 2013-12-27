/**
 * @file ds1621.c
 * @brief Thermostat I2c DS1621 de DALLAS/MAXIM - Implémentation
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/ds1621.h>
#include <avrio/delay.h>

/* private variables ======================================================== */
eTwiStatus eDs1621LastErrorValue;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static xTwiLength
prvxRead (  xTwiDeviceAddr xDeviceAddr, 
            uint8_t ucCmd, uint8_t * pucData, xTwiLength xDataLen ) {
  xTwiFrame xFrame;

  xFrame.xAddr = xDeviceAddr;
  xFrame.pxData = pucData;
  xFrame.xLen = xDataLen;
  eDs1621LastErrorValue = eTwiMem8Read (ucCmd, &xFrame);
  return xFrame.xLen;
}

// -----------------------------------------------------------------------------
static xTwiLength
prvxWrite ( xTwiDeviceAddr xDeviceAddr, 
            uint8_t ucCmd, uint8_t * pucData, xTwiLength xDataLen ) {
  xTwiFrame xFrame;

  xFrame.xAddr = xDeviceAddr;
  xFrame.pxData = pucData;
  xFrame.xLen = xDataLen;
  eDs1621LastErrorValue = eTwiMem8Write (ucCmd, &xFrame);
  return xFrame.xLen;
}

/* internal public functions ================================================ */

/* ----------------------  Fonctions de bas niveau ------------------------- */

// -----------------------------------------------------------------------------//
int16_t
iDs1621ReadTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {
  int16_t iTemp = DS1621_TEMP_ERROR;
  uint8_t pucTemp[2];

  if (prvxRead (xDeviceAddr, ucCmd, pucTemp, 2) == 2) {
  
    iTemp = pucTemp[0] * 10;
    if (pucTemp[1])
      iTemp += (iTemp > 0 ? 5 : -5);
  }
  return iTemp;
}

// -----------------------------------------------------------------------------
void
vDs1621WriteTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd, int16_t iTemp) {
  uint8_t pucTemp[2];

  pucTemp[0] = (uint8_t) (iTemp / 10);
  pucTemp[1] = ((iTemp - pucTemp[0] * 10) != 0 ? 0x80 : 0);

  (void) prvxWrite (xDeviceAddr, ucCmd, pucTemp, 2);
}

// -----------------------------------------------------------------------------
uint8_t
ucDs1621ReadByte (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {
  uint8_t ucByte;

  (void) prvxRead (xDeviceAddr, ucCmd, &ucByte, 1);
  return ucByte;
}

// -----------------------------------------------------------------------------
void
vDs1621SendCmd (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {

  (void) prvxWrite (xDeviceAddr, ucCmd, 0, 0);
}

/* ----------------------  Fonctions de haut niveau ------------------------- */

// -----------------------------------------------------------------------------
void 
vDs1621Init(xTwiDeviceAddr xDeviceAddr, uint8_t ucConfig) {

  ucConfig &= (POL | ONESHOT);
  (void) prvxWrite (xDeviceAddr, DS1621_RWCONF, &ucConfig, 1);
  delay_ms (10);
}

// -----------------------------------------------------------------------------
void
vDs1621ClrFlags (xTwiDeviceAddr xDeviceAddr, uint8_t ucFlags) {
  uint8_t ucStatus;

  ucStatus = ucDs1621GetStatus (xDeviceAddr);
  ucStatus &= ~(ucFlags & TAF) ;
  (void) prvxWrite (xDeviceAddr, DS1621_RWCONF, &ucStatus, 1);
}

// -----------------------------------------------------------------------------
void 
vDs1621SetTl(xTwiDeviceAddr xDeviceAddr, int16_t iTl) {

  while (xDs1621MemIsBusy(xDeviceAddr))
    ;
  vDs1621WriteTemp(xDeviceAddr, DS1621_RWTL, iTl);
}

// -----------------------------------------------------------------------------
void 
vDs1621SetTh(xTwiDeviceAddr xDeviceAddr, int16_t iTh) {

  while (xDs1621MemIsBusy(xDeviceAddr))
    ;
  vDs1621WriteTemp(xDeviceAddr, DS1621_RWTH, iTh);
}

/* ========================================================================== */
