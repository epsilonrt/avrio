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
 * @file lcd.h
 * @brief Afficheur LCD
 *
 * @todo Réorganiser le module lcd de façon à unifier les fonctions textes et
 * les fonctions d'appel au driver entre les 2 implémentations alpha/graph.
 * @todo Revoir la doc en conséquence.
 */
#ifndef _AVRIO_LCD_H_
#define _AVRIO_LCD_H_ 1

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
#include <stdio.h>
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup lcd_module LCD alphanumérique
 *
 *  Ce module permet de gérer un afficheur LCD alphanumérique. \n
 *  Le développement de ce module a été fait de façon modulaire en séparant
 *  l'interface vers l'afficheur et le contrôleur graphique utilisé par
 *  l'afficheur (les 2 objets, interface et contrôleur sont complétement
 *  indépendants). \n
 *  Les contrôleurs graphiques pris en charge sont :
 *  - HD44780
 *  - PCF2119
 *  - ST7032
 *  .
 *  Les interfaces prises en charge sont :
 *  - Port parallèle AVR, données sur 4 bits
 *  - Port parallèle AVR, données sur 8 bits
 *  - Interface I2C avec PCF8574 relie à un afficheur parallèle (4 bits)
 *  - Interface I2C standard (Octet de contrôle RS, CO)
 *  .
 *  Le module peut être configuré facilement grâce au fichier avrio-board-lcd.h
 *  et peut evoluer aussi facilement grâce à sa stucture clairement découpée.
 *  @{
 * @example lcd/demo_lcd.c
 * Affiche un compte à rebourd sur l'afficheur LCD.
 */
#include "lcd/lcd_defs.h"
#include "lcd/io/lcd_io.h"
#include "lcd/ctrl/lcd_ctrl.h"

/* internal public functions ================================================ */
/**
 * @brief Initialise le module LCD
 *
 * Initialise l'ensemble des éléments nécessaires au fonctionnement de
 * l'afficheur, règle le contraste à mi-course (16) et efface l'écran.
 * @return 0 succès, une valeur négative si erreur.
 */
int8_t iLcdInit (void);

/**
 * @brief Affiche un caractère sur le LCD
 *
 * Cette fonction prend en charge les caractères CR/LF et les sauts de lignes
 * automatiques
 * @param cChar caractère ASCII (code étendu non pris en charge)
 * \note Pour les affichages complexes, il peut être plus interessant d'utiliser
 * les fonctions de la libc (putc, puts, printf, fprintf ... )
 */
void vLcdPutChar (char cChar);

/**
 * @brief Affiche une chaîne caractères sur le LCD
 *
 * @param pcString chaîne caractères
 * \note Pour les affichages complexes, il peut être plus interessant d'utiliser
 * les fonctions de la libc (puts, printf, fprintf ... )
 */
void vLcdPutString (const char *pcString);

/**
 * @brief Efface les caractères
 *
 * @param xX colonne du premier caractère
 * @param xY ligne du premier caractère
 * @param uLen de caractères à effacer
 * \note Après l'effacement le curseur est repositionner en (xX, xY)
 */
void vLcdClearChars (xLcdCoord xX, xLcdCoord xY, uint8_t uLen);

/**
 * @brief Changement de la position courante
 *
 * @param xX valeur de la colonne (0 pour la colonne de gauche)
 * @param xY valeur de la ligne (0 pour la ligne du haut)
 */
void vLcdGotoXY (xLcdCoord xX, xLcdCoord xY);

/**
 * @brief Initialise un bargraph à la position courante
 * @param pucBarPatterns Pointeur sur un tableau de patterns en mémoire FLASH
 * permettant d'afficher un bloc de largeur 0 à LCD_PATTERN_WIDTH+1
 */
void vLcdBargraphInit (const uint8_t * pucBarPatterns);

/**
 * @brief Affiche ou mets à jour un bargraph
 * @param uValue Valeur à afficher entre 0 et \c uMax
 * @param uMax Valeur max de la barre correspondant à 100 %
 * @param uWidth Longueur de la barre en nombre de caractères
 * @note La fonction vLcdCtrlSetIcon() doit être prise en charge par le contrôleur
 * du LCD.
 */
void vLcdBargraph (uint16_t uValue, uint16_t uMax, xLcdCoord uWidth);

/* public variables ========================================================= */
/**
 * @brief Descripteur de fichier du LCD permettant son utilisation avec les fonctions de la libc.
 *
 * Pour utiliser le LCD comme sortie par défaut, on mettra au début
 * du main() :
 * @code
 * int main(void) {
 *   iLcdInit ();
 *   stdout = &xLcd;
  * ...
 * @endcode
 * On pourra dans le cas contraire passer &xLcd à la fonction fprintf :
 * @code
 * int choix;
 * ...
 * fprintf (&xLcd, "Choix ? ");
 * ...
 * @endcode
 */
extern FILE xLcd;

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 * @brief Réglage du constraste de l'afficheur
 * @param ucContrast Le contraste entre 0 et 31
 * @return La valeur effectivement réglée
 */
uint8_t ucLcdContrastSet (uint8_t ucContrast);

/**
 * @brief Réglage de la luminosité du rétro-éclairage
 * @param ucLevel La luminosité entre 0 et 63
 * @return La valeur effectivement réglée
 */
uint8_t ucLcdBacklightSet (uint8_t ucLevel);

/**
 * @brief Effacement de l'afficheur et retour du curseur à l'origine.
 */
void vLcdClear (void);

/**
 * @brief Modifie l'affichage du curseur
 * @param eCursor Type de curseur à afficher
 * @note Cette fonction ne fait rien si l'afficheur ne dispose pas de cette
 * fonctionnalité.
 */
void vLcdEnableCursor (eLcdCursor eCursor);

/**
 * @brief Lecture de la colonne du curseur
 */
xLcdCoord xLcdGetX (void);

/**
 * @brief Nombre de colonnes du LCD
 */
xLcdCoord xLcdWidth (void);

/**
 * Lecture de la colonne du curseur
 */
xLcdCoord xLcdGetY (void);

/**
 * @brief Nombre de lignes
 */
xLcdCoord xLcdHeight (void);

/**
 * @brief Efface seulement la ligne du curseur
 */
void vLcdClearCurrentLine (void);

/**
 * @brief Efface une ligne
 */
void vLcdClearLine (xLcdCoord xY);

/**
 * @brief Chargement d'un caractère personnalisé en CGRAM
 *
 * @param ucCode Code du caractère en CGRAM
 * @param pucIconPattern Pointeur sur la matrice des points du caractère en mémoire FLASH
 * @note Le contrôleur du LCD doit prendre en charge cette fonctionnalité.
 */
void vLcdSetIcon (uint8_t ucCode, const uint8_t * pucIconPattern);

/**
 *   @}
 * @}
 */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
/* ========================================================================== */
#include "avrio-board-lcd.h"

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (uint8_t
  ucLcdContrastSet (uint8_t ucContrast)) {

    return ucLcdCtrlContrastSet (ucContrast);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void
  vLcdClear (void)) {

    vLcdCtrlClear();
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void
  vLcdEnableCursor (eLcdCursor eCursor)) {

    vLcdCtrlEnableCursor (eCursor);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (xLcdCoord
  xLcdGetX (void)) {

    return xLcdCtrlGetX();
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (xLcdCoord
  xLcdGetY (void)) {

    return xLcdCtrlGetY();
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (xLcdCoord
  xLcdWidth (void)) {

    return (xLcdCoord)LCD_WIDTH;
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (xLcdCoord
  xLcdHeight (void)) {

    return (xLcdCoord)LCD_HEIGHT;
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void
  vLcdClearCurrentLine (void)) {

    vLcdClearChars (0, xLcdCtrlGetY(), LCD_WIDTH);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void
  vLcdClearLine (xLcdCoord xY)) {

    vLcdClearChars (0, xY, LCD_WIDTH);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void
  vLcdSetIcon (uint8_t ucIndex, const uint8_t * pucIconPattern)) {

    vLcdCtrlSetIcon (ucIndex, pucIconPattern);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (uint8_t
  ucLcdBacklightSet (uint8_t ucLevel)) {

#ifdef LCD_CTRL_BACKLIGHT_ENABLE
    return ucLcdCtrlBacklightSet (ucLevel);
#else
    return 0;
#endif
  }

/* ========================================================================== */
#endif /* AVRIO_LCD_ENABLE defined */
#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_LCD_H_ */
