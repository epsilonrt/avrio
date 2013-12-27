/*
 * @file avrio-board-xmem.h
 * @brief Configuration de l'interface mémoire externe
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120328 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_XMEM_H_
#  define _AVRIO_BOARD_XMEM_H_
/* ========================================================================== */

/* XMEM ===================================================================== */

/* constants ================================================================ */
/*
 * Adresse de la limite entre les secteurs haut et bas.
 * Doit être défini en utilisant les constantes eXmemSectorLimit
 */
#define XMEM_SECTOR_LIMIT XMEM_SECTOR_LIMIT_1100

/*
 * Nombres de temps d'attente en dessous de XMEM_SECTOR_LIMIT
 * La valeur est comprise entre 0 et 3. 
 * - 0 correspond à une durée du signal /WR ou /RD de 1 période d'horloge, 
 * - 1 correspond à 2 périodes (ajout d'un temps d'attente), 
 * - 2 correspond à 3 périodes (ajout de 2 temps d'attente),
 * - 3 correspond à une durée du signal /WR ou /RD de 3 périodes + une période 
 *   d'attente avant de déposer une nouvelle adresse sur le bus.
 */
#define XMEM_LOWER_WAIT_STATES 0

/*
 * Nombres de temps d'attente en dessous de XMEM_SECTOR_LIMIT
 * La valeur est comprise entre 0 et 3. 
 * - 0 correspond à une durée du signal /WR ou /RD de 1 période d'horloge, 
 * - 1 correspond à 2 périodes (ajout d'un temps d'attente), 
 * - 2 correspond à 3 périodes (ajout de 2 temps d'attente),
 * - 3 correspond à une durée du signal /WR ou /RD de 3 périodes + une période 
 *   d'attente avant de déposer une nouvelle adresse sur le bus.
 */
#define XMEM_UPPER_WAIT_STATES 1

/*
 * Drapeaux de configuration
 * Doit être défini en utilisant les constantes eXmemBusFlags
 */
#define XMEM_BUS_SETUP XMEM_BUS_WIDTH_A15
//#define XMEM_BUS_SETUP (XMEM_BUS_WIDTH_A15 + XMEM_BUS_KEEPER)

/*
 * Adresse de début de la mémoire SRAM externe 
 */
#define XMEM_RAM_BASE 0x1100  

/*
 * Taille de la mémoire SRAM externe 
 */
#define XMEM_RAM_END  0x7FFF  /* 28416 octets accessibles */

/* ========================================================================== */
#endif /* _AVRIO_BOARD_XMEM_H_ */
