/*
 * FreeModbus Libary: AVR Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version + ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#ifndef _PORT_H
#  define _PORT_H

/* ----------------------- Platform includes -------------------------------- */
#  include <avr/interrupt.h>
#  include <avrio/defs.h>

/* ----------------------- Defines ------------------------------------------ */
#  define PR_BEGIN_EXTERN_C           extern "C" {
#  define PR_END_EXTERN_C             }

#  define ENTER_CRITICAL_SECTION( )   cli()
#  define EXIT_CRITICAL_SECTION( )    sei()

#  define assert( x )

typedef bool BOOL;

typedef uint8_t UCHAR;
typedef int8_t CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#  ifndef TRUE
#    define TRUE            (1 == 1)
#  endif

#  ifndef FALSE
#    define FALSE           (1 == 0)
#  endif

/* ========================================================================== */
#endif /* _PORT_H */
