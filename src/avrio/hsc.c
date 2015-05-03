/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file hsc.c
 * @brief Capteur de pression Honeywell HSC (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    2014 - Initial version by epsilonRT
 */
#include <avrio/spi.h>
#include <avrio/twi.h>
#include "hsc.h"

/* constants ================================================================ */
#define HSC_S1  7
#define HSC_S0  6
#define HSC_STATUS_MASK (_BV(HSC_S1)|_BV(HSC_S0))
#define HSC_ISSTALE(_byte) (((_byte)&HSC_STATUS_MASK) == _BV(HSC_S1))

/*
 * Constantes utilisées dans le calcul de la pression et la température
 * par vHscRawToValue().
 */
#define HSC_OUTPUT_MIN 1638.0f
#define HSC_OUTPUT_MAX 14745.0f
#define HSC_TEMP_SLOPE (200.0f/2047.0f)
#define HSC_TEMP_ZERO  50.0f

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iHscGetRaw (xHscSensor *pSensor, xHscRaw *pRaw) {
  uint8_t ucByte;

  if (pSensor->eBus == HSC_SPI) {
    pSensor->vSelect (true);
    do {

      ucByte = ucSpiMasterRead();
    } while (HSC_ISSTALE (ucByte));
    if (ucByte & HSC_STATUS_MASK) {

      pSensor->vSelect (false);
      return -(((int)(ucByte & HSC_STATUS_MASK)) >> HSC_S0);
    }
    pRaw->usPress = ucByte << 8;
    ucByte = ucSpiMasterRead();
    pRaw->usPress |= ucByte;
    ucByte = ucSpiMasterRead();
    pRaw->usTemp = ucByte << 3;
    ucByte = ucSpiMasterRead();
    pRaw->usTemp |= ucByte >> 5;
    pSensor->vSelect (false);
  }
  else {
    // TODO: I2C
  }

  return 0;
}

// -----------------------------------------------------------------------------
void
vHscRawToValue (xHscSensor *pSensor, const xHscRaw *pRaw, xHscValue *pValue) {

  pValue->dPress = (((double)pRaw->usPress - HSC_OUTPUT_MIN) *
                        (pSensor->dPressMax - pSensor->dPressMin)) /
                        (HSC_OUTPUT_MAX - HSC_OUTPUT_MIN) +
                        pSensor->dPressMin;

  pValue->dTemp = (double)pRaw->usTemp * HSC_TEMP_SLOPE - HSC_TEMP_ZERO;
}

// -----------------------------------------------------------------------------
int
iHscGetValue (xHscSensor *pSensor, xHscValue *pValue) {
  xHscRaw xRaw;
  int iError = iHscGetRaw (pSensor, &xRaw);

  if (iError == 0) {

    vHscRawToValue (pSensor, &xRaw, pValue);
  }
  return iError;
}

/* ========================================================================== */
