/**
 * @file ledrgb_defs.h
 * @brief RGB
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20130129 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LEDRGB_DEFS_H_
#define _AVRIO_LEDRGB_DEFS_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS

/* constants ================================================================ */
#define LEDRGB_CTRL_TLC59116 1

#define TLC59116_ADDR_BASE   0xC0
#define TLC59116_ADDR_ALL    0xD0
#define TLC59116_ADDR_SUB1   0xD2
#define TLC59116_ADDR_SUB2   0xD4
#define TLC59116_ADDR_SWRST  0xD6
#define TLC59116_ADDR_SUB3   0xD8

#define TLC59116_ADDR(_a3,_a2,_a1,_a0) \
      ((TLC59116_ADDR_BASE) + ((_a3)*16)+((_a2)*8)+((_a1)*4)+((_a0)*2))

#define TLC59116_CM      7
#define TLC59116_HC      6
#define TLC59116_CC      0x3F
#define TLC59116_IREF(_cm,_hc,_cc) (((_cm)<<TLC59116_CM)+((_hc)<<TLC59116_HC)+((_cc)&TLC59116_CC))

#define TLC59116_SWRST_BYTE1  0xA5
#define TLC59116_SWRST_BYTE2  0x5A

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_LEDRGB_DEFS_H_ not defined */
