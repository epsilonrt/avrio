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
 * @file file.h
 * @brief Extension du système de fichiers de la libc
 */
#include <string.h>
#include <stdarg.h>
#include "avrio-config.h"
#include "file_private.h"
#include "tc.h"

/* external public functions ================================================ */
extern FILE * xTcOpen (const char * port, int flag, xTcIos * ios);

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
FILE *
xFileOpen (const char * path, int mode, void * settings) {

  if (strncmp (path, "tty", 3) == 0) {

    return xTcOpen (path, mode, (xTcIos *) settings);
  }
  // TODO: LCD, EEPROM ...
  return NULL;
}

// -----------------------------------------------------------------------------
int
iFileIoctl (FILE * f, int c, ...) {
  xFileHook * h = (xFileHook *) fdev_get_udata (f);

  if (h->ioctl) {
    int ret = 0;
    va_list ap;

    va_start (ap, c);
    ret = h->ioctl (f, c, ap);
    va_end (ap);
    return ret;
  }
  return -1; // Erreur si non implémenté
}

// -----------------------------------------------------------------------------
int
iFileClose (FILE * f) {
  xFileHook * h = (xFileHook *) fdev_get_udata (f);

  if (h->close) {

    return h->close (f);
  }
  return 0; // Pas d'erreur si non implémenté
}

/* ========================================================================== */
