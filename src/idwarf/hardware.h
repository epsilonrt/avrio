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
 * @file hardware.h
 * @brief Hardware Driver
 *
 * Revision History ------------------------------------------------------------
 *    20120227 - Initial version by epsilonRT
 */
#ifndef _IDWARF_HARDWARE_H_
#define _IDWARF_HARDWARE_H_

#include "itypes.h"

/*
  Ressources utilisées par le module pour mcu ATmega88/168/328:
  - TIMER0 
    pour les temporisations sous interruption de débordement avec prédivision par 64.
    Un préchargement de TCNT0 à 0x8D avec un quartz de 7372800 Hz donne une période
    de 1 ms.
  - SPI 
    en mode master FCLK = F_CPU/4, sans interruption.
  - WATCHDOG 
    en interruption pour le réveil du mode Power-Down (125 ms)
  - Broches utilisées
    - INT0
    - PC0   /PD
    - PC1   /RESET
    - PB2   /SS
    - PB3   MOSI
    - PB4   MISO
    - PB5   SCK
 */

/* internal public functions ================================================ */
void HwInit(void);
bool HwValidFlash(void);
void HwClearFlash (void);

/* ========================================================================== */
#endif  /* _IDWARF_HARDWARE_H_ not defined */
