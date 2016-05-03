/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file ledrgb.h
 * @brief LED RGB
 *
 * Revision History ------------------------------------------------------------
 *    20130129 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LEDRGB_H_
#define _AVRIO_LEDRGB_H_

#include <avrio/defs.h>
#include <avrio/rgb.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include "avrio-config.h"
#ifdef AVRIO_LEDRGB_ENABLE
#include "avrio-board-ledrgb.h"

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup ledrgb_module LED RGB

 *  Ce module permet de gérer les leds multicolores (Rouge, Verte, Bleue) de
 *  façons indépendantes de la plate-forme matérielle. \n
 *  Son implémentation fait appel à un contrôleur spécialisé (TLC59116...). \n
 *  La macro AVRIO_LEDRGB_ENABLE doit être définie dans avrio-config.h ou dans
 *  le makefile et dans ce cas, la partie interface matérielle doit être
 *  configurée dans le fichier avrio-board-ledrgb.h
 *  @{
 *  @example ledrgb/demo_ledrgb.c
 *  Modification des couleurs, de la luminosité et clignotement de leds RGB.
 */

/* constants ================================================================ */
/**
 * @enum eLedRgbMode
 * Mode d'allumage d'une LED RGB
 */
typedef enum {
  MODE_OFF = 0,     ///< Led éteinte
  MODE_ON = 1,      ///< Led allumée sans variation d'intensité
  MODE_BRIGHT = 2,  ///< Led avec variation d'intensité
  MODE_BLINK = 3    ///< Led avec variation d'intensité et clignotement
  } eLedRgbMode;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module
 *
 * Après un initialisation du contrôleur, un test de présence est réalisé avant
 * avant de configurer toutes les leds en mode variation d'intensité et
 * clignotement (MODE_BLINK)
 * @return 0 en cas de succès. Une valeur négative en cas d'erreur.
 */
int8_t iLedRgbInit (void);

/**
 * @brief Modifie la couleur d'une ou plusieurs leds
 *
 * @param xLed Masque des leds à modifier. Un bit 1 indique que l'on souhaite
 * modifier la led correspondante. Un bit 0 indique que la led correspondante
 * restera inchangée.
 * @param ulColor Nouvelle couleur
 */
void vLedRgbSetColor (xLedRgbMask xLed, uint32_t ulColor);

/**
 * @brief Modifie le mode d'allumage d'une ou plusieurs leds
 *
 * @param xLed Masque des leds à modifier. Un bit 1 indique que l'on souhaite
 * modifier la led correspondante. Un bit 0 indique que la led correspondante
 * restera inchangée.
 * @param eMode Nouveau mode
 */
void vLedRgbSetMode  (xLedRgbMask xLed, eLedRgbMode eMode);

/**
 * @brief Modifie la luminosité globale de toutes les leds configurées en mode MODE_BLINK
 *
 * Seules les LEDS en mode MODE_BLINK sont affectées.
 * A l'initialisation la luminosité globale est à sa valeur maximale (255).
 * @param ucDimming Nouvelle luminosité entre 0 et 255
 */
void vLedRgbSetGlobalDimming  (uint8_t ucDimming);

/**
 * @brief Fait clignoter toutes les leds configurées en mode MODE_BLINK
 *
 * Seules les LEDS en mode MODE_BLINK sont affectées. Pour mettre fin au
 * clignotement, il faut faire un appel à vLedRgbSetGlobalDimming()
 * @param usPeriod Période en millisecondes. La valeur minimale et maximale
 * dépend du contrôleur utilisé.
 * @param ucDutyCycle Rapport cyclique entre 0 et 255
 */
void vLedRgbSetGlobalBlinking (uint16_t usPeriod, uint8_t ucDutyCycle);

/**
 * @brief Renvoie les erreurs des leds
 *
 * En fonction du contrôleur utilisé, celui-ci peut effectuer une surveillance
 * des leds (led en circuit ouvert, surchauffe ...)
 * @return les bits d'erreurs pour chaque led multicolore. 1 indique une erreur.
 * Si non implémenté par le contrôleur renvoie toujours 0.
 */
xLedRgbMask xLedRgbError (void);

void vLedRgbClearError (void);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/* types ==================================================================== */
/**
 * @typedef xLedRgbMask
 * @brief Type utilisé pour les masques de LED RGB.
 *
 * Le nombre de bits doit être supérieur ou égal au nombre de LED défini dans
 * avrio-board-ledrgb.h (LEDRGB_LED_QUANTITY)
 * Le type réel n'est pas forcément uint16_t. Il doit être défini dans
 * avrio-board-ledrgb.h
 */
typedef uint16_t xLedRgbMask;

/**
 * @def LEDRGB_ALL_LEDS
 * @brief Masque de sélection pour toutes les leds
 *
 * Doit être défini dans avrio-board-ledrgb.h
 */
#define LEDRGB_ALL_LEDS

/**
 * @def LEDRGB_NO_LED
 * @brief Masque de sélection pour aucune led
 *
 * Doit être défini dans avrio-board-ledrgb.h
 */
#define LEDRGB_NO_LED

/**
 *   @}
 * @}
 */
#endif /* __DOXYGEN__ defined */

#endif /* AVRIO_LEDRGB_ENABLE defined */
__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_LEDRGB_H_ not defined */
