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
 * @file l3g.h
 * @brief Interface du circuit L3G
 *
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_L3G_H_
#define _AVRIO_L3G_H_

#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */

#if ! defined(__DOXYGEN__)

/* constants ================================================================ */
#define L3G_WHO_AM_I      0x0F

#define L3G_CTRL_REG1     0x20
#define L3G_CTRL_REG2     0x21
#define L3G_CTRL_REG3     0x22
#define L3G_CTRL_REG4     0x23
#define L3G_CTRL_REG5     0x24

#define L3G_REFERENCE     0x25
#define L3G_OUT_TEMP      0x26

#define L3G_STATUS_REG    0x27
#define L3G_ZYXOR   7
#define L3G_ZOR     6
#define L3G_YOR     5
#define L3G_XOR     4
#define L3G_ZYXDA   3 
#define L3G_ZDA     2
#define L3G_YDA     1
#define L3G_XDA     0

#define L3G_OUT_X_L       0x28
#define L3G_OUT_X_H       0x29
#define L3G_OUT_Y_L       0x2A
#define L3G_OUT_Y_H       0x2B
#define L3G_OUT_Z_L       0x2C
#define L3G_OUT_Z_H       0x2D
#define L3G_AUTOINC    7

#define L3G_FIFO_CTRL_REG 0x2E
#define L3G_FIFO_SRC_REG  0x2F

#define L3G_INT1_CFG      0x30
#define L3G_AOI   7
#define L3G_LIR   6
#define L3G_ZHIE  5
#define L3G_ZLIE  4
#define L3G_YHIE  3
#define L3G_YLIE  2
#define L3G_XHIE  1
#define L3G_XLIE  0

#define L3G_INT1_SRC      0x31
#define L3G_IA 6
#define L3G_ZH 5
#define L3G_ZL 4
#define L3G_YH 3
#define L3G_YL 2
#define L3G_XH 1
#define L3G_XL 0

#define L3G_INT1_THS_XH   0x32
#define L3G_INT1_THS_XL   0x33
#define L3G_INT1_THS_YH   0x34
#define L3G_INT1_THS_YL   0x35
#define L3G_INT1_THS_ZH   0x36
#define L3G_INT1_THS_ZL   0x37

#define L3G_INT1_DURATION 0x38

#endif /* __DOXYGEN__ not defined */

#if defined(__cplusplus)
  }
#endif
/* ========================================================================== */
#endif  /* _AVRIO_L3G_H_ not defined */

