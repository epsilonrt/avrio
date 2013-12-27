/**
 * @file halt.S
 * @brief vAvrXHalt
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_halt,"ax",@progbits
/* ========================================================================== */

  .text
/* -----------------------------------------------------------------------------
 * vAvrXHalt
 *
 * Locks up the system.
 *
 * PASSED:       
 * RETURNS:      
 * USES: 
 * CALLS:
 * ASSUMES:
 * NOTES:        
 */
  .global vAvrXHalt
  .func vAvrXHalt
// -----------------------------------------------------------------------------
vAvrXHalt:
    BeginCritical
    rjmp    vAvrXHalt

  .endfunc

/* ========================================================================== */
  .end

