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
 * @file 
 * @brief Définition pour le contrôleur LCD THMI
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_THMI_CTRL_H_
#define _AVRIO_THMI_CTRL_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* constants ================================================================ */
/* Adresse des registres */
#define THMI_IR  0       /* Registre d'instruction */
#define THMI_DR  1       /* Registre de donnée */

/* Bits de contrôle du registre d'instruction */
#define THMI_CLR   0x01  /* Commande Effacement */
#define THMI_HOME  0x02  /* Commande Retour HOME */

#define THMI_GMEM  0x40  /* Accès CGRAM */
#define THMI_DMEM  0x80  /* Accès DDRAM */

/* Reglage contraste et mise en fonction alimentation */
#define THMI_SETCONTR(__BON,__C43210) \
  ( THMI_CSET | ((__BON) & THMI_BON) | (__C43210) ) 
#define THMI_SETBL(__B543210) \
  ( THMI_BSET | (__B543210) ) 

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_THMI_CTRL_H_ not defined */
