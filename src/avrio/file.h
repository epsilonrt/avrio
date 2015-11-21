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
 * @file file.h
 * @brief Extension du système de fichiers de la libc
 */
#ifndef _AVRIO_FILE_H_
#define _AVRIO_FILE_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup file_module Fichiers
 *  Définit un système de fichiers simple. Utilise le pointeur udata du fichier
 *  stdio pour étendre les possibilités.
 *  @{
 *  @example file/demo_file.c
 */

/* flags ==================================================================== */
/**
 *    @defgroup file_flag Flags
 *    @{
 */
#define O_RD       0x0001 /**< accès en lecture */
#define O_RDONLY   O_RD /**< accès en lecture seule */
#define O_WR       0x0002 /**< accès en écriture */
#define O_WRONLY   O_WR /**< accès en écriture seule */
#define O_RDWR     (O_RD|O_WR) /**< accès en lecture et écriture */
#define O_NONBLOCK 0x0004 /**< accès non bloquant */
#define O_ECHO     0x0008 /**< echo de tous les caractères reçus */
#define O_MALLOC   0x2000 /**< allocation dynamique de l'extension */
#define O_SYNC     0x4000 /**< écriture synchrone (invalide le buffer de transmission)*/
#define O_APPEND   0x8000 /**< ajout à la fin du fichier à accès direct */
/**
 *   @}
 */

/* ioctl request ============================================================ */
/**
 *    @defgroup file_ioctl Requêtes ioctl
 *    @{
 */
#define FIOFLUSH   0x0001 /**< vidage des buffers */
#define FIONREAD   0x0002 /**< nombre de caractères reçus */
#define FIOGETS    0x0003 /**< lecture de la configuration*/
#define FIOSETS    0x0004 /**< modification de la configuration */
/**
 *   @}
 */

/* internal public functions ================================================ */
/**
 * @brief Ouverture d'un fichier
 *
 * @param path chemin du fichier
 * @param flag \c file_flag
 * @param settings pointeur sur une structure de configuration spécifique au périphérique
 * @return pointeur sur le fichier ouvert, NULL si erreur (cf errno)
 */
FILE * xFileOpen (const char * path, int flag, void * settings);

/**
 * @brief Fermeture d'un fichier
 * @param f pointeur sur le fichier ouvert avec xFileOpen()
 * @return 0, -1 si erreur
 */
int iFileClose (FILE * f);

/**
 * @brief Fonction de contrôle
 * @param f pointeur sur le fichier ouvert avec xFileOpen()
 * @param c requête \c file_ioctl, suivie des paramètres éventuels
 * @return 0, -1 si erreur
 */
int iFileIoctl (FILE * f, int c, ...);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 * @brief Vidage des buffers
 * @param f pointeur sur le fichier ouvert avec xFileOpen()
 * @return 0, -1 si erreur
 */
static inline int iFileFlush (FILE * f);

/**
 * @brief Nombre de caractères reçus
 * @param f pointeur sur le fichier ouvert avec xFileOpen()
 * @return Nombre de caractères reçus, -1 si erreur
 */
static inline int iFileDataAvailable (FILE * f);

/**
 * @brief Modification de la configuration
 * @param f pointeur sur le fichier ouvert avec xFileOpen()
 * @param settings pointeur sur une structure de configuration spécifique au périphérique
 * @return 0, -1 si erreur
 */
static inline int iFileGetSettings (FILE * f, void * settings);

/**
 * @brief Lecture de la configuration
 * @param f pointeur sur le fichier ouvert avec xFileOpen()
 * @param settings pointeur sur une structure de configuration spécifique au périphérique
 * @return 0, -1 si erreur
 */
static inline int iFileSetSettings (FILE * f, const void * settings);

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

// -----------------------------------------------------------------------------
INLINE int
iFileFlush (FILE * f) {
  return iFileIoctl (f, FIOFLUSH);
}

// -----------------------------------------------------------------------------
INLINE int
iFileDataAvailable (FILE * f) {
  return iFileIoctl (f, FIONREAD);
}

// -----------------------------------------------------------------------------
INLINE int
iFileGetSettings (FILE * f, void * s) {
  return iFileIoctl (f, FIOGETS, s);
}

// -----------------------------------------------------------------------------
INLINE int
iFileSetSettings (FILE * f, const void * s) {
  return iFileIoctl (f, FIOSETS, s);
}
#endif

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_FILE_H_ */
