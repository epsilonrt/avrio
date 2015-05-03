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
 * @file hdlc.h
 * @brief Définitions HDLC
 * @author Francesco Sacchi <batt@develer.com>
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
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
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 */

#ifndef _AVRIO_HDLC_H_
#define _AVRIO_HDLC_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/**
 *  @addtogroup net_group
 *  @{
 *  @defgroup hdlc_module Liaison HDLC
 *  Ce module contient les définitions pour la liaison HDLC.
 *  @{
 */

/* constants ================================================================ */
/**
 * @brief Drapeau HDLC
 *
 * Marque le début et la fin d'une trame HDLC (6 bits à '1')
 */
#define HDLC_FLAG  0x7E

/**
 * @brief Reset HDLC
 *
 * Provoque l'arrêt de traitement d'une trame HDLC (7 bits à '1')
 */
#define HDLC_RESET 0x7F

/**
 * @brief Code d'échappement HDLC
 *
 * Ce code permet de stocker dans la pile de réception des codes HDLC
 */
#define HDLC_ESC 0x1B

/**
 *  @}
 * @}
 */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

#endif /* _AVRIO_HDLC_H_ */
