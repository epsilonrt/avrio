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
 * @file mmc.h
 * @brief Module d'accès aux cartes MMC/SD
 * Copyright © 2006 Jesper Hansen <jesper@redegg.net>
 *
 * Revision History ------------------------------------------------------------
 *    20120323 - Initial version from Michael Spiceland version
 *               (http://tinkerish.com)
 */
#ifndef _AVRIO_MMC_H_
#define _AVRIO_MMC_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup mmc_module Cartes mémoire MMC/SD
 *
 *  Ce module permet d'écrire et de lire des secteurs de 512 octets dans la
 *  mémoire FLASH d'une carte SD/MMC. \n
 *  Il peut être associé au projet <a href="http://ultra-embedded.com/?fat_filelib">fat filelib</a>
 *  afin d'implémenter un accès "fichiers" aux partitions FAT16 ou FAT32
 *  présentes sur la carte SD/MMC.
 *  @{
 * @example mmc/demo_mmc.c
 * Ce test écrit une suite d'octets de valeurs croissantes (0x00 à 0xFF) dans
 * le secteur de numéro TEST_SECTOR, puis relit ce secteur en vérifiant que
 * les données lues correpondent à celles écrites.
 * Si cela fonctionne, la LED1 clignote régulièrement, sinon elle flashe
 * rapidement.
 */

/* constants ================================================================ */
/**
 * @brief Taille d'un secteur MMC en octets
 *
 * Le mcu utilisé doit donc disposer d'au moins 1 ko de SRAM !
 */
#define MMC_SECTOR_SIZE 512

/**
 * @enum eMmcError
 * @brief Erreurs renvoyées par les fonctions du module MMC
 */
typedef enum {

  MMC_SUCCESS = 1,
  MMC_ERROR   = 0
} eMmcError;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module cartes MMC/SD.
 *
 * Initialise les broches de port nécessaires à l'interface MMC/SD et envoie
 * les commandes nécessaires à l'initialisation de la carte MMC/SD.
 * @warning La liaison SPI doit être initialisée au préalable à l'aide de
 * \ref vSpiMasterInit().
 * @return MMC_SUCCESS en cas de succès, MMC_ERROR sinon.
 */
int iMmcOpen (void);
void vMmcInit (void);
void vMmcClose (void);
bool xMmcIsOpen (void);
bool xMmcIsDetected (void);
bool xMmcIsReadOnly (void);

/**
 * @brief Ecriture de secteurs MMC/SD de 512 octets.
 *
 * Permet d'écrire des données dans la mémoire FLASH de la carte par secteurs.
 * @param ulSector Numéro du premier secteur où on doit écrire
 * @param pucBuffer Pointeur sur les octets à écrire
 * @param ulSectorCount Nombre de secteurs à écrire
 * @return MMC_SUCCESS en cas de succès, MMC_ERROR sinon.
 */
int iMmcWriteSector (uint32_t ulSector, uint8_t  * pucBuffer, uint32_t ulSectorCount);

/**
 * @brief Lecture de secteurs MMC/SD de 512 octets.
 *
 * Permet de lire des données dans la mémoire FLASH de la carte par secteurs.
 * @param ulSector Numéro du premier secteur où on doit lire
 * @param pucBuffer Pointeur sur le buffer où seront stockés les octets lus
 * @param ulSectorCount Nombre de secteurs à lire
 * @return MMC_SUCCESS en cas de succès, MMC_ERROR sinon.
 */
int iMmcReadSector (uint32_t ulSector, uint8_t  * pucBuffer, uint32_t ulSectorCount);

/**
 *   @}
 * @}
 */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_MMC_H_ not defined */
