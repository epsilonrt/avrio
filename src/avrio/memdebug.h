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

#if defined(__cplusplus)
  extern "C" {
#endif
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
#if defined(__cplusplus)
  }
#endif
#endif // _AVRIO_MEM_DEBUG_H_
