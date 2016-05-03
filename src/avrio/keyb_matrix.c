/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file keyb_matric.c
 * @brief Clavier matriciel
 */
#include "avrio-config.h"

#ifdef AVRIO_KEYB_ENABLE
/* ========================================================================== */
#  include "avrio-board-keyboard.h"
#  include <util/atomic.h>
#  include <avrio/task.h>
#  include <avrio/keyb.h>

# if KEYB_WIDTH > 4
#  error KEYB_WIDTH trop grand !
# endif
# if KEYB_HEIGHT > 4
#  error KEYB_HEIGHT trop grand !
# endif

/* ========================================================================== 
 * Implémentation inspirée de la note d'application ATMEL: AVR243: Matrix
 * Keyboard Decoder
 * http://www.atmel.com/dyn/resources/prod_documents/doc2532.pdf
 * ========================================================================= */

/* constants ================================================================ */
/* 
 * Différentes phases d'acquisition d'une touche
 */
enum {
  IDLE_STATE = 1, /* Aucune touche appuyée */
  DEBOUNCE_STATE, /* Une touche a été pressée, une temporisation va être
                   * effectuée pour l'anti-rebond */
  AVAILABLE_STATE,  /* Lecture touche terminée, les variables ucRow et ucCol
                     * ont été mises à jour */
  FREEZE_STATE  /* La touche a été lue, attente délai répétition */
};

/* macros =================================================================== */
#define KEYB_MSK(__width) ((uint8_t)((1 << (__width)) - 1))

/* private variables ======================================================== */
static volatile uint8_t ucState = IDLE_STATE;
static uint8_t ucRow, ucCol;
static const char acKey[KEYB_HEIGHT][KEYB_WIDTH] = KEYB_ARRAY;
static xTaskHandle xKeybTimer;

/* private functions ======================================================== */
#  include "keyb_matrix_pio.c"
#  include "keyb_matrix_pcf8574.c"
/* 
 * Décodage de la valeur lu sur les ports Ligne et Colonne
 * Cette fonction permet de donner le numéro de la ligne ou de la colonne
 * à partir du code lu sur le port. Par exemple:
 * Code lu = 00000001 (activation bit 0), la fonction prvucDecode() renvoie 0
 * Code lu = 00000100 (activation bit 2), la fonction prvucDecode() renvoie 2
 * Cela permet de simplifier le décodage de la touche en utilisant un tableau
 * à 2 dimensions (cf fonction iKeybGetChar())
 * @return numéro ligne ou colonne (entre 0 et 3) ou 0xFF en cas d'erreur si
 * plus d'un bit est à 1 ou si aucun bit est à 1
 */
static uint8_t
prvucDecode (uint8_t value) {
  uint8_t i = value;
  uint8_t result = 0xFF;

  while (i != 0) {
    i >>= 1;
    result++;
  }
  if (value > (1 << result))
    result = 0xFF;
  return result;
}

/* 
 * Renvoie le numéro entre 0 et 3 de la ligne de la touche
 */
static inline uint8_t
prvucRow (void) {

  return prvucDecode (prvucReadCodeY ());
}

/* 
 * Renvoie le numéro entre 0 et 3 de la colonne de la touche
 */
static inline uint8_t
prvucCol (void) {

  return prvucDecode (prvucReadCodeX ());
}

#  if KEYB_BUFSIZE > 0
/**************************** Version bufferisée ******************************/
#    include <avrio/queue.h>

QUEUE_STATIC_DECLARE (xKeybQueue, KEYB_BUFSIZE);

// ------------------------------------------------------------------------------
static inline void
prvvPutKey (void) {

  vQueuePush (&xKeybQueue, acKey[ucRow][ucCol]);
  ucState = FREEZE_STATE;
}

// ------------------------------------------------------------------------------
static inline char
prvvGetKey (void) {

  return ucQueuePull (&xKeybQueue);
}

// ------------------------------------------------------------------------------
static inline uint16_t
prvusKeyAvailable (void) {

  return xQueueLength (&xKeybQueue);
}

// -----------------------------------------------------------------------------
void
vKeybFlush (void) {

  vQueueFlush (&xKeybQueue);
  ucState = IDLE_STATE;
}

#  else
/************************** Version non bufferisée *****************************/

// ------------------------------------------------------------------------------
static inline void
prvvPutKey (void) {

  ucState = AVAILABLE_STATE;
}

// ------------------------------------------------------------------------------
static inline char
prvvGetKey (void) {
  char cKey = acKey[ucRow][ucCol];

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    if (xTaskIsStarted (xKeybTimer)) {

      ucState = FREEZE_STATE;
    } else {

      ucState = IDLE_STATE;
      vTaskSetInterval (xKeybTimer, xTaskConvertMs (KEYB_DEBOUNCE_DELAY));
    }
  }
  return cKey;
}

// ------------------------------------------------------------------------------
static inline uint16_t
prvusKeyAvailable (void) {
  uint16_t ucValue = 0;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    if (ucState == AVAILABLE_STATE)
      ucValue = 1;
  }
  return ucValue;
}

// -----------------------------------------------------------------------------
void
vKeybFlush (void) {

  ucState = IDLE_STATE;
}

/************************ Fin Version non bufferisée **************************/
#  endif

/* 
 * Service du clavier
 */
static void
prvvKeybTask (xTaskHandle xDummy __attribute__ ((unused))) {

  switch (ucState) {

    case IDLE_STATE:
      if (prvucReadCodeX () != 0) {
        /* une touche a été appuyée, passe en phase anti-rebond */
        ucState = DEBOUNCE_STATE;
      }
      break;

    case DEBOUNCE_STATE:
      ucCol = prvucCol ();
      if (ucCol < KEYB_WIDTH) {

        /* colonne valide, appui confirmé */
        prvvSetRowIn ();
        ucRow = prvucRow ();
        prvvSetRowOut ();
        if (ucRow < KEYB_HEIGHT) {
          /* ligne valide, touche appuyée, passage en phase lecture */
          prvvPutKey ();
          vTaskSetInterval (xKeybTimer, xTaskConvertMs (KEYB_REPEAT_DELAY));
          break;
        }
      }
      /* colonne ou ligne invalide, retour en mode attente */
      ucState = IDLE_STATE;
      break;

    case AVAILABLE_STATE:
      return; /* touche dispo, attente lecture */
      break;

    case FREEZE_STATE:
      ucState = IDLE_STATE;
      vTaskSetInterval (xKeybTimer, xTaskConvertMs (KEYB_DEBOUNCE_DELAY));
      break;
  }
  vTaskStart (xKeybTimer);
}

/* internal public functions ================================================ */
/* 
 * Initialisation du clavier
 */
void
vKeybInit (void) {

  prvvIfaceInit ();
  prvvSetRowOut ();
  xKeybTimer =
     xTaskCreate (xTaskConvertMs (KEYB_DEBOUNCE_DELAY), prvvKeybTask);
  vTaskStart (xKeybTimer);
}

/* 
 * Renvoie le nombre de touches appuyées
 */
uint16_t
usKeybHit (void) {

  return prvusKeyAvailable ();
}

/* 
 * Lecture d'une touche du clavier
 */
char
cKeybGetChar (void) {

  while (usKeybHit () == 0) ;

  return prvvGetKey ();
}

/* avr-libc stdio interface ================================================= */
static int
prvGetChar (FILE * pxStream) {

  return (unsigned int) cKeybGetChar ();
}

FILE xKeyb = FDEV_SETUP_STREAM (NULL, prvGetChar, _FDEV_SETUP_READ);

#endif /* AVRIO_KEYB_ENABLE defined */
/* ========================================================================== */
