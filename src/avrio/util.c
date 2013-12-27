/**
 * @file util.c
 * @brief Module utilitaire - Implémentation C
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120317 - Initial version by epsilonRT
 */

/* ========================================================================== */
#include <avrio/util.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint8_t 
ucUtilReverseBits (uint8_t ucByte) {
    uint8_t ucCount = sizeof(ucByte) * 8 - 1;
    uint8_t ucResult = ucByte;
 
    ucByte >>= 1;
    while(ucByte) {
    
       ucResult <<= 1;
       ucResult |= ucByte & 1;
       ucByte >>= 1;
       ucCount--;
    }
    ucResult <<= ucCount;
    return ucResult;
}
/* ========================================================================== */
