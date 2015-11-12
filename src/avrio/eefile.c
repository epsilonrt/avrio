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
 * @file eefile.c
 * @brief Fichier en mémoire EEPROM.
 */
#include <avrio/eefile.h>
#include <avrio/crc.h>
#include <string.h>
#include <errno.h>

/* structures =============================================================== */
/*
 * Entête de fichier en EEPROM
 * En EEPROM cet entête est suivi d'un octet de CRC iButton permettant de 
 * vérifier sa consistance puis des octets de données du fichier
 */
typedef struct xEeFileHeader {
  int iSize;
  int iLen;
  uint16_t usCrc;
} xEeFileHeader;

/*
 * Extension de la structure FILE (udata)
 */
typedef struct xEeFile {
  xEeFileHeader xHdr;
  int iFlags;
  uint8_t * pucEeBlock;
} xEeFile;

/* constants ================================================================ */
// Bloc de données après l'entête et le crc iButton de l'entête (8 bits)
#define DATA_OFFSET (sizeof(xEeFileHeader) + 1)

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Fonction d'écriture d'un octet dans le fichier appelée par les fonctions libc
static int
iPut (char c, FILE * f) {
  xEeFile * eef = (xEeFile *) fdev_get_udata (f);

  if (eef->xHdr.iLen < eef->xHdr.iSize) {
    uint8_t * eeb = &eef->pucEeBlock[DATA_OFFSET];

    eeprom_write_byte (&eeb[eef->xHdr.iLen], c);
    eef->xHdr.iLen++;
    eef->xHdr.usCrc = usCrcCcittUpdate (c, eef->xHdr.usCrc);
    return 0;
  }

  return _FDEV_EOF;
}

// -----------------------------------------------------------------------------
// Fonction de lecture d'un octet dans le fichier appelée par les fonctions libc
static int
iGet (FILE * f) {
  xEeFile * eef = (xEeFile *) fdev_get_udata (f);

  if (eef->xHdr.iLen < eef->xHdr.iSize) {
    uint8_t * eeb = &eef->pucEeBlock[DATA_OFFSET];

    return (unsigned) eeprom_read_byte (&eeb[eef->xHdr.iLen++]);
  }
  return _FDEV_EOF;
}

/* -----------------------------------------------------------------------------
 * Initialise la structure FILE avec les éléments en EEPROM ou par défaut:
 * - Si l'entête n'est pas lu en EEPROM, un nouveau fichier est initialisé
 * - Si le bloc demandé en EEPROM a une taille différente de celle lue en 
 *   EEPROM il est flushé
 * - Si l'entête a pu être lu en EEPROM, la longueur et le CRC du fichier 
 *   existant est lu, le CRC est vérifié. Si un accès avec O_APPEND est 
 *   demandé, la tête est placé à la fin du fichier
 */
static int
iInitFile (FILE * f) {
  xEeFileHeader eehdr;
  xEeFile * eef = (xEeFile *) fdev_get_udata (f);
  bool isNewFile = true;

  if (iEepromLoadBlock (&eehdr, eef->pucEeBlock, sizeof (eehdr)) == 0) {

    // Entête de fichier existant lue avec succès
    if (eef->xHdr.iSize == eehdr.iSize) {

      // La taille demandée correspond à la taille actuelle
      if ( (eef->iFlags & O_WRONLY) && (eef->iFlags & O_APPEND)) {

        eef->xHdr.iLen = eehdr.iLen;
      }
      eef->xHdr.usCrc = eehdr.usCrc;
      isNewFile = false;
    }
  }

  if (isNewFile) {

    // Nouveau fichier
    eef->xHdr.usCrc = CRC_CCITT_INIT_VAL;
  }
  else {

    // Fichier existant, on vérifie le CRC
    if (eef->xHdr.iLen) {
      uint16_t usCrc = CRC_CCITT_INIT_VAL;
      uint8_t * eeb = &eef->pucEeBlock[DATA_OFFSET];

      for (int i = 0; i < eef->xHdr.iLen; i++) {

        usCrc = usCrcCcittUpdate (eeprom_read_byte (&eeb[i]), usCrc);
      }

      if (usCrc != eef->xHdr.usCrc) {

        // Erreur de CRC, on flush le fichier
        errno = EIO;
        return iEeFileFlush (f);
      }
      // Fichier correctement lu
    }
  }
  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iEeFileFlush (FILE * f) {

  xEeFile * eef = (xEeFile *) fdev_get_udata (f);
  eef->xHdr.iLen = 0;
  eef->xHdr.usCrc = CRC_CCITT_INIT_VAL;
  return 0;
}

#ifndef AVRIO_EEFILE_STATIC
/* ==========================================================================
 * Implémentation par défaut avec mémoire dynamique (malloc)
 */
#include <stdlib.h>

// -----------------------------------------------------------------------------
FILE *
xEeFileOpen (uint8_t * pcBufferEem, int iSize, int iFlags) {

  if (iSize > (sizeof (xEeFileHeader) + 1)) {

    xEeFile * eef = calloc (1, sizeof (xEeFile));
    if (eef == NULL) {

      errno = ENOMEM;
      return NULL;
    }
    eef->xHdr.iSize = iSize;
    eef->iFlags = iFlags;
    eef->pucEeBlock = (uint8_t *) pcBufferEem;

    FILE * f = calloc (1, sizeof (FILE));
    if (f == NULL) {

      free (eef);
      errno = ENOMEM;
      return NULL;
    }

    uint8_t flags = ( (iFlags & O_RDONLY) ? __SRD : 0) |
                    ( (iFlags & O_WRONLY) ? __SWR : 0);
    fdev_setup_stream (f, iPut, iGet, flags);
    fdev_set_udata (f, eef);
    if (iInitFile (f) == 0) {

      return f;
    }

    free (eef);
    free (f);
  }
  return NULL;
}

// -----------------------------------------------------------------------------
int
iEeFileClose (FILE * f) {
  xEeFile * eef = (xEeFile *) fdev_get_udata (f);

  vEepromSaveBlock (&eef->xHdr, eef->pucEeBlock, sizeof (eef->xHdr));
  free (eef);
  free (f);
  return 0;
}

#else /* AVRIO_EEFILE_STATIC defined */
/* ==========================================================================
 * Implémentation sans mémoire dynamique (no malloc)
 */

/* private variables ======================================================== */
#warning TODO
static int iFileCount;
static xEeFile xEeFileList[AVRIO_EEFILE_STATIC];
static FILE xFileList[AVRIO_EEFILE_STATIC];

// -----------------------------------------------------------------------------
FILE *
xEeFileOpen (uint8_t * pcBufferEem, int iSize, int iFlags) {

  if (iSize > (sizeof (xEeFileHeader) + 1)) {

    xEeFile * eef = &xEeFileList[iFileCount];
    eef->xHdr.iSize = iSize;
    eef->iFlags = iFlags;
    eef->pucEeBlock = (uint8_t *) pcBufferEem;

    FILE * f = &xFileList[iFileCount++];
    uint8_t flags = ( (iFlags & O_RDONLY) ? __SRD : 0) |
                    ( (iFlags & O_WRONLY) ? __SWR : 0);
    fdev_setup_stream (f, iPut, iGet, flags);
    fdev_set_udata (f, eef);
    if (iInitFile (f) == 0) {

      return f;
    }
  }
  return NULL;
}

// -----------------------------------------------------------------------------
int
iEeFileClose (FILE * f) {
  xEeFile * eef = (xEeFile *) fdev_get_udata (f);

  vEepromSaveBlock (&eef->xHdr, eef->pucEeBlock, sizeof (eef->xHdr));
  memset (f, 0, sizeof(FILE));
  return 0;
}

/* ========================================================================== */
#endif /* AVRIO_EEFILE_STATIC defined */

/* ========================================================================== */
