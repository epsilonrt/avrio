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
