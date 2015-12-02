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
 * @file eefile.h
 * @brief Fichier en mémoire EEPROM.
 */
#ifndef _AVRIO_EEFILE_H_
#define _AVRIO_EEFILE_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>
#include <avrio/eeprom.h>
#include <avrio/file.h>

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup eefile_module Fichier en EEPROM
 *
 *  Ce module permet le stockage de fichiers en EEPROM de façon sécurisé.\n
 *  @{
 */

/* internal public functions ================================================ */

/**
 * @brief Ouverture d'un fichier en EEPROM
 * Si un fichier correspondant à la taille est déjà présent à l'adresse de 
 * pcEeBuffer il est ouvert, sinon il est créé * 
 * @param pcEeBuffer Adresse du buffer en EEPROM où sera stocké le fichier
 * @param iEeBufferSize Taille en octets du buffer
 * @param iMode Drapeaux de mode, O_RDONLY, O_WRONLY, O_RDWR, O_APPEND pouvant
 *        être combinés
 * @return pointeur sur le fichier ou NULL en cas d'erreur (errno indique l'erreur)
 */
FILE * xEeFileOpen (uint8_t *pcEeBuffer, int iEeBufferSize, int iMode);

/**
 * @brief Fermeture d'un fichier en EEPROM 
 * @param xFile pointeur sur le fichier ouvert avec xEeFileOpen()
 * @return 0, -1 si erreur
 */
int iEeFileClose (FILE * xFile);

/**
 * @brief Vide le fichier
 * @param xFile pointeur sur le fichier ouvert avec xEeFileOpen()
 * @return 0, -1 si erreur
 */
int iEeFileFlush (FILE * xFile);

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_EEFILE_H_ */
