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
 * @file vector.c
 * @brief Tableau dynamique (Implémentation)
 *
 * Inspiré par la lecture du livre "Maitrise des algorithmes en C" de K. Loudon
 * 
 * Revision History ------------------------------------------------------------
 *    20151103 - Version initiale issue de sysio
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <avr/version.h>
#include <avrio/vector.h>
#include <avrio/assert.h>

#if __AVR_LIBC_VERSION__  < 10701UL 
#warning avr-libc realloc() have serious bug for version < 1.7.1, disabled used realloc()...
#define VECTOR_REALLOC_DISABLED
#endif

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iVectorInit (xVector * v, int growth, vVectorElmtNew fnew, vVectorElmtDestroy fdestroy) {

  if (v == NULL) {
    errno = EFAULT;
    return -1;
  }

  memset (v, 0, sizeof (xVector));
  v->fdestroy = fdestroy;
  v->fnew = fnew;
  v->growth = (growth <= 1) ? 1 : growth;
  return 0;
}

// -----------------------------------------------------------------------------
int
iVectorInitSearch (xVector * v, pvVectorElmtKey fkey,
                   iVectorElmtMatch fmatch) {
  if (v == NULL) {
    errno = EFAULT;
    return -1;
  }

  v->fkey = fkey;
  v->fmatch = fmatch;
  return 0;
}

// -----------------------------------------------------------------------------
int
iVectorResize (xVector * v, int new_size) {

  if (v == NULL) {

    errno = EFAULT;
    return -1;
  }

  if (new_size == v->size) {

    return 0;
  }

  if (new_size >= 0)  {
    int new_alloc = (new_size / v->growth) * v->growth;

    if ( (new_size - new_alloc) > 0) {

      new_alloc += v->growth;
    }

    // Retrait des éléments en excès
    if (v->fdestroy)  {
      for (int i = v->size - 1; i >= new_size; i--) {

        if (v->data[i]) {

          v->fdestroy (v->data[i]);
          v->data[i] = NULL;
        }
      }
    }

#ifndef VECTOR_REALLOC_DISABLED
    if (new_alloc != v->alloc) {
      void * p = realloc (v->data, sizeof (void*) * new_alloc);
      assert (p);
      v->alloc = new_alloc;
      v->data = p;
    }
#else
#warning TODO: do not used avr-libc version < 1.7.1
#endif

    if (new_size > v->size) {
      // Init des nouveaux éléments
      for (int i = v->size; i < new_size; i++) {
        if (v->fnew) {
          v->data[i] = v->fnew();
        }
        else {
          v->data[i] = NULL;
        }
      }
    }
    v->size = new_size;
    return 0;
  }
  return -1;
}

// -----------------------------------------------------------------------------
int
iVectorRemove (xVector *v, int index) {

  if (v == NULL) {

    errno = EFAULT;
    return -1;
  }

  if ( (index < v->size) && (index >= 0)) {

    // libère la mémoire de l'élément
    if ( (v->fdestroy) && (v->data[index])) {

      v->fdestroy (v->data[index]);
    }

    // Si il reste des éléments dans le vecteur les décaler vers la gauche
    if (v->size > 1) {

      memcpy (&v->data[index], &v->data[index + 1],
              sizeof (void *) * (v->size - index - 1));
    }
    v->size--;

#ifndef VECTOR_REALLOC_DISABLED
    if ( (v->alloc - v->size) >= v->growth) {
      // l'excès d'allocation est supérieur ou égal à l'acroissement, on réduit

      // si v->size vaut 0, équivalent à free
      v->data = realloc (v->data, sizeof (void*) * v->size);
      v->alloc = v->size;
    }
#else
#warning TODO: do not used avr-libc version < 1.7.1
#endif

    return 0;
  }
  return -1;
}

// -----------------------------------------------------------------------------
void
vVectorDestroy (xVector * v) {
  if (v) {
    if (iVectorClear (v) == 0) {

      if (v->malloc) {
        free (v);
      }
      else {
        memset (v, 0, sizeof (xVector));
      }
    }
  }
}

// -----------------------------------------------------------------------------
int
iVectorClear (xVector * v) {

  if (v == NULL) {

    errno = EFAULT;
    return -1;
  }

  if (v->fdestroy) {

    for (int i = 0; i < v->size; i++) {
      int dup ;
      void * p = v->data[i];

      if (p) {
        v->fdestroy (p);
        v->data[i] = NULL;

        // Tagg toutes les copies comme libérées
        while ( (dup = iVectorFindFirstIndex (v, p)) != -1) {

          v->data[dup] = NULL;
        }
      }
    }
  }
  free (v->data);
  v->data = NULL;
  v->size = 0;
  v->alloc = 0;
  return 0;
}


// -----------------------------------------------------------------------------
int
iVectorReplace (xVector * v, int index, void * e) {

  if (v == NULL) {

    errno = EFAULT;
    return -1;
  }
  if ( (index < 0) || (index > v->size)) {

    return -1;
  }
  v->data[index] = e;
  return 0;
}

// -----------------------------------------------------------------------------
void *
pvVectorGet (const xVector *v, int index) {

  if (v == NULL) {

    errno = EFAULT;
    return NULL;
  }
  if ( (index >= v->size) || (index < 0)) {

    return NULL;
  }

  return v->data[index];
}

// -----------------------------------------------------------------------------
int
iVectorAppend (xVector *v, void *e) {

  if (v == NULL) {

    errno = EFAULT;
    return -1;
  }
  if (iVectorResize (v, v->size + 1) != 0) {

    return -1;
  }
  v->data[v->size - 1] = e;
  return 0;
}

// -----------------------------------------------------------------------------
void *
pvVectorFindFirst (const xVector *v, const void * key) {

  if (v == NULL) {

    errno = EFAULT;
    return NULL;
  }

  if ( (v->fkey) && (v->fmatch)) {

    for (int i = 0; i < iVectorSize (v); i++) {
      void * p = v->data[i];
      if (p) {
        if (v->fmatch (key, v->fkey (p)) == 0) {

          return p;
        }
      }
    }
  }
  return NULL;
}

// -----------------------------------------------------------------------------
int
iVectorFindFirstIndex (const xVector * v, const void * key) {

  if (v == NULL) {

    errno = EFAULT;
    return -2;
  }

  if ( (v->fkey) && (v->fmatch)) {

    for (int i = 0; i < iVectorSize (v); i++) {
      void * p = v->data[i];
      if (p) {
        if (v->fmatch (key, v->fkey (p)) == 0) {

          return i;
        }
      }
    }
  }
  return -1;
}

// -----------------------------------------------------------------------------
int
iVectorSize (const xVector * v) {

  if (v == NULL) {

    errno = EFAULT;
    return -1;
  }
  return v->size;
}

/* ========================================================================== */
