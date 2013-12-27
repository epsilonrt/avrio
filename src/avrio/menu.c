/**
 * @file menu.c
 * @brief Menus hiérarchiques textuels
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include <avrio/term.h>
#include "avrio-config.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* constants ================================================================ */

/* private variables ======================================================== */

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
prvvCopyItem (xTerm * pxTerm, int iItem, xMenuItem * pxItem) {

  if (pxTerm->pxMenu->eFlag & MENU_FLAG_PGM_ITEM) {
    const xMenuItem *pxChosenItem = &pxTerm->pxMenu->pxItem[iItem];

    memcpy_P (pxItem,
              // (PGM_VOID_P) pgm_read_word (),
              pxChosenItem, sizeof (xMenuItem));
  } else {

    memcpy (pxItem, &pxTerm->pxMenu->pxItem[iItem], sizeof (xMenuItem));
  }
}

// ------------------------------------------------------------------------------
// Affiche le texte correspondant à un item du menu index
static void
prvvPrintItem (xTerm * pxTerm, int iItem) {
  xMenuItem xItem;

  prvvCopyItem (pxTerm, iItem, &xItem);

  fprintf_P (pxTerm->pxOuputStream, PSTR ("%d%"), iItem + 1);
  if (xItem.xRaw.eFlag & MENU_FLAG_ACTION) {

    fputc ('-', pxTerm->pxOuputStream);
  } else {

    fputc ('+', pxTerm->pxOuputStream);
  }

  if (pxTerm->pxMenu->eFlag & MENU_FLAG_PGM_NAME) {

    fputs_P (xItem.xRaw.pcName, pxTerm->pxOuputStream);
  } else {

    fputs (xItem.xRaw.pcName, pxTerm->pxOuputStream);
  }
  fputc ('\n', pxTerm->pxOuputStream);
}

// ------------------------------------------------------------------------------
// Affiche les items du menu index
void
prvvPrintMenu (xTerm * pxTerm) {
  int iItem;

  fputc ('\n', pxTerm->pxOuputStream);
  if (pxTerm->pxMenu->pcName) {

    fputs_P (PSTR("--- "), pxTerm->pxOuputStream);
    if (pxTerm->pxMenu->eFlag & MENU_FLAG_PGM_NAME) {

      fputs_P (pxTerm->pxMenu->pcName, pxTerm->pxOuputStream);
    } else {

      fputs (pxTerm->pxMenu->pcName, pxTerm->pxOuputStream);
    }
    fputc ('\n', pxTerm->pxOuputStream);
  }

  for (iItem = 0; iItem < pxTerm->pxMenu->iSize; iItem++) {

    prvvPrintItem (pxTerm, iItem);
  }

  if (pxTerm->pxMenu->pxUp != 0) {

    fprintf_P (pxTerm->pxOuputStream, PSTR("%d+Precedent\n"), iItem + 1);
  }
}

// ------------------------------------------------------------------------------
// Renvoie l'item choisi par l'utilisateur dans le menu index
int
prviGetChosenItem (xTerm * pxTerm) {
  int iItem = 0;
  int iSize = pxTerm->pxMenu->iSize;

  if (pxTerm->pxMenu->pxUp != 0) {

    iSize++;
  }

  do {

    fprintf_P (pxTerm->pxOuputStream, PSTR("Votre Choix [1-%d] ? "), iSize);
    iItem = iTermGetDec (pxTerm->pxInputStream, 0);
    // fputc ('\n', pxTerm->pxOuputStream);
  } while ((iItem < 1) || (iItem > iSize));

  return iItem - 1;
}

// ------------------------------------------------------------------------------
// Exécute l'action correspondant à un item du menu index
static int
prveExecuteItem (xTerm * pxTerm, int iItem) {

  if (iItem == pxTerm->pxMenu->iSize) {

    /* remonte vers le menu supérieur */
    pxTerm->pxMenu = pxTerm->pxMenu->pxUp;
  } else {
    xMenuItem xItem;

    prvvCopyItem (pxTerm, iItem, &xItem);

    if (xItem.xRaw.eFlag == MENU_FLAG_ACTION) {

      /* exécute l'action */
      pvMenuFunction pvFunc = (pvMenuFunction) xItem.xRaw.pvItem;

      if (pvFunc)
        pvFunc ();
    } else {

      /* descend dans le menu inférieur */
      pxTerm->pxMenu = (xMenu *) xItem.xRaw.pvItem;
    }
  }

  return ERROR_SUCCESS;
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
int
eMenuPoll (xTerm * pxTerm) {
  int iItem;

  prvvPrintMenu (pxTerm);
  iItem = prviGetChosenItem (pxTerm);
  return prveExecuteItem (pxTerm, iItem);
}

#include "term.c"

/* ========================================================================== */
