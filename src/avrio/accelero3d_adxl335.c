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
 * @file accelero3d_adxl335.c
 * @brief Acceleromètre 3 axes ADXL335
 *
 * Revision History ------------------------------------------------------------
 *    20121213 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_ACCELERO3D_ENABLE
/* ========================================================================== */
#  ifndef AVRIO_ADC_ENABLE
#   error AVRIO_ADC_ENABLE must be defined !
#  endif
#  include "gyro3d.h"
#  include "adc.h"
#  include "delay.h"
#  include "avrio-board-adxl335.h"
#  include "avrio-board-adc.h"

/* constants ================================================================ */
#  define AXE_X 0
#  define AXE_Y 1
#  define AXE_Z 2

/* public variables ========================================================= */
const float fAccelero3dLsb = ADXL335_ACC_LSB;

/* private variables ======================================================== */
static uint8_t ucAdxl335AdcChan[3] = ADXL335_ADC_LIST;

// -----------------------------------------------------------------------------
int
prviReadAxe (uint8_t ucAxe) {

  return ((int) usAdcReadAverage (ucAdxl335AdcChan[ucAxe], 16)) - 512;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iAccelero3dDeviceInit (void) {

  vAdcInit();
  return 0;
}


// -----------------------------------------------------------------------------
int
iAccelero3dReadRaw (int * iAcc) {

  iAcc[0] = prviReadAxe (AXE_X);
  iAcc[1] = prviReadAxe (AXE_Y);
  iAcc[2] = prviReadAxe (AXE_Z);

#if ADXL335_AXIS_SIGN & _BV(2)
  iAcc[0] = -iAcc[0];
#endif
#if ADXL335_AXIS_SIGN & _BV(1)
  iAcc[1] = -iAcc[1];
#endif
#if ADXL335_AXIS_SIGN & _BV(0)
  iAcc[2] = -iAcc[2];
#endif

  return 0;
}

/* ========================================================================== */
#endif /* AVRIO_ACCELERO3D_ENABLE defined */
