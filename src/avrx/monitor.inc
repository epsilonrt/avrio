/**
 * @file monitor.inc
 * @brief Inclusion moniteur AvrX
 * @author Pascal Jean, epsilonRT. Copyright _ 2011 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110912 - Initial version by epsilonRT
 */
#include "avrx.inc"
#include "debug.inc"

/* constants ================================================================ */

#define MONITOR_VERSION "2.1"  

// Registers
#define Addressl        R4
#define Addressh        R5
#define EEAddressl      R6
#define EEAddressh      R7
#define ObjectIDl       R8
#define ObjectIDh       R9
#define DumpLength      R10
#define MonFlags        R11
#define TempWl          R12
#define TempWh          R13
#define TempB           R14

// MonFlags bits
#define WORDFLAG  0
#define EEFLAG    1
#define DUMPFLAG  2
/* ========================================================================== */
