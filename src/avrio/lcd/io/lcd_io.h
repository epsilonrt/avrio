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
 * @file lcd_io.h
 * @brief Couche Interface d'un afficheur LCD
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LCD_IO_H_
#define _AVRIO_LCD_IO_H_
#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* internal public functions ================================================ */
/**
 * @addtogroup lcd_module
 * @{
 *
 *  @defgroup lcd_io Interface matérielle
 *
 *  Ce module décrit les fonctions d'accès à l'interface matérielle du LCD.
 *  @{
 */

/**
 * @brief Initialise de l'interface de le LCD
 * @return 0 succès, une valeur négative si erreur.
 */
int8_t iLcdIoInit (void);

/**
 * @brief Ecriture d'un octet vers le LCD
 * @param ucData Valeur de l'octet
 * @param ucAddress Adresse où l'octet sera écrit.
 */
void vLcdIoWrite (uint8_t ucData, uint8_t ucAddress);

/** 
 * @brief Ecriture d'un quartet B7-B4 vers le LCD
 * @param ucHighNibble quartet à écrire aligné à gauche (B3-B0 = 0)
 * @param ucAddress Adresse du registre
 * @note Nécessaire uniquement pour les contrôleurs d'afficheur disposant d'un
 * mode 4 bits.
 */
void vLcdIoWriteNibble (uint8_t ucHighNibble, uint8_t ucAddress);

/**
 * @brief Lecture d'un octet dans le LCD
 * @param ucAddress Adresse où l'octet sera écrit.
 * @return Valeur de l'octet
 */
uint8_t ucLcdRead (uint8_t ucAddress);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_LCD_IO_H_ not defined */
