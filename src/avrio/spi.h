/**
 * @file spi.h
 * @brief Bus SPI
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_SPI_H_
#define _AVRIO_SPI_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup net_group
 * @{
 *
 *  @defgroup spi_module Bus SPI
 *  Ce module permet d'utiliser la liaison SPI des MCU AVR. Cette liaison série
 *  synchrone est constituée de 3 signaux:
 *  - MISO: Master In Slave Out
 *  - MOSI: Master Out Slave In
 *  - SCLK: Serial Clock
 *  .
 *  Il s'agit d'une liaison maître-esclave. Le maître contrôle la ligne d'horloge
 *  SCK. Pour désigner l'esclave parmi N, le maître dispose de N signaux SS
 *  (Slave Select) actifs à l'état bas. \n
 *  \image html SPI_three_slaves.png "Un maître peut contrôler plusieurs esclaves"
 *  \image latex SPI_three_slaves.eps "Un maître peut contrôler plusieurs esclaves" width=7cm
 *  À chaque coup d'horloge le maître et l'esclave s'échangent un bit.
 *  Après huit coups d'horloges le maître a transmis un octet à l'esclave et
 *  vice versa. La vitesse de l'horloge est réglée selon des caractéristiques
 *  propres aux périphériques. \n
 *  \image html SPI_timing_diagram.png "Chronologie d'un échange SPI"
 *  \image latex SPI_timing_diagram.eps "Chronologie d'un échange SPI" width=7cm
 *  Les MCU AVR peuvent être maître ou/et esclave et disposent donc des broches
 *  MOSI, MISO, SCK et SS. \n
 *  Pour utiliser ce module, il faut ajouter AVRIO_SPI_ENABLE àà la variable
 *  AVRIO_CONFIG dans le Makefile. \n
 *  La configuration du module est effectuée dans le fichier avrio-board-afsk.h
 *  Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 *  (répertoire du projet ou dossier board normalement).
 *
 *  @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 *  @copyright GNU Lesser General Public License version 3
 *             <http://www.gnu.org/licenses/lgpl.html>
 *  @{
 *    @example spi/demo_spi.c
 *    Ecrit un octet dans un registre et relit cet octet en vérifiant que la
 *    valeur lue est identique à celle écrite.
 *
 *    @defgroup spiboard_module Configuration du module SPI
 */

/* constants ================================================================ */
/**
 * @enum eSpiFclkDiv
 * @brief Facteur de division de l'horloge SPI
 */
typedef enum {

  SPI_DIV2   = 0x04,  /**< F_SPI = (F_CPU / 2) */
  SPI_DIV4   = 0x00,  /**< F_SPI = (F_CPU / 4) */
  SPI_DIV8   = 0x05,  /**< F_SPI = (F_CPU / 8) */
  SPI_DIV16  = 0x01,  /**< F_SPI = (F_CPU / 16) */
  SPI_DIV32  = 0x06,  /**< F_SPI = (F_CPU / 32) */
  SPI_DIV64  = 0x02,  /**< F_SPI = (F_CPU / 64) */
  SPI_DIV128 = 0x03   /**< F_SPI = (F_CPU / 128) */

} eSpiFclkDiv;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module SPI en mode maître.
 *
 * A l'issue le bus SPI est validé et prêt à transmettre.
 * @param eFclkDiv Facteur de division de l'horloge SPI
 * @warning La broche /SS doit être configurée en sortie ou en entrée à l'état
 *  haut \b AVANT l'appel de cette fonction grâce aux fonctions
 *  \ref vSpiSetSsAsInput() ou \ref vSpiSetSsAsOutput(). \n
 *  Dans le cas contraire, le mode maître ne sera pas enclenché.
 */
void vSpiMasterInit (eSpiFclkDiv eFclkDiv);

/**
 * @brief Transmission SPI
 *
 * Décale ucByte dans le registre à décalage de l'esclave et récupère l'octet
 * qui en sort.
 * @param ucByte Octet à écrire dans le registre à décalage de l'esclave (0xFF si aucun)
 * @return Octet lu en sortie du registre à décalage de l'esclave
 * @warning La broche SS de l'esclave doit être affirmée avant l'appel à cette
 * fonction.
 */
uint8_t ucSpiMasterWriteRead (uint8_t ucByte);

#  if defined(__DOXYGEN__)
  /*
   * __DOXYGEN__ defined
   * Partie documentation ne devant pas être compilée.
   * =========================================================================
   */
  /**
   * @brief Ecriture SPI
   *
   * Décale ucByte dans le registre à décalage de l'esclave.
   * @param ucByte Octet à écrire dans le registre à décalage de l'esclave.
   * @warning La broche SS de l'esclave doit être affirmée avant l'appel à cette
   * fonction.
   */
  inline void vSpiMasterWrite (uint8_t ucByte);

  /**
   * @brief Lecture SPI
   *
   * Décale 0xFF dans le registre à décalage de l'esclave et récupère l'octet
   * qui en sort.
   * @return Octet lu en sortie du registre à décalage de l'esclave
   * @warning La broche SS de l'esclave doit être affirmée avant l'appel à cette
   * fonction.
   */
  inline uint8_t ucSpiMasterRead (void);

  /**
   * @brief Configure la broche SS en entrée avec résistance de pull-up
   */
  inline void vSpiSetSsAsInput (void);

  /**
   * @brief Configure la broche SS en sortie à l'état haut
   */
  inline void vSpiSetSsAsOutput (void);

  /**
   * @brief Valide le bus SPI et remets à zéro le drapeau d'interruption SPI
   */
  inline void vSpiEnable (void);

  /**
   * @brief Invalide le bus SPI
   */
  inline void vSpiDisable (void);

  /**
   * @brief Remets à zéro le drapeau d'interruption SPI
   */
  inline void vSpiClearIF (void);

  /**
   *   @}
   * @}
   */
#  else
  /*
   * __DOXYGEN__ not defined
   * Partie ne devant pas être documentée.
   * =========================================================================
   */
#  include <avr/io.h>
  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void
  vSpiClearIF (void)) {
    register uint8_t ucDummy;

    ucDummy = SPSR;
    ucDummy = SPDR;  // clear irq flag
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void
  vSpiMasterWrite (uint8_t ucByte)) {

    (void) ucSpiMasterWriteRead (ucByte);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(uint8_t
  ucSpiMasterRead (void)) {

    return ucSpiMasterWriteRead (0xFF);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void
  vSpiEnable (void)) {

    SPCR |= _BV(SPE);
    vSpiClearIF ();
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void
  vSpiDisable (void)) {

    SPCR &= ~_BV(SPE);
  }

#  include "avrio-config.h"

#  ifdef AVRIO_SPI_ENABLE
/* ========================================================================== */
#   include "avrio-board-spi.h"

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void
  vSpiSetSsAsInput (void)) {

    vSpiBoardSetSsAsInput ();
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE(void
  vSpiSetSsAsOutput (void)) {

    vSpiBoardSetSsAsOutput ();
  }
#  endif /* AVRIO_SPI_ENABLE defined */
#  endif /* __DOXYGEN__ not defined */

  /* ======================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

/* ========================================================================== */
#endif  /* _AVRIO_SPI_H_ not defined */
