/**
 * @file afsk.h
 * @brief Modem AFSK 1200 Bd
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140214 - Initial version from BeRTOS
 */
#ifndef _AVRIO_AFSK_H_
#define _AVRIO_AFSK_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>
#include <avrio/hdlc.h>

/* constants ================================================================ */

/**
 * @name Type de filtre utilisé pour la démodulation
 * @{
 */
#define AFSK_BUTTERWORTH  0
#define AFSK_CHEBYSHEV    1

/**
 * RX FIFO buffer full error.
 */
#define AFSK_RXFIFO_OVERRUN -2


#define AFSK_MODE_NOBLOCK 0x80

/* internal public functions ================================================ */

/**
 * Initialise le module Afsk
 */
void vAfskInit (int mode);

bool bAfskSending (void);

/* public variables ========================================================= */
/**
 * @brief Fichier permettant l'utilisation du modem avec les fonctions de la libc.
 *
 * Pour utiliser le modem comme entrée et sortie pour le module Ax25, on mettra
 * au début du main() :
 * @code
 * int main(void) {
 *   vAfskInit ();
 *   vAx25Init (&xAfskPort, vAx25RxCB);
 * ...
 * @endcode
 */
extern FILE xAfskPort;

/** @} */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

#endif /* _AVRIO_AFSK_H_ */
