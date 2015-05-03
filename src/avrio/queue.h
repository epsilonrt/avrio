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
 * @file queue.h
 * @brief Pile d'octets FIFO
 */
#ifndef _AVRIO_QUEUE_H_
#  define _AVRIO_QUEUE_H_

#  include <avrio/defs.h>
#  include <avrio/mutex.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup queue_module Pile d'octets FIFO
 *
 *  Une pile FIFO est un buffer circulaire qui dispose d'un pointeur en écriture
 *  et d'un pointeur en lecture.\n
 *  Les piles FIFO sont utilisées dans les communications inter-processus et
 *  les réseaux.\n
 *  @{
 */
  /* structures ============================================================= */
/**
 * @struct xQueue
 * @brief Pile FIFO
 */
   struct xQueue;

/* constants ================================================================ */

/* internal public functions ================================================ */
/**
 * @brief Empile un octet
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas pleine.
 *
 * @param pxQueue La pile à utiliser
 * @param ucByte L'octet à empiler
 */
void vQueuePush (struct xQueue *pxQueue, uint8_t ucByte);

/**
 * @brief Dépile un octet
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas vide.
 *
 * @param pxQueue La pile à utiliser
 * @return L'octet dépilé
 */
uint8_t ucQueuePull (struct xQueue *pxQueue);

/**
 * @brief Détruit un octet
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas vide.
 *
 * @param pxQueue La pile à utiliser
 */
void vQueueDrop (struct xQueue *pxQueue);

/**
 * @brief Lecture d'un octet (sans le dépiler)
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas vide et
 * que l'index fournit est inférieur à la longueur de la pile.
 *
 * @param pxQueue La pile à utiliser
 * @param xIndex Position de l'octet dans la pile entre 0 et (xQueueLength - 1)
 * @return L'octet lu
 */
uint8_t ucQueueRead (struct xQueue *pxQueue, size_t xIndex);

/**
 * @brief Empile un mot de 16 bits
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas pleine.
 *
 * @param pxQueue La pile à utiliser
 * @param usWord Le mot à empiler
 */
void vQueuePushWord (struct xQueue *pxQueue, uint16_t usWord);

/**
 * @brief Dépile un mot de 16 bits.
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas vide.
 *
 * @param pxQueue La pile à utiliser
 * @return Le mot dépilé
 */
uint16_t usQueuePullWord (struct xQueue *pxQueue);

/**
 * @brief Lecture d'un mot de 16 bits (sans dépiler)
 *
 * Il est du ressort de l'utilisateur de vérifier que la pile n'est pas vide et
 * que l'index fournit est inférieur à la longueur de la pile.
 *
 * @param pxQueue La pile à utiliser
 * @param xIndex Position du dans la pile entre 0 et (xQueueLength - 2)
 * @return Le mot lu
 */
uint16_t usQueueReadWord (struct xQueue *pxQueue, size_t xIndex);

/**
 * @brief Empile une chaîne d'octets à zéro terminal.
 *
 * L'empilage est limité à la taille de la pile.  Le zéro terminal n'est pas
 *  empilé.
 *
 * @param pxQueue La pile à utiliser
 * @param pcString Pointeur vers la chaîne d'octets à zéro terminal
 * @return Si la longueur de la chaine dépasse la taille de la pile,
 *  la fonction remplie la pile et renvoie le pointeur sur l'octet suivant
 *  le dernier transmis.\n
 *  Si la pile peut contenir la chaîne en entier la fonction renvoie le pointeur
 *  sur le zéro terminal.
 */
const char *pcQueuePushString (struct xQueue *pxQueue, const char *pcString);

/**
 * @brief Empile une suite d'octets.
 *
 * L'empilage est limité à la taille de la pile.
 *
 * @param pxQueue La pile à utiliser
 * @param pucBytes Pointeur vers les octets
 * @param xLength Nombre d'octets
 * @return 0 ou le nombre d'octets restants si le nombre d'octets dépasse la
 *  taille de la pile.
 */
size_t xQueuePushBytes (struct xQueue *pxQueue, const uint8_t * pucBytes,
                        size_t xLength);

/**
 * @brief Empile des octets d'une pile dans une autre.
 *
 * L'empilage est limité à la taille de la pile pxDstQueue.
 *
 * @param pxDstQueue La pile destinataire
 * @param pxSrcQueue La pile source
 * @return 0 ou le nombre d'octets restants dans pxSrcQueue si le nombre
 *  d'octets dépasse la taille de la pile.
 */
size_t xQueuePushBytesOfQueue (struct xQueue *pxDstQueue,
                               struct xQueue *pxSrcQueue, size_t xLength);

/**
 * @brief Dépile des octets
 *
 * @param pxQueue La pile à utiliser
 * @param pucBytes Pointeur vers la zone mémoire destination
 * @param xLength Le nombre  d'octets à dépiler (limité à la longueur de la pile)
 * @return Le nombre d'octets dépilés
 */
size_t xQueuePullBytes (struct xQueue *pxQueue,
                        uint8_t * pucBytes, size_t xLength);

/**
 * @brief Dépile tous les octets
 *
 * @param pxQueue La pile à utiliser
 * @param pucBytes Pointeur vers la zone mémoire destination
 * @return Le nombre d'octets dépilés
 */
size_t xQueuePullAll (struct xQueue *pxQueue, uint8_t * pucBytes);

/**
 * @brief Détruit plusieurs octets
 *
 * Le nombre d'octets à détruire est limité à la longueur de la pile
 *
 * @param pxQueue La pile à utiliser
 * @param xLength Nombre d'octets
 */
void vQueueDropBytes (struct xQueue *pxQueue, size_t xLength);

/**
 * @brief Vide la pile
 *
 * @param pxQueue La pile à utiliser
 */
void vQueueFlush (struct xQueue *pxQueue);

/**
 * @brief Initialise la pile avec le buffer
 *
 * @param pxQueue La pile à utiliser
 * @param pucBuffer Zone mémoire qui sera utilisée par la pile
 * @param xBufferSize Nombre d'octets pouvant être stockés dans le buffer
 */
void vQueueSetBuffer (struct xQueue *pxQueue, uint8_t * pucBuffer, size_t xBufferSize);

/**
 * @brief Renvoie le nombre d'octets empilés
 *
 * @param pxQueue La pile à utiliser
 * @return Le nombre d'octets empilés dans la pile
 */
size_t xQueueLength (struct xQueue *pxQueue);

/**
 * @brief Renvoie la longueur de la pile
 *
 * @param pxQueue La pile à utiliser
 * @return La taille de la pile
 */
size_t xQueueSize (struct xQueue *pxQueue);

/**
 * @brief Compare le contenu de deux piles d'octets
 *
 * @param pxQueue1 Première pile à utiliser
 * @param pxQueue2 Deuxième pile à utiliser
 * @return 0 si elles contiennent le même nombre d'octets identiques, -1 si
 *  ne contiennent aucun octets en commun, une valeur positive correspondant
 *  au nombre d'octets en communs.
 */
int iQueueCompare (struct xQueue *pxQueue1, struct xQueue *pxQueue2);

 /**
 * @brief Renvoie le nombre d'octets disponibles
 *
 * @param pxQueue La pile à utiliser
 * @return Le nombre d'octets disponibles
 */
size_t xQueueFree (struct xQueue *pxQueue);

/**
 * @brief Crée et initialise une pile en mémoire dynamique
 *
 * La macro AVRIO_QUEUE_MALLOC_ENABLE doit être définie dans avrio-config.h ou
 * à la compilation pour pouvoir utiliser cette fonction.
 *
 * @param xBufferSize Nombre d'octets pouvant être stockés dans la pile
 * @return Pointeur sur la nouvelle pile ou 0 en cas d'erreur
 */
struct xQueue *pxQueueNew (size_t xBufferSize);

/**
 * @brief Détruit une pile créée avec xQueueNew()
 *
 * La macro AVRIO_QUEUE_MALLOC_ENABLE doit être définie dans avrio-config.h ou
 * à la compilation pour pouvoir utiliser cette fonction.
 *
 * @param pxQueue La pile à détruire
 */
void vQueueDelete (struct xQueue *pxQueue);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/* macros =================================================================== */
/**
 * @def QUEUE_DECLARE(xVarName, xBufferSize)
 * @brief Déclare une variable de type pile FIFO
 *
 * @param xVarName Nom de la variable
 * @param xBufferSize Nombre d'octets pouvant être stockés dans la pile
 */
#    define QUEUE_DECLARE(xVarName, xBufferSize)

/**
 * @def QUEUE_STATIC_DECLARE(xVarName, xBufferSize)
 * @brief Déclare une variable static de type pile FIFO
 *
 * @param xVarName Nom de la variable
 * @param xBufferSize Nombre d'octets pouvant être stockés dans la pile
 */
#    define QUEUE_STATIC_DECLARE(xVarName, xBufferSize)

/* inline public functions ================================================== */

/**
 * @brief Teste si la pile est vide.
 *
 * @param pxQueue La pile à utiliser
 * @return true si la pile est vide, false sinon
 */
bool xQueueIsEmpty (struct xQueue *pxQueue);

/**
 * @brief Teste si la pile est vide.
 *
 * @param pxQueue La pile à utiliser
 * @return true si la pile est pleine, false sinon
 */
bool xQueueIsFull (struct xQueue *pxQueue);

/**
 * @brief Empile une pile dans une autre.
 *
 * L'empilage est limité à la taille de la pile pxDstQueue.
 *
 * @param pxDstQueue La pile destinataire
 * @param pxSrcQueue La pile source
 * @return 0 ou le nombre d'octets restants dans pxSrcQueue si le nombre
 *  d'octets dépasse la taille de la pile.
 */
size_t xQueuePushQueue (struct xQueue *pxDstQueue, struct xQueue *pxSrcQueue);

/**
 * @brief Renvoie la taille mémoire d'une pile
 *
 * @param pxQueue La pile à utiliser
 * @return La taille mémoire d'une pile (y compris la taille du buffer)
 */
size_t xQueueSizeOf (struct xQueue *pxQueue);

/**
 * @brief Copie d'une pile sur une autre
 *
 * @param pxDst La pile destination
 * @param pxSrc La pile source
 * @warning Les octets contenus dans la source ne sont pas copiés.
 */
void vQueueCopy (struct xQueue *pxDst, const struct xQueue *pxSrc);

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
#    include "avrio-config.h"
#    include <string.h>

/* structures =============================================================== */
struct xQueue {
  uint8_t *pxFirst;
  uint8_t *pxLast;
  uint8_t * volatile pxIn;
  uint8_t * volatile pxOut;
  volatile bool isEmpty;
  volatile bool isFull;
  // TODO: suppression
  xMutex xLock;
};

/* types ==================================================================== */
typedef struct xQueue xQueue;

/* macros =================================================================== */
#define QUEUE_MUTEX_INITIALIZER (MUTEX_INITIALIZER & ~QUEUE_LOCK_EMPTY)

  // ----------------------------------------------------------------------------
#      define QUEUE_DECLARE(__name,__size)                          \
    uint8_t __name ## Buffer[__size];                           \
    xQueue __name = { .pxFirst = __name ## Buffer,              \
                      .pxLast  = __name ## Buffer + __size - 1, \
                      .pxIn    = __name ## Buffer,              \
                      .pxOut   = __name ## Buffer,              \
                      .isEmpty = true,                          \
                      .isFull  = false,                         \
                      .xLock   = MUTEX_INITIALIZER }

  // ----------------------------------------------------------------------------
#      define QUEUE_STATIC_DECLARE(__name,__size)                           \
    static uint8_t __name ## Buffer[__size];                            \
    static xQueue __name = {  .pxFirst = __name ## Buffer,              \
                              .pxLast  = __name ## Buffer + __size - 1, \
                              .pxIn    = __name ## Buffer,              \
                              .pxOut   = __name ## Buffer,              \
                              .isEmpty = true,                          \
                              .isFull  = false,                         \
                              .xLock   = MUTEX_INITIALIZER }

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
INLINE bool xQueueIsEmpty (xQueue * q) {

  return q->isEmpty;
}

// ------------------------------------------------------------------------------
INLINE bool xQueueIsFull (xQueue * q) {

  return q->isFull;
}

// ------------------------------------------------------------------------------
INLINE size_t xQueuePushQueue (struct xQueue *pxDstQueue,
                               struct xQueue *pxSrcQueue) {

  return xQueuePushBytesOfQueue (pxDstQueue, pxSrcQueue,
                                 xQueueLength (pxSrcQueue));
}

// ------------------------------------------------------------------------------
INLINE size_t xQueueSizeOf (xQueue * q) {

  return sizeof (xQueue) + xQueueSize (q);
}

// ------------------------------------------------------------------------------
INLINE void
  vQueueCopy (struct xQueue *pxDst, const struct xQueue *pxSrc) {

  memcpy (pxDst, pxSrc, sizeof(xQueue));
}

////////////////////////////////////////////////////////////////////////////////
//
// TODO:
//  Suppression des mutex (recherche dépendances dans avrio et autres projets
//
////////////////////////////////////////////////////////////////////////////////
/* constants ================================================================ */
/**
 * @enum eQueueLock
 * @brief Liste des verrous utilisés par le module.
 */
typedef enum {

  QUEUE_LOCK_RD     = 0x01,  /**< Verrou d'accès en lecture  */
  QUEUE_LOCK_WR     = 0x02,  /**< Verrou d'accès en écriture */
  QUEUE_LOCK_FULL   = 0x04,  /**< Verrou pile pleine */
  QUEUE_LOCK_EMPTY  = 0x08,  /**< Verrou pile vide */
  QUEUE_LOCK_FREE   = QUEUE_LOCK_RD | QUEUE_LOCK_WR,
  QUEUE_LOCK_ALL    = QUEUE_LOCK_RD | QUEUE_LOCK_WR | QUEUE_LOCK_FULL
} eQueueLock;

// ----------------------------------------------------------------------------
INLINE void
                        vQueueUnlock (struct xQueue *q,
                                      uint8_t ucMask) {

  vMutexUnlockBit (&(q->xLock), ucMask);
}

// ----------------------------------------------------------------------------
INLINE void
                        vQueueLock (struct xQueue *q, uint8_t ucMask) {

  vMutexLockBit (&(q->xLock), ucMask);
}

// ----------------------------------------------------------------------------
INLINE void
                        vQueueUntilLocked (struct xQueue *q, uint8_t ucMask) {

  vMutexUntilBitUnlocked (&(q->xLock), ucMask);
}

// ----------------------------------------------------------------------------
INLINE int8_t
                        xQueueTryLock (struct xQueue *q,
                                       uint8_t ucMask) {

  return xMutexTryLockBit (&(q->xLock), ucMask);
}

// ------------------------------------------------------------------------------
INLINE void vQueueWaitUntilIsFull (struct xQueue * q) {

  vQueueUntilLocked    (q, QUEUE_LOCK_FULL);
}

// ------------------------------------------------------------------------------
INLINE void vQueueWaitUntilIsEmpty (struct xQueue * q) {

  vQueueUntilLocked    (q, QUEUE_LOCK_EMPTY);
}

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_QUEUE_H_ */
