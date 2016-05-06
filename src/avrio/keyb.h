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
 * @file keyb.h
 * @brief Clavier matriciel sur port PIO
 */
#ifndef _AVRIO_KEYB_H_
#  define _AVRIO_KEYB_H_

#  include <stdio.h>
#  include <avrio/defs.h>

__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 * @defgroup keyb_module Clavier matriciel
 *
 * Ce module permet de gérer clavier organisé sous forme d'une matrice XY.\n
 * La macro AVRIO_KEYB_ENABLE doit être définie dans avrio-config.h et dans ce
 * cas, la partie interface matérielle doit être implémentée dans le fichier
 * avrio-board-keyboard.h se trouvant dans le même répertoire.
 * @{
 * @example keyboard/demo_keyboard.c
 * Scrute le clavier et affiche les touches sur la liaison série.
 */
  /* internal public functions ============================================== */
/**
 * @brief Initialisation du clavier
 */
void vKeybInit (void);

/**
 * @brief Lecture d'une touche du clavier
 *
 * Cette fonction est bloquante.
 * @return Code de la touche (suivant KEYB_ARRAY dans avrio-config.h)
 */
char cKeybGetChar (void);

/**
 * @brief Renvoie le nombre de touches appuyées
 */
uint16_t usKeybHit (void);

/**
 * @brief Vide le tampon de réception
 */
void vKeybFlush (void);

/* public variables ========================================================= */
/**
 * @brief Descripteur de fichier du clavier permettant son utilisation avec les fonctions de la libc.
 *
 * Pour utiliser le clavier comme entrée et sortie, on mettra au début
 * du main() :
 * @code
 * int main(void) {
 *   vKeybInit ();
  *   stdin  = &xKeyb;
 * ...
 * @endcode
 * On pourra dans le cas contraire passer &xKeyb aux fonctions fscanf, fgetc ... :
 * @code
 * int choix;
 * ...
 * fscanf (&xKeyb, "%d", &choix);
 * ...
 * @endcode
 */
extern FILE xKeyb;

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

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

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_KEYB_H_ */
