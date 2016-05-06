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
 * @file lcd_backlight.h
 * @brief Rétroéclairage d'afficheur LCD
 */
#ifndef _AVRIO_LCD_BACKLIGHT_H_
#  define _AVRIO_LCD_BACKLIGHT_H_ 1
#  include <avrio/defs.h>

__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup lcd_module
 * @{
 *
 *  @defgroup lcd_backlight_module Rétroéclairage
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
#endif /* _AVRIO_LCD_BACKLIGHT_H_ */
