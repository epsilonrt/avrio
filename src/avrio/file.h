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
 *  @defgroup file_port Système de fichiers
 *
 *  @{
 *  @example file/demo_file.c
 */

/* constants ================================================================ */
#define O_RD       0x0001
#define O_RDONLY   O_RD
#define O_WR       0x0002
#define O_WRONLY   O_WR
#define O_RDWR     (O_RD|O_WR)
#define O_NONBLOCK 0x0004
#define O_ECHO     0x0008
#define O_MALLOC   0x2000
#define O_SYNC     0x4000
#define O_APPEND   0x8000

#define FIOFLUSH   0x0001
#define FIONREAD   0x0002
#define FIOGETS    0x0003
#define FIOSETS    0x0004

/* internal public functions ================================================ */
/**
 * @brief Ouverture d'un fichier
 * 
 * @param path
 * @param flag
 * @param settings
 * @return 
 */
FILE * xFileOpen (const char * path, int flag, void * settings);

/**
 * @brief Fermeture d'un fichier
 * @param f
 * @return 
 */
int iFileClose (FILE * f);

/**
 * @brief Fonction de Contrôle 
 */
int iFileCtl (FILE * f, int c, ...);

/**
 * @brief
 */
#define iFileFlush(f) iFileCtl(f,FIOFLUSH)

/**
 * @brief
 */
#define iFileDataAvailable(f) iFileCtl(f,FIONREAD)

/**
 * @brief
 */
#define iFileGetSettings(f,s) iFileCtl(f,FIOGETS,s)

/**
 * @brief
 */
#define iFileSetSettings(f,s) iFileCtl(f,FIOSETS,s)

/**
 *   @}
 * @}
 */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_FILE_H_ */
