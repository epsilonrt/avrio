/**
 * @file lcd_ctrl.h
 * @brief Contrôleur d'un afficheur LCD
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LCD_CTRL_H_
#define _AVRIO_LCD_CTRL_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
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
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_LCD_CTRL_H_ not defined */
