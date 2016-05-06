/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
 *
 * @file lcd_ctrl_pcf2119.h
 * @brief Définition pour le contrôleur LCD PCF2119
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_PCF2119_CTRL_H_
#define _AVRIO_PCF2119_CTRL_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* constants ================================================================ */

/* Adresse des registres */
#define PCF2119_IR  0       /* Registre d'instruction */
#define PCF2119_DR  1       /* Registre de donnée */

/* Bits de contrôle du registre d'instruction */
#define PCF2119_CLR   0x01  /* Commande Effacement */
#define PCF2119_HOME  0x02  /* Commande Retour HOME */

#define PCF2119_GMEM  0x40  /* Accès CGRAM */
#define PCF2119_DMEM  0x80  /* Accès DDRAM */

/* Bits d'état du registre d'instruction */
#define PCF2119_BF 0x80     /* BUSY Flag */

/* Réglage de la tension interne pour le mode caractère (mode normal):
 * V = N * 0.08 + 1.82 à 27 oC
 * N = 0 arrête le générateur
 * Vmin = 2.2 -> N = 5
 * Vmax = 6.5 -> N = 54
 * Plage optimale avec VDD 3.3, N de 32 à 48
 */ 
#define PCF2119_SETVA(__N) ((1<<7) | (__N))

/* Réglage de la tension interne pour le mode caractère (mode icône):
 * V = N * 0.08 + 1.82 à 27 oC
 * N = 0 arrête le générateur
 * Vmin = 2.2 -> N = 5
 * Vmax = 6.5 -> N = 54
 * Plage optimale avec VDD 3.3, N de 32 à 48
 */ 
#define PCF2119_SETVB(__N) ((1<<7) | (1<<6) | (__N))

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_PCF2119_CTRL_H_ not defined */
