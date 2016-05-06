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
 * @file menu.h
 * @brief Menus hiérarchiques textuels
 */
#ifndef _AVRIO_MENU_H_
#  define _AVRIO_MENU_H_

#  include <avrio/defs.h>
#  include <avr/pgmspace.h>
#  include <stdio.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup util_group
 * @{
 *
 *  @defgroup menu_module Menus hiérarchiques textuels
 *
 *  Ce module implémente des menus organisés sous forme d'arbres hiérarchisés.\n
 *  Chaque élément d'un menu peut déclencher une action ou déclencher
 *  l'affichage d'un nouveau menu (un sous-menu).\n
 *  La structure des menus est décrite sous forme de variables statiques
 *  stockées en mémoire programme.
 *  @{
 */
/* constants ================================================================ */
/**
 * @enum eMenuFlag
 * @brief Flags d'un menu
 */
   typedef enum {

  MENU_FLAG_MENU = 0x00,        /**< L'item est un sous-menu qui peut être affiché */
  MENU_FLAG_ACTION = 0x01,      /**< L'item est une action qui lance une fonction */
  MENU_FLAG_PGM_ITEM = 0x02,    /**< L'item est stocké en mémoire programme */
  MENU_FLAG_PGM_NAME = 0x04,    /**< Le nom est stocké en mémoire programme */
  MENU_FLAG_PGM_ALL = MENU_FLAG_PGM_ITEM + MENU_FLAG_PGM_NAME     /**< L'item et le nom sont stockés en mémoire programme */
} eMenuFlag;

/* types ==================================================================== */
/**
 * @brief Fonction appelée lorsque une action est sélectionnée.
 */
typedef void (*pvMenuFunction) (void);

/* structures =============================================================== */
#  if !defined(__DOXYGEN__)
struct xTerm;
#  endif

/**
 * @union xMenuItem
 * @brief Elément d'un menu
 *
 * C'est une union car un item peut être de type différent.
 */
typedef union xMenuItem {
  /**
   * @struct xSubMenu
   * @brief Item de type Menu
   */
  struct {
    const char *pcName;    /**< Texte affiché */
    eMenuFlag eFlag;       /**< Flags de l'item */
    struct xMenu *pxMenu;  /**< Pointeur sur le menu */
  } xSubMenu;

  /**
   * @struct xAction
   * @brief Item de type Action
   */
  struct {
    const char *pcName;    /**< Texte affiché */
    eMenuFlag eFlag;       /**< Flags de l'item */
    pvMenuFunction pvFunc; /**< Fonction appelée lorsque l'item est sélectionné */
  } xAction;

  /**
   * @struct xRaw
   * @brief Champs communs au différents types
   */
  struct {
    const char *pcName;   /**< Texte affiché */
    eMenuFlag eFlag;      /**< Flags de l'item */
    void *pvItem;         /**< Pointeur sur un item générique */
  } xRaw;

} xMenuItem;

/**
 * @struct xMenu
 * @brief Un menu hiérarchisé
 */
typedef struct xMenu {
  const char *pcName;       /**< Titre du menu */
  const xMenuItem *pxItem;  /**< Tableau des items du menu */
  int iSize;                /**< Nombre d'items dans le tableau */
  struct xMenu *pxUp;       /**< Menu parent, NULL si aucun (menu racine) */
  eMenuFlag eFlag;
} xMenu;

/* internal public functions ================================================ */
/**
 * @brief Boucle de traitement des menus
 *
 * Cette fonction affiche le menu pointé par le champs pxMenu de pxTerm, puis
 * attends que l'utilisateur choisisse un numéro d'item.\n
 * - Si l'item choisi est une action, la fonction associée est exécutée.
 * - Sinon, le nouveau menu est sélectionné et sera affiché à la prochaine
 * itération.
 * .
 *
 * Le champs pxMenu de pxTerm doit pointer sur une variable xMenu correctement
 * initialisée.
 *
 * @param pxTerm Pointeur sur le terminal sur lequel s'affiche le menu
 * @return 0 en cas de succès, le code erreur sinon.
 */
int eMenuPoll (struct xTerm *pxTerm);

/* macros =================================================================== */
#  if defined(__DOXYGEN__)
/**
 * @brief Renvoie le nombre d'items d'un menu
 */
#    define MENU_SIZEOF(menu)

  /**
   *   @}
   * @}
   */
#  else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#    define MENU_SIZEOF(menu) (sizeof(menu)/sizeof(xMenuItem))

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_MENU_H_ */
