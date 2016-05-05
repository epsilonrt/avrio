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
 * @file lcd_ctrl_hd44780.h
 * @brief Définition pour le contrôleur LCD HD44780
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_HD44780_CTRL_H_
#define _AVRIO_HD44780_CTRL_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* constants ================================================================ */
/* Adresse des registres */
#define HD44780_IR  0       /* Registre d'instruction */
#define HD44780_DR  1       /* Registre de donnée */

/* Bits de contrôle du registre d'instruction */
#define HD44780_CLR   0x01  /* Commande Effacement */
#define HD44780_HOME  0x02  /* Commande Retour HOME */

#define HD44780_GMEM  0x40  /* Accès CGRAM */
#define HD44780_DMEM  0x80  /* Accès DDRAM */

/* Bits d'état du registre d'instruction */
#define HD44780_BF 0x80     /* BUSY Flag */

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_HD44780_CTRL_H_ not defined */
