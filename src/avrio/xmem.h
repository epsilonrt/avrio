/**
 * @file xmem.h
 * @brief Accès au mode étendu (bus d'adresses et de données)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120328 - Initial version by epsilonRT
 */
#ifndef _AVRIO_XMEM_H_
#define _AVRIO_XMEM_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup xmem_module Interface mémoire externe
 * 
 *  Ce module permet de gérer l'accès à l'interface mémoire externe disponible
 *  sur certains MCU comme l'ATmega128. \n
 *  Le module doit être configuré dans avrio-board-xmem.h. Aucune API n'est
 *  disponible car la configuration de l'interface est insérée dans le code
 *  de démarrage du programme (section .init3) 
 *  @author epsilonRT. Copyright © 2012 All rights reserved.
 *  @copyright GNU Lesser General Public License version 3
 *             <http://www.gnu.org/licenses/lgpl.html>
 *  @{
 */

/* constants ================================================================ */
/**
 * @enum eXmemSectorLimit
 * @brief Adresse de la limite entre les secteurs de "waitstate" haut et bas.
 *
 * Les temps d'attente (Wait States) lors de l'accès aux circuits connectés
 * à l'interface de mémoire externe sont définis pour deux zones mémoires qui 
 * sont délimitées par une une adresse fixée dans \c avrio-board-xmem.h à 
 * l'aide des constantes ci-dessous.
 */
typedef enum {

  XMEM_SECTOR_LIMIT_1100 = 0, /**< Limite de secteur à 0x1100 */
  XMEM_SECTOR_LIMIT_2000 = 1, /**< Limite de secteur à 0x2000 */
  XMEM_SECTOR_LIMIT_4000 = 2, /**< Limite de secteur à 0x4000 */
  XMEM_SECTOR_LIMIT_6000 = 3, /**< Limite de secteur à 0x6000 */
  XMEM_SECTOR_LIMIT_8000 = 4, /**< Limite de secteur à 0x8000 */
  XMEM_SECTOR_LIMIT_A000 = 5, /**< Limite de secteur à 0xA000 */
  XMEM_SECTOR_LIMIT_C000 = 6, /**< Limite de secteur à 0xC000 */
  XMEM_SECTOR_LIMIT_E000 = 7  /**< Limite de secteur à 0xE000 */
} eXmemSectorLimit;

/**
 * @enum eXmemBusFlags
 * @brief Drapeaux de configuration du bus de l'interface mémoire externe
 */
#define XMEM_BUS_WIDTH_A15 0 /**< Bus d'adresse A15:A0 */
#define XMEM_BUS_WIDTH_A14 1 /**< Bus d'adresse A14:A0 - PC7 disponible */
#define XMEM_BUS_WIDTH_A13 2 /**< Bus d'adresse A13:A0 - PC7:PC6 disponibles */
#define XMEM_BUS_WIDTH_A12 3 /**< Bus d'adresse A12:A0 - PC7:PC5 disponibles */
#define XMEM_BUS_WIDTH_A11 4 /**< Bus d'adresse A11:A0 - PC7:PC4 disponibles */
#define XMEM_BUS_WIDTH_A10 5 /**< Bus d'adresse A10:A0 - PC7:PC3 disponibles */
#define XMEM_BUS_WIDTH_A9  6 /**< Bus d'adresse A9:A0  - PC7:PC2 disponibles */
#define XMEM_BUS_WIDTH_A7  7 /**< Bus d'adresse A7:A0  - Port C disponible */
#define XMEM_BUS_PULLUP_RESISTOR (1<<6) /**< Validation des résistances de pullup sur AD0:7 */
#define XMEM_BUS_KEEPER          (1<<7) /**< Validation du BUSKEEPER sur AD0:7 */

/**
 * @def XMEM_RAM_BASE 
 * @brief Adresse de début de la mémoire SRAM externe 
 */

/**
 * @def XMEM_RAM_END 
 * @brief Adresse de fin de la mémoire SRAM externe
 */

/**
 *   @}
 * @}
 */
// -----------------------------------------------------------------------------
#if !defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =========================================================================
 */
#include "avrio-config.h"
#ifdef AVRIO_XMEM_ENABLE
# include "avrio-board-xmem.h"
#endif /* AVRIO_XMEM_ENABLE defined */
#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

#endif  /* _AVRIO_XMEM_H_ */
