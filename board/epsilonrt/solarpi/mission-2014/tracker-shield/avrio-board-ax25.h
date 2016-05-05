/*
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
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
