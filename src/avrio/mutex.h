/**
 * @file mutex.h
 * @brief Mutex
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

 */
#ifndef _AVRIO_MUTEX_H_
#  define _AVRIO_MUTEX_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup mutex_module Mutex
 *
 * Un mutex est un sémaphore binaire permettant de gérer l'accès
 * à une ressource unique. \n
 * Par convention un mutex verrouillé vaut 0, et un mutex libre vaut une
 * valeur différente de 0.
 *  @{
 */

/**
 * @brief Valeur d'un mutex verrouillé
 */
#  define MUTEX_LOCK  (0)

/**
 * @brief Valeur d'un mutex libre
 */
#  define MUTEX_UNLOCK  (-1)

/**
 * @brief Valeur d'initialisation d'un mutex non verrouillé (libre)
 *
 * Par convention un mutex verrouillé vaut 0, et un mutex libre vaut une
 * valeur différente de 0.
 */
#  define MUTEX_INITIALIZER MUTEX_UNLOCK

/**
 * @brief Type mutex
 */
typedef volatile uint8_t xMutex;

/**
 * Libère un mutex (V)
 * @param pxMutex pointeur sur le mutex
 */
void vMutexUnlock (xMutex * pxMutex);

/**
 * @brief Attend jusqu'à ce que le mutex soit libre et le verrouille (P)
 * @param pxMutex pointeur sur le mutex
 */
void vMutexLock (xMutex * pxMutex);

/**
 * @brief Variante non  bloquante de vMutexLock.
 * @param pxMutex pointeur sur le mutex à attendre
 * @return 0 s'il a pu être verrouillé, différent de 0 si le bit du mutex est
 *  déjà verrouillé.
 */
int8_t xMutexTryLock (xMutex * pxMutex);

/**
 * @addtogroup mutex_module
 * @{
 *
 *  @defgroup mutexbit_module Mutex Bit
 *
 * Un mutex est un sémaphore binaire, les fonctions de ce module stocke sa
 * sous forme d'un bit d'un dans une variable xMutex.\n
 * Cela permet de gérer 8 mutex dans un octet au lieu d'un seul mais cela
 * alourdit une peu la taille du code source.
 *  @{
 */

/**
 * Libère un ou plusieurs bits d'un mutex (V)
 * @param pxMutex pointeur sur le mutex
 * @param ucMask Masque du ou des bits concernés
 */
void vMutexUnlockBit (xMutex * pxMutex, uint8_t ucMask);

/**
 * @brief Attend jusqu'à ce qu'un ou plusieurs bits d'un mutex soit libres et les verrouillent (P)
 * @param pxMutex pointeur sur le mutex
 * @param ucMask Masque du ou des bits concernés
 */
void vMutexLockBit (xMutex * pxMutex, uint8_t ucMask);

/**
 * @brief Variante non bloquante de vMutexLockBit()
 * @param pxMutex pointeur sur le mutex à attendre
 * @param ucMask Masque du ou des bits concernés
 * @return 0 s'il a pu être verrouillé, différent de 0 si déjà verrouillé.
 */
int8_t xMutexTryLockBit (xMutex * pxMutex, uint8_t ucMask);

/**
 * @brief Teste si un ou plusieurs bits sont verrouillés
 * @param pxMutex pointeur sur le mutex à attendre
 * @param ucMask Masque du ou des bits concernés
 * @return false si tous les bits sont libres, true sinon.
 */
bool xMutexBitLocked (xMutex * pxMutex, uint8_t ucMask);

/**
 * @brief Attend tant qu'un ou plusieurs bits d'un mutex sont verrouillés (P)
 * @param pxMutex pointeur sur le mutex
 * @param ucMask Masque du ou des bits concernés
 */
void vMutexUntilBitUnlocked (xMutex * pxMutex, uint8_t ucMask);

/**
 * @}
 */

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

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

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_MUTEX_H_ */
