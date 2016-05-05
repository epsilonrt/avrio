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

