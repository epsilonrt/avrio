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
