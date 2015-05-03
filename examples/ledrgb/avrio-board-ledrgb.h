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
 * @file avrio-board-ledrgb.h
 * @brief Configuration module LED RGB
 * Copyright © 2012-2013 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20130129 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_LEDRGB_H_
#define _AVRIO_BOARD_LEDRGB_H_

#include <avrio/ledrgb_defs.h>

/* constants ================================================================ */
#define LEDRGB_LED_QUANTITY   16 ///< Nombre de LED RGB
#define LEDRGB_ALL_LEDS       0xFFFF
#define LEDRGB_NO_LED         0
#define LEDRGB_DEFAULT_MODE   MODE_BLINK
#define LEDRGB_CTRL           LEDRGB_CTRL_TLC59116 ///< Type de contrôleur

/* types ==================================================================== */
/*
 * Type utilisé pour les masques de LED RGB. 
 * Le nombre de bits doit être supérieur ou égal à LEDRGB_LED_QUANTITY
 */
typedef uint16_t xLedRgbMask;

/* Contrôleur TLC59116 (I2C) =============== */
/** Réglage du gain en courant à l'init. (c.f. p. 19 du datasheet) */
#define TLC59116_DEFAULT_IREF TLC59116_IREF(1,1,63) // CM=1, HC=1, D=63 

/** 
 * Listes d'adresse I2C des contrôleurs (1 liste par couleur)
 * Il doit y avoir un contrôleur TLC59116 de chaque couleur par paquet
 * de 16 leds.
 *                             TLC59116_ADDR(A3,A2,A1,A0) */
#define TLC59116_RED_LIST    { TLC59116_ADDR(0,0,0,0) }
#define TLC59116_GREEN_LIST  { TLC59116_ADDR(0,0,0,1) }
#define TLC59116_BLUE_LIST   { TLC59116_ADDR(0,0,1,0) }

/** Broche de RESET des contrôleurs, si non défini, un RESET SOFT est effectué */
//#define TLC59116_RESET 5
#define TLC59116_RESET_PORT PORTD
#define TLC59116_RESET_DDR  DDRD

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_LEDRGB_H_ not defined */
