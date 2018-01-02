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
 * @file lcd_ctrl.h
 * @brief Contrôleur d'un afficheur LCD
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LCD_CTRL_H_
#define _AVRIO_LCD_CTRL_H_

#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup lcd_module
 * @{
 *
 *  @defgroup lcd_ctrl Contrôleur graphique
 *
 *  Ce module décrit les fonctions d'accès au contrôleur graphique du LCD.
 *  @{
 */

/* internal public functions ================================================ */
/**
 * @brief Initialise l'interface matérielle et l'afficheur.
 * @return 0 succès, une valeur négative si erreur.
 */
int8_t iLcdCtrlInit (void);

/**
 * @brief Réglage du constraste de l'afficheur 
 * @param ucContrast Le contraste entre 0 et 31
 * @return L'ancienne valeur de constraste
 */
uint8_t ucLcdCtrlContrastSet (uint8_t ucContrast);

/**
 * @brief Réglage de la luminosité du rétro-éclairage
 * @param ucLevel La luminosité
 * @return L'ancienne luminosité
 */
uint8_t ucLcdCtrlBacklightSet (uint8_t ucLevel);

/**
 *  @brief Efface l'écran de l'afficheur.
 */
void vLcdCtrlClear (void);

/**
 *  @brief Affiche un caractère sur l'afficheur à la position courante
 */
void vLcdCtrlPutChar (char cChar);

/**
 *  @brief Envoie une commande à l'afficheur
 */
void vLcdCtrlPutCmd (char cCmd);

/**
 *  @brief Envoie une commande étendue à l'afficheur
 */
void vLcdCtrlPutExtCmd (char cCmd);

/**
 * @brief Modifie l'affichage du curseur
 * @param eCursor Type de curseur à afficher
 * @note Cette fonction ne fait rien si l'afficheur ne dispose pas de cette 
 * fonctionnalité.
 */
void vLcdCtrlEnableCursor (eLcdCursor eCursor);

/**
 *  @brief Modifie l'abscisse de la position courante
 */
void vLcdCtrlSetX (xLcdCoord xX);

/**
 *  @brief Modifie l'ordonnée de la position courante
 */
void vLcdCtrlSetY (xLcdCoord xY);

/**
 *  @brief Lecture de l'abscisse de la position courante
 */
xLcdCoord xLcdCtrlGetX (void);

/**
 *  @brief Lecture de l'ordonnée de la position courante
 */
xLcdCoord xLcdCtrlGetY (void);

/**
 * @brief Chargement d'un caractère personnalisé en CGRAM
 *
 * @param ucIndex Index du caractère en CGRAM
 * @param pucIconPattern Pointeur sur la matrice des points du caractère en mémoire FLASH
 * @note Le contrôleur du LCD doit prendre en charge cette fonctionnalité.
 */
void vLcdCtrlSetIcon (uint8_t ucIndex, const uint8_t * pucIconPattern);

/**
 * @brief Initialise le contrôle de rétro-éclairage
 */
void vLcdCtrlBacklightInit(void);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif  /* _AVRIO_LCD_CTRL_H_ not defined */
