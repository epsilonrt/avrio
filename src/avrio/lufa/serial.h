/**
 * @file serial.h
 * @brief CDC Virtual Serial
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20121226 - Initial version by epsilonRT
 */
#ifndef _MSL_SERIAL_H_
#define _MSL_SERIAL_H_
#include <avrio/defs.h>
#include <stdio.h>

/* public variables ========================================================= */
extern FILE xUsbSerial;

/* internal public functions ================================================ */
void vSerialInit (void);
uint16_t usSerialHit (void);
void vSerialUsbTask (void);
#endif  /* _MSL_SERIAL_H_ not defined */
/* ========================================================================== */
