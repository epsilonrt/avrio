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
 * @file lsm303.h
 * @brief Interface du circuit LSM303
 *
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LSM303_H_
#define _AVRIO_LSM303_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* constants ================================================================ */

/* ACC Registers */
#define LSM303_CTRL_REG1_A       0x20
#define LSM303_PM2 7
#define LSM303_PM1 6
#define LSM303_PM0 5
#define LSM303_DR1 4
#define LSM303_DR0 3
#define LSM303_ZEN 2
#define LSM303_YEN 1
#define LSM303_XEN 0

#define LSM303_CTRL_REG2_A       0x21
#define LSM303_BOOT  7
#define LSM303_HPM1  6
#define LSM303_HPM0  5
#define LSM303_FDS   4
#define LSM303_HPEN2 3 
#define LSM303_HPEN1 2
#define LSM303_HPCF1 1
#define LSM303_HPCF0 0

#define LSM303_CTRL_REG3_A       0x22
#define LSM303_IHL     7 
#define LSM303_PP_OD   6
#define LSM303_LIR2    5
#define LSM303_I2_CFG1 4
#define LSM303_I2_CFG0 3 
#define LSM303_LIR1    2
#define LSM303_I1_CFG1 1 
#define LSM303_I1_CFG0 0

#define LSM303_CTRL_REG4_A       0x23
#define LSM303_BDU 7
#define LSM303_BLE 6
#define LSM303_FS1 5
#define LSM303_FS0 4

#define LSM303_CTRL_REG5_A       0x24
#define LSM303_TURNON1 1
#define LSM303_TURNON0 0

#define LSM303_CTRL_REG6_A       0x25 // DLHC only
#define LSM303_I2_CLICKEN  7
#define LSM303_I2_INT1     6
#define LSM303_I2_INT2     5
#define LSM303_BOOT_I1     4
#define LSM303_P2_ACT      3 
#define LSM303_H_LACTIVE   1

#define LSM303_HP_FILTER_RESET_A 0x25 // DLH, DLM only
#define LSM303_REFERENCE_A       0x26

#define LSM303_STATUS_REG_A      0x27
#define LSM303_ZYXOR   7
#define LSM303_ZOR     6
#define LSM303_YOR     5
#define LSM303_XOR     4
#define LSM303_ZYXDA   3 
#define LSM303_ZDA     2
#define LSM303_YDA     1
#define LSM303_XDA     0

#define LSM303_OUT_X_L_A         0x28
#define LSM303_OUT_X_H_A         0x29
#define LSM303_OUT_Y_L_A         0x2A
#define LSM303_OUT_Y_H_A         0x2B
#define LSM303_OUT_Z_L_A         0x2C
#define LSM303_OUT_Z_H_A         0x2D
#define LSM303_AUTOINC 7

#define LSM303_FIFO_CTRL_REG_A   0x2E // DLHC only
#define LSM303_FM1   7
#define LSM303_FM0   6
#define LSM303_TR    5
#define LSM303_FTH4  4
#define LSM303_FTH3  3
#define LSM303_FTH2  2
#define LSM303_FTH1  1
#define LSM303_FTH0  0

#define LSM303_FIFO_SRC_REG_A    0x2F // DLHC only
#define LSM303_WTM       7
#define LSM303_OVRN_FIFO 6
#define LSM303_EMPTY     5
#define LSM303_FSS4      4
#define LSM303_FSS3      3
#define LSM303_FSS2      2
#define LSM303_FSS1      1
#define LSM303_FSS0      0

#define LSM303_INT1_CFG_A        0x30
#define LSM303_INT2_CFG_A        0x34
#define LSM303_AOI   7
#define LSM303_6D   6
#define LSM303_ZHIE  5
#define LSM303_ZLIE  4
#define LSM303_YHIE  3
#define LSM303_YLIE  2
#define LSM303_XHIE  1
#define LSM303_XLIE  0

#define LSM303_INT1_SRC_A        0x31
#define LSM303_INT2_SRC_A        0x35
#define LSM303_IA 6
#define LSM303_ZH 5
#define LSM303_ZL 4
#define LSM303_YH 3
#define LSM303_YL 2
#define LSM303_XH 1
#define LSM303_XL 0

#define LSM303_INT1_THS_A        0x32
#define LSM303_INT1_DURATION_A   0x33


#define LSM303_INT2_THS_A        0x36
#define LSM303_INT2_DURATION_A   0x37

#define LSM303_CLICK_CFG_A       0x38 // DLHC only
#define LSM303_CLICK_SRC_A       0x39 // DLHC only
#define LSM303_CLICK_THS_A       0x3A // DLHC only
#define LSM303_TIME_LIMIT_A      0x3B // DLHC only
#define LSM303_TIME_LATENCY_A    0x3C // DLHC only
#define LSM303_TIME_WINDOW_A     0x3D // DLHC only


/* MAG Registers */
#define LSM303_CRA_REG_M         0x00
#define LSM303_DO2 4
#define LSM303_DO1 3
#define LSM303_DO0 2

#define LSM303_CRB_REG_M         0x01
#define LSM303_GN2 7
#define LSM303_GN1 6
#define LSM303_GN0 5

#define LSM303_MR_REG_M          0x02
#define LSM303_MD1 1
#define LSM303_MD0 0

#define LSM303_OUT_X_H_M         0x03
#define LSM303_OUT_X_L_M         0x04
#define LSM303_OUT_Y_H_M         -1   // The addresses of the Y and Z magnetometer output registers 
#define LSM303_OUT_Y_L_M         -2   // are reversed on the DLM and DLHC relative to the DLH.
#define LSM303_OUT_Z_H_M         -3   // These four defines have dummy values so the library can 
#define LSM303_OUT_Z_L_M         -4   // determine the correct address based on the device type.

#define LSM303_SR_REG_M          0x09
#define LSM303_LOCK 1
#define LSM303_DRDY 0

#define LSM303_IRA_REG_M         0x0A
#define LSM303_IRB_REG_M         0x0B
#define LSM303_IRC_REG_M         0x0C

#define LSM303_WHO_AM_I_M        0x0F // DLM only

#define LSM303_TEMP_OUT_H_M      0x31 // DLHC only
#define LSM303_TEMP_OUT_L_M      0x32 // DLHC only

#define LSM303DLH_OUT_Y_H_M      0x05
#define LSM303DLH_OUT_Y_L_M      0x06
#define LSM303DLH_OUT_Z_H_M      0x07
#define LSM303DLH_OUT_Z_L_M      0x08

#define LSM303DLM_OUT_Z_H_M      0x05
#define LSM303DLM_OUT_Z_L_M      0x06
#define LSM303DLM_OUT_Y_H_M      0x07
#define LSM303DLM_OUT_Y_L_M      0x08

#define LSM303DLHC_OUT_Z_H_M     0x05
#define LSM303DLHC_OUT_Z_L_M     0x06
#define LSM303DLHC_OUT_Y_H_M     0x07
#define LSM303DLHC_OUT_Y_L_M     0x08

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_LSM303_H_ not defined */

