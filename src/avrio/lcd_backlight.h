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
 * @file lcd_backlight.h
 * @brief Rétroéclairage d'afficheur LCD
 */
#ifndef _AVRIO_LCD_BACKLIGHT_H_
#  define _AVRIO_LCD_BACKLIGHT_H_ 1
#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup lcd_module
 * @{
 *
 *  @defgroup lcd_backlight_module Rétroéclairage d'afficheur LCD
 *
 *  Ce module permet de gérer le rétroéclairage d'un afficheur LCD qu'il soit
 *  alphanumérique ou graphique.\n
 *  Le module utilise un timer en mode PWM pour faire varier l'intensité du
 *  rétroéclairage.\n
 *  La macro AVRIO_LCD_BACKLIGHT_ENABLE dans avrio-config.h
 *  et dans ce cas, la partie interface matérielle doit être implémentée dans le
 *  fichier avrio-board-lcdbacklight.h .\n
 *  @{
 */
  /* internal public functions ============================================== */
/**
 * @brief Initialise la commande de rétroéclairage de l'afficheur.
 */
void vLcdCtrlBacklightInit (void);

/**
 * @brief Modifie l'intensité de l'allumage du rétroéclairage
 * @param iPercent pourcentage d'éclairage de 0 à 100
 * @return la valeur effectivement réglée (bornée entre 0 et 100)
 */
int8_t iLcdBacklightSet (int8_t iPercent);

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

  /**
   *   @}
   * @}
   */
#  else
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_LCD_BACKLIGHT_H_ */
