/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
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
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAx25HwFrmLedOff (void) {

  // Mettre votre code ici si nécessaire
}

// -----------------------------------------------------------------------------
// External
INLINE void
vAx25HwInit (void) {

  // Mettre votre code ici si nécessaire
  // Leds initialisées par Afsk
}

#endif /* _AVRIO_CFG_AX25_H_ */
