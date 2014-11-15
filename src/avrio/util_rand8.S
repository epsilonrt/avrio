/**
 * @file util.S
 * @brief Implémentation utilitaires (assembleur)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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

