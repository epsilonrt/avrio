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
#endif  /* _AVRIO_LCD_IO_H_ not defined */
