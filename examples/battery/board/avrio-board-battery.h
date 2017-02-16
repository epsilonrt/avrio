/*
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
 */
#ifndef _AVRIO_BOARD_BATTERY_H_
#define _AVRIO_BOARD_BATTERY_H_

#include <avr/io.h>
#include <avrio/battery.h>

/* ADC pour la mesure de tension */
#define ADC_VREF  (2.56) /* AREF = Internal Voltage Ref. */

/* setup ==================================================================== */

/* Batterie */
#define BAT_CELLS 4
#define BAT_CELL_VOLTAGE_MIN       (0.8)
#define BAT_CELL_VOLTAGE_MAX       (1.8)
#define BAT_CELL_VOLTAGE_LOW       (1.1)
#define BAT_CELL_DV_MAX            (6E-3)

/* Mesure de tension batterie */
#define BAT         PF0
#define BAT_SCALE   ((604.0 + 200.0) / 200.0 * ADC_VREF)

/* Mesure de tension du bloc d'alim */
#define BAT_PSEC        PF1
#define BAT_PSEC_SCALE  ((1000.0 + 200.0) / 200.0 * ADC_VREF)
#define BAT_PSEC_VOLTAGE_MIN    (6.0)

/* Compteur de temps */
#define BAT_PROCESS_INTERVAL_MIN    10
#define BAT_COUNTER_PRE_CHARGE      12
#define BAT_COUNTER_FAST_CHARGE     9
#define BAT_COUNTER_TOPOFF_CHARGE   3

#define BAT_MONITOR_INTERVAL_MS     200
#define BAT_COUNTER_DVDT            150

#  ifndef __ASSEMBLER__
/* inline public functions ================================================== */
/* MAX1640 */
#define MAX1640_PORT  PORTE
#define MAX1640_DDR   DDRE
#define MAX1640_PIN   PINE
#define MAX1640_D0    PE3
#define MAX1640_D1    PE2
#define MAX1640_DMASK (_BV(MAX1640_D0)|_BV(MAX1640_D1))

// -----------------------------------------------------------------------------
static inline void 
vBatChargeInit (void) {

  MAX1640_PORT &= ~MAX1640_DMASK;
  MAX1640_DDR  |=  MAX1640_DMASK;
}

// -----------------------------------------------------------------------------
static inline void 
vBatChargeSet (eBatCharge eCharge) {

  MAX1640_PORT &= ~MAX1640_DMASK;
  MAX1640_PORT |= (((uint8_t)eCharge) << MAX1640_D1) & MAX1640_DMASK;
}

// -----------------------------------------------------------------------------
static inline eBatCharge 
eBatChargeGet (void) {
  
  return (eBatCharge) ((MAX1640_PIN & MAX1640_DMASK) >> MAX1640_D1);
}

#endif /* __ASSEMBLER__ not defined */
/* ========================================================================== */
#endif  /* _AVRIO_BOARD_BATTERY_H_ not defined */
