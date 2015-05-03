/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
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

/* *INDENT-OFF* */
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
/* *INDENT-ON* */
#endif  /* _AVRIO_ST7032_CTRL_H_ not defined */
