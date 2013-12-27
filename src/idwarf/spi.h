/**
 * @file spi.h
 * @brief SPI Driver
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120227 - Initial version by epsilonRT
 */
#ifndef _IDWARF_SPI_H_
#define _IDWARF_SPI_H_

#include "itypes.h"

/* internal public functions ================================================ */
U8 SPI_MasterReceive(void);
void SPI_MasterTransmit(U8);

/* ========================================================================== */
#endif  /* _IDWARF_SPI_H_ not defined */
