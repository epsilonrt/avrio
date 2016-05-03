/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2000, 2001, 2008 Bernie Innocenti <bernie@codewiz.org>
 * --> */
#ifndef _AVRIO_HEAP_H_
#define _AVRIO_HEAP_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup heap_module Allocation mémoire dynamique
 *  Ce module fournit un mécanisme d'allocation dynamique de mémoire optimisé.
 *  @{
 */
#ifndef __DOXYGEN__
/* NOTE: struct size must be a 2's power! */
typedef struct xMemChunk {
  struct xMemChunk *next;
  size_t size;
} xMemChunk;

typedef xMemChunk heap_buf_t;

#define HEAP_DEFINE_BUF(name, size) \
  heap_buf_t name[((size) + sizeof(heap_buf_t) - 1) / sizeof(heap_buf_t)]

#else
/**
 * @brief Allocation d'un tas de taille size
 * @param nom du tas
 * @param taille du tas en octets
 */
#define HEAP_DEFINE_BUF(name, size)
#endif

/**
 * @brief Structure pour gérer un tas
 */
typedef struct xHeap {
  struct xMemChunk * freelist; /**< Début du tas */
} xHeap;

/**
 * @brief Initialise un tas
 *
 * @param heap tas
 * @param memory buffer mémoire déclaré avec \ref HEAP_DEFINE_BUF()
 * @param size taille en octets du buffer
 */
void vHeapInit (xHeap* heap, void* memory, size_t size);

/**
 * @brief Alloue un bloc sur le tas
 *
 * @param heap tas
 * @param size taille en octets du bloc demandé
 * @return pointeur sur le bloc ou NULL en cas d'erreur
 */
void * pvHeapAllocMem (xHeap* heap, size_t size);

/**
 * @brief Libère un bloc alloué avec pvHeapAllocMem()
 *
 * @param heap tas
 * @param mem pointeur sur le bloc à libérer
 * @param size taille en octets du bloc
 */
void vHeapFreeMem (xHeap* heap, void *mem, size_t size);

/**
 * @brief Quantité de mémoire libre d'un tas
 *
 * @param heap tas
 * @return nombre d'octets libre
 * @note La valeur retournée est la somme de toutes les zones libres. Ces régions
 * ne sont pas forcément contigues, aussi une allocation peut échouer même si
 * si la taille demandée est inférieure à l'espace libre.
 */
size_t ulHeapFreeSpace (xHeap* heap);

/**
 * @brief Alloue un bloc sur le tas
 *
 * Cette fonction est conforme à la fonction malloc() C ANSI. \n
 * Chaque bloc alloué avec cette fonction aura une taille supérieure à la taille
 * demandée de sizeof(size_t) (2 octets sur AVR8).
 *
 * @param heap tas
 * @param size taille en octets du bloc demandé
 * @return pointeur sur le bloc ou NULL en cas d'erreur
 */
void * pvHeapMalloc (xHeap* heap, size_t size);

/**
 * @brief Alloue un bloc sur le tas pour un tableau
 *
 * Cette fonction est conforme à la fonction calloc() C ANSI. \n
 * Chaque bloc alloué avec cette fonction aura une taille supérieure à la taille
 * demandée de sizeof(size_t) (2 octets sur AVR8).
 *
 * @param heap tas
 * @param nmemb nombre d'éléments
 * @param size taille en octets de l'élément
 * @return pointeur sur le bloc ou NULL en cas d'erreur
 */
void * pvHeapCalloc (xHeap* heap, size_t nmemb, size_t size);

/**
 * @brief Libère un bloc alloué avec pvHeapMalloc() ou pvHeapCalloc()
 *
 * Cette fonction est conforme à la fonction free() C ANSI. \n
 *
 * @param heap tas
 * @param mem pointeur sur le bloc à libérer
 */
void vHeapFree (xHeap* heap, void * mem);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_HEAP_H_ */
