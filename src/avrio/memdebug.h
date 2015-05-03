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
 * @file memdebug.h
 *
 *  Created on: 15 Dec 2010
 *      Author: Andy Brown
 *
 *  Use without attribution is permitted provided that this
 *  header remains intact and that these terms and conditions
 *  are followed:
 *
 *  http://andybrown.me.uk/ws/terms-and-conditions
 */
#ifndef _AVRIO_MEM_DEBUG_H_
#define _AVRIO_MEM_DEBUG_H_
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup memdebug_module Statistiques mémoire
 *  Ce module fournit des fonctions permettant de suivre l'utilisation de la
 *  mémoire vive.
 *  @{
 */

/**
 * @brief Mémoire vive utilisée par le programme
 *
 * Cela inclut la mémoire utilisée par la bibilothèque
 * @return nombre d'octets utilisés.
 */
size_t ulMemoryUsed(void);

/**
 * @brief Mémoire vive disponible
 *
 * @return nombre d'octets de RAM disponible
 */
size_t ulFreeMemory(void);

/**
 * @brief Taille du plus grand bloc de mémoire vive allouable par malloc()
 *
 * @return nombre d'octets
 */
size_t ulLargestAvailableMemoryBlock(void);

/**
 * @brief Taille du plus grand bloc de mémoire vive dans la free list
 *
 * @return nombre d'octets
 */
size_t ulLargestBlockInFreeList(void);

/**
 * @brief Nombre de blocs de mémoire vive dans la free list
 *
 * @return nombre de blocs
 */
int iNumberOfBlocksInFreeList(void);

/**
 * @brief Taille totale de la free list
 *
 * @return nombre d'octets
 */
size_t ulFreeListSize(void);

/**
 * @brief Taille du plus grand bloc de mémoire vive allouable par malloc() sans réutilistion de la free list
 *
 * @return nombre d'octets
 */
size_t ulLargestNonFreeListBlock(void);

/**
 *   @}
 * @}
 */

/* ======================================================================== */
__END_C_DECLS
#endif // _AVRIO_MEM_DEBUG_H_
