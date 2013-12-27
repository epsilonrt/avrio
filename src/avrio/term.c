/**
 * @file term.c
 * @brief Entrées-Sorties Terminal texte
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
/*
  sizeof(int) 12345678901234567890  _DEC_MAX_DIGITS_
         8    -9223372036854775808        20
         4    -2147483648                 11 
         2    -32768                      6
         1    -128                        4
*/
#ifdef __SIZEOF_INT__
# if __SIZEOF_INT__ == 1
#   define _DEC_MAX_DIGITS_ 4
#   define _HEX_MAX_DIGITS_ 2
# elif __SIZEOF_INT__ == 2
#   define _DEC_MAX_DIGITS_ 6
#   define _HEX_MAX_DIGITS_ 4
# elif __SIZEOF_INT__ == 4
#   define _DEC_MAX_DIGITS_ 11
#   define _HEX_MAX_DIGITS_ 8
# elif __SIZEOF_INT__ == 8
#   define _DEC_MAX_DIGITS_ 20
#   define _HEX_MAX_DIGITS_ 16
# else
#error "Taille d'entier incorrecte !"
# endif
#else
#error "Taille d'entier inconnue !"
#endif

/* private variables ======================================================== */

/* private functions ======================================================== */
static char 
prvcHexToAscii (char hnibble) {
	register char c;

	c = hnibble & 0x0F; 
	return ( c >= 0x0A ? c + 0x37 : c + 0x30 );  /* Hex -> Ascii */
}

static int 
prviAsciiToHex (char cDigit) {

  if (cDigit >= 'a')
    cDigit -= 0x20;
  cDigit -= 0x30;
  if (cDigit > 9)
    cDigit -= 7;
	return cDigit;
}

/* internal public functions ================================================ */
 
// -----------------------------------------------------------------------------
// Lecture d'une chaîne de caractères
int
iTermGetStr (char * pcStr, int iSize, FILE * pxInputStream) {
  int iKey, iLen = 0;

  while (iSize--) {
  
    iKey = fgetc (pxInputStream);
    if ((iKey != '\n') && (iKey != '\r')) {
    
      *pcStr++ = iKey;
      iLen++;
    }
    else {
      
      break;
    }
  }
  *pcStr = 0;
  return iLen;
}

// -----------------------------------------------------------------------------
// Lecture d'un entier
int
iTermGetDec (FILE * pxInputStream, int iSize) {
  char pcStr[_DEC_MAX_DIGITS_ + 1];
  int iKey, iLen = 0;

  if ((iSize == 0) || (iSize > _DEC_MAX_DIGITS_)) {
  
    iSize = _DEC_MAX_DIGITS_;
  }
  
  while (iSize--) {
  
    iKey = fgetc (pxInputStream);
    if (isdigit(iKey)) {
    
      pcStr[iLen++] = iKey;
    }
    else {
      
      break;
    }
  }
  pcStr[iLen] = 0;
  
  if (iLen) {
  
    return atoi (pcStr);
  }
  return 0;
}

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
// -----------------------------------------------------------------------------
int
iTermGetHex (FILE * pxInputStream, int iSize) {
  char pcStr[_HEX_MAX_DIGITS_];
  int iKey, iLen = 0;

  if ((iSize == 0) || (iSize > _HEX_MAX_DIGITS_)) {
  
    iSize = _HEX_MAX_DIGITS_;
  }
  
  while (iSize--) {
  
    iKey = fgetc (pxInputStream);
    if (isxdigit(iKey)) {
    
      pcStr[iLen++] = iKey;
    }
    else {
      
      break;
    }
  }
  
  if (iLen--) {
    int iResult = 0; 
    uint8_t ucShift = 0;
    
    do {
    
      iResult += prviAsciiToHex (pcStr[iLen]) << ucShift;
      ucShift += 4;
    } while (iLen--);
    return iResult;
  }
  return 0;
}

// -----------------------------------------------------------------------------
void 
vTermPutHexByte (uint8_t ucByte, FILE * pxOutputStream) {

  (void) fputc (prvcHexToAscii (ucByte>>4), pxOutputStream);
  (void) fputc (prvcHexToAscii (ucByte), pxOutputStream);
}

// -----------------------------------------------------------------------------
void 
vTermPutHexWord (uint16_t usWord, FILE * pxOutputStream) {

  vTermPutHexByte (usWord>>8, pxOutputStream);
  vTermPutHexByte (usWord, pxOutputStream);
}

// -----------------------------------------------------------------------------
void 
vTermPutHexDword (uint32_t ulDword, FILE * pxOutputStream) {

  vTermPutHexWord (ulDword>>16, pxOutputStream);
  vTermPutHexWord (ulDword, pxOutputStream);
}

// -----------------------------------------------------------------------------
void 
vTermPutHexBytes (const uint8_t * pucBytes, int xLength, FILE * pxOutputStream) {

  while (xLength--) {
    
    vTermPutHexByte (*pucBytes++, pxOutputStream);  
  }
}

// -----------------------------------------------------------------------------
void 
vTermPutDec(uint16_t usValue, FILE * pxOutputStream) {
    char pcDisplayStr[6];
    uint8_t ucLoop = 5;
    
    pcDisplayStr[5] = '\0';
    
    while (ucLoop) {
        // 22 code bytes cheaper here than in the while above
        --ucLoop;

        pcDisplayStr[ucLoop] = (usValue % 10) + '0';
        usValue /= 10;
        
        // This prevents leading zeros
        if (usValue == 0)
            break;
    }
    
	
    fputs(&pcDisplayStr[ucLoop], pxOutputStream);
}

/* ========================================================================== */
