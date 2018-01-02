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
 * @file file_private.h
 * @brief Extension du système de fichiers de la libc
 */
#ifndef _AVRIO_FILE_PRIVATE_H_
#define _AVRIO_FILE_PRIVATE_H_
#if ! defined(__DOXYGEN__)

#include <avrio/file.h>
#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */

/* structures =============================================================== */
/**
 * @brief Extension du fichier stdio
 * Usage interne à avrio
 */
typedef struct xFileHook {
  int flag; /**< flags définis dans file.h */
  void * dev; /**< accès au matériel */
  int (*close)(FILE*);
  int (*ioctl)(FILE*, int, va_list);
} xFileHook;

// -----------------------------------------------------------------------------
INLINE void *
pvFileDevice (FILE * f) {
  
  return ((xFileHook *) fdev_get_udata (f))->dev;
}

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif /* __DOXYGEN__ not defined */
#endif /* _AVRIO_FILE_PRIVATE_H_ */
