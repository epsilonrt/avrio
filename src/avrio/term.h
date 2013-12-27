/**
 * @file term.h
 * @brief Terminal texte
 *
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup term_module Terminal texte
 *
 *  Ce module permet de gérer un terminal à partir d'un flux d'entrée et de
 *  sortie de type caractère.\n
 *  @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 *  @version $Id$
 */
#ifndef _AVRIO_TERM_H_
#  define _AVRIO_TERM_H_

#  include <avrio/defs.h>
#  include <avrio/menu.h>
#  include <stdio.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/** @{ */
  /* macros ================================================================= */
  /* constants ============================================================== */
/**
 * @brief Flags d'un terminal
 */
   typedef enum {

  TERM_FLAG_NORMAL = 0x00,
  TERM_FLAG_INVERSE = 0x01,
  TERM_FLAG_BLINK = 0x02
} eTermFlag;

/* structures =============================================================== */
/**
 * @brief Taille d'un terminal
 */
typedef struct xTermSize {
  int iHSize; /**< Hauteur du terminal en nombre de caractères */
  int iVSize; /**< Largeur du terminal en nombre de caractères */
} xTermSize;

/**
 * @brief Position du curseur d'un terminal
 *
 * La position de départ est 0,0, ce qui correspond au coin en haut à gauche.
 */
typedef struct xTermPos {
  int iLine;   /**< Position verticale en nombre de caractères */
  int iColumn; /**< Position horizontale en nombre de caractères */
} xTermPos;

/**
 * @brief Terminal
 */
typedef struct xTerm {
  FILE *pxInputStream;   /**< Pointeur vers le flux d'entrée */
  FILE *pxOuputStream;   /**< Pointeur vers le flux de sortie */
  xMenu *pxMenu;         /**< Pointeur vers le menu */
  xTermSize xSize;       /**< Taille */
  xTermPos xPos;         /**< Position du curseur */
  eTermFlag eFlag;       /**< Flags du terminal */
} xTerm;

/* types ==================================================================== */
/* internal public functions ================================================ */
/**
 * @brief Lecture d'un entier en décimal
 * 
 * La saisie s'arrête dès qu'un caractère non numérique est saisie ou lorsque
 * le nombre maximal de caractère est atteind.
 * @param pxInputStream Flux d'entrée à utiliser pour la saisie
 * @param iDigits nombre maximal de caractères à attendre à la saisie. 
 *  0 signifie qu'il faut attendre le maximum de caractères possible. Cette
 *  valeur est bridée au nombre maximal de caractères possible (6 dans le cas
 *  où l'entier est codé sur 16 bits, 11 pour 32 bits ...). 
 * @return le nombre entier (0 par défaut: pas de chiffre correct saisi).
 */
int iTermGetDec (FILE * pxInputStream, int iDigits);

/**
 * @brief Lecture d'un entier en hexadécimal
 * 
 * La saisie s'arrête dès qu'un caractère non valide est saisi ou lorsque
 * le nombre maximal de caractère est atteind.
 * @param pxInputStream Flux d'entrée à utiliser pour la saisie
 * @param iDigits nombre maximal de caractères à attendre à la saisie. 
 *  0 signifie qu'il faut attendre le maximum de caractères possible. Cette
 *  valeur est bridée au nombre maximal de caractères possible (4 dans le cas
 *  où l'entier est codé sur 16 bits, 8 pour 32 bits ...). 
 * @return le nombre entier (0 par défaut: pas de chiffre correct saisi).
 */
int iTermGetHex (FILE * pxInputStream, int iDigits);

/**
 * @brief Lecture d'une chaîne de caractères
 *
 * Lit et recopie dans pcStr tous les caractères :
 * - jusqu'à ce que un CR ou LF soit saisi (non mémorisé) ou
 * - jusqu'à ce que le nombre maximal de caractères iSize soit atteind.
 * .
 * Il est du ressort de l'utilisateur de réserver suffisament de place dans
 * pcStr pour y stocker iSize+1 caractères.
 * @param pcStr pointeur sur la zone où socker les caracrères.
 * @param iSize nombre maximal de caractères à attendre à la saisie.
 * @param pxInputStream Flux d'entrée à utiliser pour la saisie.
 * @return le nombre de caractères stockés (sans le zéro final), -1 si erreur.
 */
int iTermGetStr (char * pcStr, int iSize, FILE * pxInputStream);

/**
 * @brief Affiche un octet sous forme de 2 digits hexadécimaux
 */
void vTermPutHexByte (uint8_t ucByte, FILE * pxOutputStream);

/**
 * @brief Affiche un mot de 16 bits sous forme de 4 digits hexadécimaux
 */
void vTermPutHexWord (uint16_t usWord, FILE * pxOutputStream);

/**
 * @brief Affiche un mot de 32 bits sous forme de 8 digits hexadécimaux
 */
void vTermPutHexDword (uint32_t ulDword, FILE * pxOutputStream);

/**
 * @brief Affiche une suite d'octets sous forme de digits hexadécimaux
 */
void vTermPutHexBytes (const uint8_t * pucBytes, int xLength, FILE * pxOutputStream);

/**
 * @brief Affiche un mot de 16 bits sous forme de décimal
 */
void vTermPutDec(uint16_t usValue, FILE * pxOutputStream);

/* public variables ========================================================= */
/* inline public functions ================================================== */

#  if defined(__DOXYGEN__)
/**
 * @brief Modifie le menu d'un terminal
 */
inline void vTermSetMenu (xTerm * pxTerm, xMenu * pxMenu);

/**
 * @brief Renvoie le menu d'un terminal
 */
inline xMenu *pxTermGetMenu (xTerm * pxTerm);

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
// ------------------------------------------------------------------------------
static inline void
vTermSetMenu (xTerm * pxTerm, xMenu * pxMenu) {

  pxTerm->pxMenu = pxMenu;
}

// ------------------------------------------------------------------------------
static inline xMenu *
pxTermGetMenu (xTerm * pxTerm) {

  return pxTerm->pxMenu;
}

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_TERM_H_ */
