/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
#if !defined(HSC_SPI_ENABLE) && !defined(HSC_TWI_ENABLE)
#define HSC_SPI_ENABLE 1
#endif

#ifdef HSC_SPI_ENABLE
#include <avrio/spi.h>
#endif
#ifdef HSC_TWI_ENABLE
#include <avrio/twi.h>
#endif
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

#ifdef HSC_SPI_ENABLE  
  if (pSensor->eBus == HSC_SPI) {
    uint8_t ucByte;
    // Sélection /SS capteur
    pSensor->vSelect (true);

    // Attente nouvelle mesure
    do {

      ucByte = ucSpiMasterRead();
    }
    while (HSC_ISSTALE (ucByte));

    // Vérification du status, sortie si erreur
    if (ucByte & HSC_STATUS_MASK) {

      pSensor->vSelect (false);
      return - ( ( (int) (ucByte & HSC_STATUS_MASK)) >> HSC_S0);
    }
    // 1er octet: poids fort pression
    pRaw->usPress = ucByte << 8;
    // 2ème octet: poids faible pression
    ucByte = ucSpiMasterRead();
    pRaw->usPress |= ucByte;
    // 3ème octet: poids fort température
    ucByte = ucSpiMasterRead();
    pRaw->usTemp = ucByte << 3;
    // 4ème octet: poids faible température
    ucByte = ucSpiMasterRead();
    pRaw->usTemp |= ucByte >> 5;
    // Désélection /SS capteur
    pSensor->vSelect (false);
  }
#endif /* HSC_SPI_ENABLE defined */  

#ifdef HSC_TWI_ENABLE  
  if (pSensor->eBus == HSC_TWI) {
    uint8_t ucBuffer[4];

    xTwiFrame xFrame = {

      .xLen = 4,
      .xAddr = pSensor->ucTwiAddr,
      .pxData = ucBuffer
    };

    // Attente nouvelle mesure
    do {

      if (eTwiReceive (&xFrame) != TWI_SUCCESS) {
        return HSC_TWI_ERROR;
      }
    }
    while (HSC_ISSTALE (ucBuffer[0]));

    // Vérification du status, sortie si erreur
    if (ucBuffer[0] & HSC_STATUS_MASK) {

      return - ( ( (int) (ucBuffer[0] & HSC_STATUS_MASK)) >> HSC_S0);
    }
    // 1er/2ème octet: Pression
    pRaw->usPress = (ucBuffer[0] << 8) + ucBuffer[1];
    // 3ème octet: poids fort température
    pRaw->usTemp = (ucBuffer[2] << 3) + (ucBuffer[3] >> 5);
  }
#endif /* HSC_TWI_ENABLE defined */  

  return 0;
}

// -----------------------------------------------------------------------------
void
vHscRawToValue (xHscSensor *pSensor, const xHscRaw *pRaw, xHscValue *pValue) {

  pValue->dPress = ( ( (double) pRaw->usPress - HSC_OUTPUT_MIN) *
                     (pSensor->dPressMax - pSensor->dPressMin)) /
                   (HSC_OUTPUT_MAX - HSC_OUTPUT_MIN) +
                   pSensor->dPressMin;

  pValue->dTemp = (double) pRaw->usTemp * HSC_TEMP_SLOPE - HSC_TEMP_ZERO;
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
