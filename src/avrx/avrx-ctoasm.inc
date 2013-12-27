/**
 * @file avrx-ctoasm.inc
 * @brief Définitions des registres utilisés par GNU GCC
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 * @author Pascal Jean, epsilonRT. Copyright © 2011 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#ifndef _AVRX_CTOASM_H_
#define _AVRX_CTOASM_H_
/* ========================================================================== */

#  define p1l     R24 /* LSB Paramètre N°1 de la fonction */
#  define p1h     R25 /* MSB Paramètre N°1 de la fonction */
#  define p2l     R22 /* LSB Paramètre N°2 de la fonction */
#  define p2h     R23 /* MSB Paramètre N°2 de la fonction */
#  define r1l     R24 /* LSB Valeur retournée de la fonction */
#  define r1h     R25 /* MSB Valeur retournée de la fonction */

#  define tmp0    R18
#  define tmp1    R19
#  define tmp2    R20
#  define tmp3    R21
#  define tmp4    R26   /* X */
#  define tmp5    R27   /* X */

/* ========================================================================== */
#endif  /* _AVRX_CTOASM_H_ not defined */
