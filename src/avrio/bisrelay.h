/**
 * @file bisrelay.h
 * @brief Relais bistables
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140312 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BISRELAY_H_
#define _AVRIO_BISRELAY_H_
#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup bisrelay_module Relais bistables
 *  Ce module permet de gérer des relais bistables.
 *  @{
 *    @example bisrelay/demo_bisrelay.c
 *    Exemple qui bascule chaque relais à l'état On puis à l'état Off.
 */
/* macros =================================================================== */
/* constants ================================================================ */
#define BISRELAY_ALL      0xFF


/* structures =============================================================== */
typedef struct xBisRelay {
  uint8_t ucOn;
  uint8_t ucOff;
  volatile uint8_t *pPort;
} xBisRelay;

/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
/* internal public functions ================================================ */
void vBisRelayInit (void);
void vBisRelayClear (uint8_t ucMask);
void vBisRelaySet (uint8_t ucMask);
void vBisRelayToggle (uint8_t ucMask);

// -----------------------------------------------------------------------------
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
#include "avrio-board-bisrelay.h"

#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_BISRELAY_H_ not defined */
