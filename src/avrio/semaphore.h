/**
 * @file semaphore.h
 * @brief Sémaphore
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#ifndef _AVRIO_SEMAPHORE_H_
#  define _AVRIO_SEMAPHORE_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup semaphore_module Sémaphore
 *
 * Les sémaphores sont des compteurs permettant de gérer l'accès à des
 * ressources partagées.
 *  @{
 */
/**
 * @brief Valeur d'un sémaphore
 */
typedef uint16_t xSemValue;

/**
 * @brief Type sémaphore
 */
typedef volatile xSemValue xSem;

/**
 * @brief Initialisation de la valeur d'un sémaphore (Init)
 * @param pxSemaphore pointeur sur le sémaphore.
 * @param xSetValue valeur du sémaphore (nombre de ressources à partager)
 */
void vSemInit (xSem * pxSemaphore, xSemValue xSetValue);

/**
 * @brief Incrémente un sémaphore (V)
 * @param pxSemaphore pointeur sur le sémaphore
 */
void vSemPost (xSem * pxSemaphore);

/**
 * @brief Attend jusqu'à ce que le sémaphore soit non nul et le décrémente (P)
 * @param pxSemaphore pointeur sur le sémaphore
 */
void vSemWait (xSem * pxSemaphore);

/**
 * Variante non  bloquante de vSemWait.
 * @param pxSemaphore pointeur sur le sémaphore à attendre
 * @return 0 si sémaphore est décrémenté, différent de 0 si le sémaphore est nul
 */
int8_t xSemTryWait (xSem * pxSemaphore);

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
#endif /* _AVRIO_SEMAPHORE_H_ */
