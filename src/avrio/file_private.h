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
 * @file file_private.h
 * @brief Extension du système de fichiers de la libc
 */
#ifndef _AVRIO_FILE_PRIVATE_H_
#define _AVRIO_FILE_PRIVATE_H_

#include <avrio/file.h>
__BEGIN_C_DECLS
/* ========================================================================== */

/* constants ================================================================ */
typedef struct xFileHook {
  int flag;
  void * dev;
  int (*close)(FILE*);
  int (*ioctl)(FILE*, int, va_list);
} xFileHook;

/* internal public functions ================================================ */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_FILE_PRIVATE_H_ */
