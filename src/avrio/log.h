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
 * @file log.h
 * @brief Log (compatibilité sysio)
 */
#ifndef _AVRIO_LOG_H_
#define _AVRIO_LOG_H_
#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#define LOG_EMERG   0 /* system is unusable */
#define LOG_ALERT   1 /* action must be taken immediately */
#define LOG_CRIT    2 /* critical conditions */
#define LOG_ERR     3 /* error conditions */
#define LOG_WARNING 4 /* warning conditions */
#define LOG_NOTICE  5 /* normal but significant condition */
#define LOG_INFO    6 /* informational */
#define LOG_DEBUG   7 /* debug-level messages */

#define vLog (p,fmt,...)
#define vLogSetMask(m)
#define LOG_UPTO(p) (p)
#define LOG_MASK(p) (p)

#if defined(LOG_DEBUG) 
#include <avr/pgmspace.h>
#define PDEBUG(fmt,...) printf_P(PSTR("d:"fmt),##__VA_ARGS__)
#define PINFO(fmt,...) printf_P(PSTR("i:"fmt),##__VA_ARGS__)
#define PNOTICE(fmt,...) printf_P(PSTR("n:"fmt),##__VA_ARGS__)
#define PWARNING(fmt,...) printf_P(PSTR("W:"fmt),##__VA_ARGS__)
#define PERROR(fmt,...) printf_P(PSTR("E:"fmt),##__VA_ARGS__)
#else
#define PDEBUG(fmt,...)
#define PINFO(fmt,...)
#define PNOTICE(fmt,...)
#define PWARNING(fmt,...)
#define PERROR(fmt,...)
#endif

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_LOG_H_ */
