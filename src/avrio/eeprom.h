/**
 * @file eeprom.h
 * @brief Gestion de la mémoire EEPROM.
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#ifndef _AVRIO_EEPROM_H_
#  define _AVRIO_EEPROM_H_

#  include <avr/eeprom.h>
#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup eeprom_module Mémoire EEPROM
 *
 *  Ce module permet de gérer le stockage en mémoire EEPROM de façon sécurisé.\n
 *  Au moment du stockage, un octet de CRC est ajouté à la suite des octets
 *  mémorisés. Au moment de la lecture, le CRC est vérifié et une erreur est
 *  renvoyée par iEepromLoadBlock().
 *  @{
 */
  /* internal public functions ============================================== */
/**
 * @brief Sauvegarde d'un bloc en EEPROM
 *
 * La sauvegarde est sécurisée par l'ajout d'un CRC 8 bits ajouté après le bloc
 * destination en EEPROM.
 *
 * @param pvSrcRam Adresse du 1er octet du bloc en mémoire vive à sauvegarder
 * @param pvDstEem Adresse du 1er octet du bloc de sauvegarde
 * @param xSize Nombre d'octets du bloc à sauvegarder
 * \note La taille réservée en EEPROM doit être de xSize + 1 (un octet de plus
 * pour le CRC)
 */
void vEepromSaveBlock (const void *pvSrcRam, void *pvDstEem, size_t xSize);

/**
 * @brief Sauvegarde d'un bloc en EEPROM
 *
 * La restauration est sécurisée par l'ajout d'un CRC 8 bits ajouté après le
 * bloc destination en EEPROM.
 *
 * @param pvDstRam Adresse du 1er octet du bloc en mémoire vive de restauration
 * @param pvSrcEem Adresse du 1er octet du bloc à restaurer
 * @param xSize Nombre d'octets du bloc à restaurer
 * @return 0 succès, -1 erreur de CRC
 * \note Le dernier octet du bloc source contient le CRC.
 */
int iEepromLoadBlock (void *pvDstRam, const void *pvSrcEem, size_t xSize);

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
#endif /* _AVRIO_EEPROM_H_ */
