/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file file.h
 * @brief Extension du système de fichiers de la libc
 */
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "avrio-config.h"
#include "file_private.h"
#include "tc.h"

/* external public functions ================================================ */
extern FILE * xTcOpen (const char * port, int flag, xSerialIos * ios);

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
FILE *
xFileOpen (const char * path, int mode, void * settings) {

  if (strncmp (path, "tty", 3) == 0) {

    return xTcOpen (path, mode, (xSerialIos *) settings);
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
