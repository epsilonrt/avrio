/**
 * @file led.c
 * @brief LED (diodes électroluminescentes)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

 */
#include "avrio-config.h"

#ifdef AVRIO_LED_ENABLE
/* ========================================================================== */
#  include <avrio/led.h>

/* public variables ========================================================= */
DECLARE_LED_MASK_ARRAY;

#endif /* AVRIO_LED_ENABLE defined */
/* ========================================================================== */
