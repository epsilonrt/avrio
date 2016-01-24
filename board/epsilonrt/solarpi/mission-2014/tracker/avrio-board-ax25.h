/*
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
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
