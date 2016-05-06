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
 * @file lcd_ctrl_st7032.h
 * @brief Définition pour le contrôleur LCD ST7032
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_ST7032_CTRL_H_
#define _AVRIO_ST7032_CTRL_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* constants ================================================================ */

/* Adresse des registres */
#define ST7032_IR  0       /* Registre d'instruction */
#define ST7032_DR  1       /* Registre de donnée */

/* Bits de contrôle du registre d'instruction */
#define ST7032_CLR   0x01  /* Commande Effacement */
#define ST7032_HOME  0x02  /* Commande Retour HOME */

#define ST7032_GMEM  0x40  /* Accès CGRAM */
#define ST7032_DMEM  0x80  /* Accès DDRAM */

/* Bits d'état du registre d'instruction */
#define ST7032_BF 0x80     /* BUSY Flag */

/* Commandes en mode étendu */
/* Bits */
#define ST7032_OSC    0x10  /* Réglage Bias et Oscillateur */
#define ST7032_IADDR  0x40  /* Modification adresse ICON */
#define ST7032_POWER  0x50  /* Mise en fonction Booster, Icon, et poids fort contraste */
#define ST7032_FOLOW  0x60  /* Réglage alim interne */
#define ST7032_CONTR  0x70  /* Reglage 4 bits poids faible contraste */

/* Macros */
/* Réglage Bias et Oscillateur */
#define ST7032_SETOSC(__BS, __F210) \
  ( ST7032_OSC   | (((__BS)&1)<<3)  | ((__F210)&7) )
/* Mise en fonction Booster, Icon, et 2 bits poids fort contraste */
#define ST7032_SETPOWER(__ION,__BON,__C54) \
  ( ST7032_POWER | (((__ION)&1)<<3) | (((__BON)&1)<<2) | ((__C54)&3) ) 
/* Réglage alim interne (facteur amplification de Vref) */
#define ST7032_SETFOLOW(__FON,__RAB210) \
  ( ST7032_FOLOW | (((__FON)&1)<<3) | ((__RAB210)&7) )
/* Reglage 4 bits poids faible contraste */
#define ST7032_SETCONTR(__C3210) \
  ( ST7032_CONTR | ((__C3210)&0x0F) )
/* Modification adresse ICON */
#define ST7032_SETIADDR(_AC3210) \
  ( ST7032_IADDR | ((_AC3210)&0x0F) )

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_ST7032_CTRL_H_ not defined */
