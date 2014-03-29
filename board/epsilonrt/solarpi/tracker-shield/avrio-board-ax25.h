/**
 * @file avrio-board-ax25.h
 * @brief Fichier de configuration du module Ax25
 * @author Francesco Sacchi <batt@develer.com>
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 */
#ifndef _AVRIO_CFG_AX25_H_
#define _AVRIO_CFG_AX25_H_

#include <avrio/led.h>

/* public constants ========================================================= */

/*
 * Configuration de la taille du buffer de réception utilisé par le module.
 * La taille doit être suffisante pour contenir une trame complète (sans les
 * flags HDLC).
 */
#define CONFIG_AX25_FRAME_BUF_LEN 330

/*
 * Valide la prise en charge des répéteurs dans les trames, cela utilise 56
 * octets supplémentaire par trame.
 */
#define CONFIG_AX25_RPT_LST 1

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
// External
INLINE void
vAx25HwFrmLedOn (void) {

  // Mettre votre code ici si nécessaire
  vLedSet (LED_RX);
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAx25HwFrmLedOff (void) {

  // Mettre votre code ici si nécessaire
  vLedClear (LED_RX);
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAx25HwInit (void) {

  // Mettre votre code ici si nécessaire
  // Leds initialisées par Afsk
}

#endif /* _AVRIO_CFG_AX25_H_ */
