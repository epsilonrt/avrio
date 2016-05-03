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
 * @file util.S
 * @brief Implémentation utilitaires (assembleur)
 *
 * Revision History ---
 *    20111113 - Initial version by epsilonRT
 */

/* registers ================================================================ */
#define tmp 24
#define random 25

/* internal public functions ================================================ */

/* uint8_t rand8(void); */
// -----------------------------------------------------------------------------
.section .data,"aw",@progbits
seed:  .byte  0xFF

.section .text,"ax",@progbits
.global rand8
.type rand8,@function

rand8:
  ldi r31, hi8(seed)
  ldi r30, lo8(seed)
  ld random, Z
  mov tmp,random
  lsl tmp
  swap tmp
  andi tmp,0xE0

  add random,tmp
  add random,tmp
  add random,tmp
  ldi tmp,0x35
  sub random,tmp
  mov tmp,random
  st Z, random
  ret

.size rand8, . - rand8 + 1

/* void srand8 (uint8_t seed); */
// -----------------------------------------------------------------------------
.section .text,"ax",@progbits
.global srand8
.type srand8,@function

srand8:
  ldi r31, hi8(seed)
  ldi r30, lo8(seed)
  st  Z, r24  
  ret

.size srand8, . - srand8 + 1

/* ========================================================================== */

