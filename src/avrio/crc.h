/**
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 *
 * @file crc.h
 * @brief Calcul de CRC (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version from BeRTOS
 */

#ifndef _AVRIO_CRC_H_
#define _AVRIO_CRC_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup math_group
 * @{
 *
 *  @defgroup crc_module CRC
 *
 *  Calcul de CRC
 *  @{
 */

/* constants ================================================================ */
/**
 * Valeur initiale d'un CRC CCITT 16-bit
 */
#define CRC_CCITT_INIT_VAL ((uint16_t)0xFFFF)

/**
 * Valeur initiale d'un CRC iButton 8-bit
 */
#define CRC_IBUTTON_INIT_VAL ((uint8_t)0x5A)

/* internal public functions ================================================ */

/**
 * @brief Calcul du CRC iButton (8-bits)
 *
 * @param ucCrc CRC initial ou précédent
 * @param pvBuf Buffer contenant les octets dont on va calculer le CRC
 * @param uLen Nombre d'octets présents dans le buffer
 * @return la valeur du CRC iButton calculé
 */
uint8_t ucCrcIButton (uint8_t ucCrc, const void * pvBuf, size_t uLen);

/**
 * @brief Calcul du CRC CCITT (16-bits)
 *
 * @param usCrc CRC initial ou précédent
 * @param pvBuf Buffer contenant les octets dont on va calculer le CRC
 * @param uLen Nombre d'octets présents dans le buffer
 * @return la valeur du CRC CCITT calculé
 */
uint16_t usCrcCcitt (uint16_t usCrc, const void *pvBuf, size_t uLen);


#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Mise à jour du CRC CCITT avec un octet
 * @param ucByte Nouvel octet
 * @param usCrc CRC initial ou précédent
 * @return la valeur du CRC CCITT calculé
 */
static inline uint16_t usCrcCcittUpdate (uint8_t ucByte, uint16_t usCrc);

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

#include <avr/pgmspace.h>

/* CRC table */
extern const uint16_t usCrcCcittTab[256];

/* inline public functions ================================================ */
INLINE uint16_t
usCrcCcittUpdate (uint8_t c, uint16_t oldcrc) {

  return (oldcrc >> 8) ^ pgm_read_word (&usCrcCcittTab[ (oldcrc ^ c) & 0xff]);
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_CRC_H_ */
