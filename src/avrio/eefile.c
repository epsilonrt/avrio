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
  uint8_t * pucEeBlock; /**< Pointeur sur la zone de stockage EEPROM */
} xEeFile;

/* constants ================================================================ */
// Bloc de données après l'entête 
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

  // Lecture de l'entête du fichier en EEPROM
  if (iEepromLoadBlock (&eehdr, eef->pucEeBlock, sizeof (eehdr)) == 0) {

    // Entête de fichier existant lue avec succès
    if (eef->xHdr.iSize == eehdr.iSize) {

      // La taille demandée correspond à la taille actuelle
      if ( (eef->iFlags & O_WR) && (eef->iFlags & O_APPEND)) {
        
        // Mode ajout, positionne le pointeur d'accès à la fin du bloc
        eef->xHdr.iLen = eehdr.iLen;
      }
      // recopie du CRC16 actuel
      eef->xHdr.usCrc = eehdr.usCrc;
      isNewFile = false;
    }
  }
  else {
    // Echec de lecture, fichier inexistant, si pas d'accès en écriture on
    // on sort sur erreur "No such file or directory"
    if ((eef->iFlags & O_WR) == 0) {
      
      errno = ENOENT;
      return -1;
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

        // Erreur de CRC, on vide le fichier
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
