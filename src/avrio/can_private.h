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
 * @file can_private.h
 * @brief Bus CAN
 */
#ifndef _AVRIO_CAN_PRIVATE_H_
#  define _AVRIO_CAN_PRIVATE_H_
/* ========================================================================== */

/* constants ================================================================ */
// TODO 8MHz

#  if AVRIO_CPU_FREQ == 12000000UL
// 10 kbit/s 85.00% 20 quanta (BRP=59 Tprs=8 Tph1=8 Tph2=3 Tsjw=2)
#    define CAN_BT1_10K 0x76
#    define CAN_BT2_10K 0x2E
#    define CAN_BT3_10K 0x2E
// 20 kbit/s 86.67% 15 quanta (BRP=39 Tprs=8 Tph1=4 Tph2=2 Tsjw=2)
#    define CAN_BT1_20K 0x4E
#    define CAN_BT2_20K 0x2E
#    define CAN_BT3_20K 0x16
// 50 kbit/s 87.50% 16 quanta (BRP=14 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_50K 0x1C
#    define CAN_BT2_50K 0x2E
#    define CAN_BT3_50K 0x18
// 100 kbit/s 86.67% 15 quanta (BRP=7 Tprs=8 Tph1=4 Tph2=2 Tsjw=2)
#    define CAN_BT1_100K 0x0E
#    define CAN_BT2_100K 0x2E
#    define CAN_BT3_100K 0x16
// 125 kbit/s 87.50% 16 quanta (BRP=5 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_125K 0x0A
#    define CAN_BT2_125K 0x2E
#    define CAN_BT3_125K 0x18
// 250 kbit/s 87.50% 16 quanta (BRP=2 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_250K 0x04
#    define CAN_BT2_250K 0x2E
#    define CAN_BT3_250K 0x18
// 500 kbit/s 83.33% 12 quanta (BRP=1 Tprs=7 Tph1=2 Tph2=2 Tsjw=2)
#    define CAN_BT1_500K 0x02
#    define CAN_BT2_500K 0x2C
#    define CAN_BT3_500K 0x12
// 800 kbit/s 86.67% 15 quanta (BRP=0 Tprs=8 Tph1=4 Tph2=2 Tsjw=2)
#    define CAN_BT1_800K 0x00
#    define CAN_BT2_800K 0x2E
#    define CAN_BT3_800K 0x16
// 1000 kbit/s 83.33% 12 quanta (BRP=0 Tprs=7 Tph1=2 Tph2=2 Tsjw=2)
#    define CAN_BT1_1M 0x00
#    define CAN_BT2_1M 0x2C
#    define CAN_BT3_1M 0x12

#  elif AVRIO_CPU_FREQ == 16000000UL
// 10 kbit/s 68.00% 25 quanta (BRP=63 Tprs=8 Tph1=8 Tph2=8 Tsjw=2)
#    define CAN_BT1_10K 0x7E
#    define CAN_BT2_10K 0x2E
#    define CAN_BT3_10K 0x7E
// 20 kbit/s 87.50% 16 quanta (BRP=49 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_20K 0x62
#    define CAN_BT2_20K 0x2E
#    define CAN_BT3_20K 0x18
// 50 kbit/s 87.50% 16 quanta (BRP=19 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_50K 0x26
#    define CAN_BT2_50K 0x2E
#    define CAN_BT3_50K 0x18
// 100 kbit/s 87.50% 16 quanta (BRP=9 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_100K 0x12
#    define CAN_BT2_100K 0x2E
#    define CAN_BT3_100K 0x18
// 125 kbit/s 87.50% 16 quanta (BRP=7 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_125K 0x0E
#    define CAN_BT2_125K 0x2E
#    define CAN_BT3_125K 0x18
// 250 kbit/s 87.50% 16 quanta (BRP=3 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_250K 0x06
#    define CAN_BT2_250K 0x2E
#    define CAN_BT3_250K 0x18
// 500 kbit/s 87.50% 16 quanta (BRP=1 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_500K 0x02
#    define CAN_BT2_500K 0x2E
#    define CAN_BT3_500K 0x18
// 800 kbit/s 85.00% 20 quanta (BRP=0 Tprs=8 Tph1=8 Tph2=3 Tsjw=2)
#    define CAN_BT1_800K 0x00
#    define CAN_BT2_800K 0x2E
#    define CAN_BT3_800K 0x2E
// 1000 kbit/s 87.50% 16 quanta (BRP=0 Tprs=8 Tph1=5 Tph2=2 Tsjw=2)
#    define CAN_BT1_1M 0x00
#    define CAN_BT2_1M 0x2E
#    define CAN_BT3_1M 0x18
#  else
#    error Frequence AVRIO_CPU_FREQ non supportee !
#  endif

/* ========================================================================== */
#endif /* _AVRIO_CAN_PRIVATE_H_ */
