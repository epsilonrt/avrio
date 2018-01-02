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
 * @file spi.h
 * @brief Bus SPI
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_SPI_H_
#define _AVRIO_SPI_H_

#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
  /* ======================================================================== */
/**
 * @addtogroup dev_group
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
 *  SCLK. Pour désigner l'esclave parmi N, le maître dispose de N signaux SS
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
 *  MOSI, MISO, SCK (SCLK) et SS. \n
 *  Pour utiliser ce module, il faut ajouter AVRIO_SPI_ENABLE àà la variable
 *  AVRIO_CONFIG dans le Makefile. \n
 *  La configuration du module est effectuée dans le fichier avrio-board-afsk.h
 *  Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 *  (répertoire du projet ou dossier board normalement).
 *
 *  @{
 * @example net/spi/demo_spi.c
 * Demo bus SPI
 * Ce programme écrit un octet dans un registre et relit cet octet en
 * vérifiant que la valeur lue est identique à celle écrite. \n
 * Si cela fonctionne, la LED1 clignote régulièrement, sinon elle flashe
 * rapidement.
 *
 *    @defgroup spiboard_module Configuration du module SPI
 */

/* constants ================================================================ */
#define SPI_CPHA    0x01 /**< phase horloge: 0 premier front (montant si CPOL=0), 1 deuxième front (descendant si CPOL=0) */
#define SPI_CPOL    0x02 /**< polarité horloge: 0 horloge sur état haut, 1 horloge sur état bas */
#define SPI_REG_RW_BIT 7 /**< bit lecture-écriture pour les fonctions de la famille Reg */

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

    
/**
 * @enum eSpiMode
 * @brief Mode SPI
 * 
 * Différents modes \n  <hr>
  | SPI Mode | POL | PHA | 1er front                   | 2ème front                  |
  | :------: | :-: | :-: | :-------------------------- | :-------------------------- |
  | 0        |  0  |  0  | montant, échantillonnage    | descendant, changement état |
  | 1        |  0  |  1  | montant, changement état    | descendant, échantillonnage |
  | 2        |  1  |  0  | descendant, échantillonnage | montant, changement état    |
  | 3        |  1  |  1  | descendant, changement état | montant, échantillonnage    |
 * 
 * cf [Wikipedia](https://fr.wikipedia.org/wiki/Serial_Peripheral_Interface) 
 */
typedef enum {
  eSpiMode0 = (0|0),
  eSpiMode1 = (0|SPI_CPHA),
  eSpiMode2 = (SPI_CPOL|0),
  eSpiMode3 = (SPI_CPOL|SPI_CPHA),
  eSpiModeNotSet = -1 /**< Non modifié */
} eSpiMode;

/**
 * @enum eSpiBitOrder
 * @brief Ordre des bits
 */
typedef enum {
  eSpiBitOrderMsb = 0, /**< MSB en premier */
  eSpiBitOrderLsb = 1, /**< LSB en premier */
  eSpiBitOrderNotSet = -1 /**< Non modifié */
} eSpiBitOrder;

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
 * @brief Modification du mode de fonctionnement
 * @param eMode mode choisi
 */
void vSpiSetMode (eSpiMode eMode);

/**
 * @brief Modification de l'ordre des bites
 * @param eOrder ordre choisi
 */
void vSpiSetBitOrder (eSpiBitOrder eOrder);

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

/**
 * @brief Lecture/Ecriture d'un bloc octets
 * 
 * @param tx_buffer pointeur vers la zone de stockage des octets
 * @param tx_len nombre d'octets à écrire
 * @param rx_buffer pointeur vers la zone de stockage des octets, la taille doit
 *        être suffisante pour y stocker le nombre d'octets demandés.
 * @param rx_len nombre d'octets à lire
 */
void vSpiMasterXfer (const uint8_t * tx_buffer, uint8_t tx_len, uint8_t *rx_buffer, uint8_t rx_len);

/**
 * @brief Lecture d'un registre 8 bits
 *
 * Cette fonction réalise une transmission de l'adresse du registre à lire,
 * suivie d'une lecture d'un octet.\n
 * @param reg adresse du registre
 * @return la valeur de l'octet
 */
uint8_t ucSpiMasterReadReg8 (uint8_t reg);

/**
 * @brief Ecriture d'un registre 8 bits
 *
 * @param reg adresse du registre
 * @param data valeur de l'octet
 */
void vSpiMasterWriteReg8 (uint8_t reg, uint8_t data);

/**
 * @brief Ecriture d'un bloc de registres
 *
 * @param reg adresse du premier registre
 * @param buffer pointeur vers la zone de stockage des octets
 * @param len nombre d'octets à écrire
 */
void vSpiMasterWriteRegBlock (uint8_t reg, const uint8_t * buf, uint8_t len);

/**
 * @brief Lecture d'un bloc de registres
 *
 * @param reg adresse du premier registre
 * @param buffer pointeur vers la zone de stockage des octets
 * @param len nombre d'octets à lire
 */
void vSpiMasterReadRegBlock (uint8_t reg, uint8_t * buf, uint8_t len);

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
   * @brief Affirme la broche SS (SS = 0 -> validé)
   */
  inline void vSpiSetSs (void);

  /**
   * @brief Désaffirme la broche SS (SS = 1 -> invalidé)
   */
  inline void vSpiClearSs (void);

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
  INLINE void
  vSpiClearIF (void) {
    register uint8_t ucDummy __attribute__ ((unused));

    ucDummy = SPSR;
    ucDummy = SPDR;  // clear irq flag
  }

  // ---------------------------------------------------------------------------
  INLINE void
  vSpiMasterWrite (uint8_t ucByte) {

    (void) ucSpiMasterWriteRead (ucByte);
  }

  // ---------------------------------------------------------------------------
  INLINE uint8_t
  ucSpiMasterRead (void) {

    return ucSpiMasterWriteRead (0);
  }

  // ---------------------------------------------------------------------------
  INLINE void
  vSpiEnable (void) {

    SPCR |= _BV(SPE);
    vSpiClearIF ();
  }

  // ---------------------------------------------------------------------------
  INLINE void
  vSpiDisable (void) {

    SPCR &= ~_BV(SPE);
  }

#  include "avrio-config.h"

#  ifdef AVRIO_SPI_ENABLE
/* ========================================================================== */
#   include "avrio-board-spi.h"

  // ---------------------------------------------------------------------------
  INLINE void
  vSpiSetSsAsInput (void) {

    vSpiBoardSetSsAsInput ();
  }

  // ---------------------------------------------------------------------------
  INLINE void
  vSpiSetSsAsOutput (void) {

    vSpiBoardSetSsAsOutput ();
  }
#  endif /* AVRIO_SPI_ENABLE defined */
#  endif /* __DOXYGEN__ not defined */

  /* ======================================================================== */
#if defined(__cplusplus)
  }
#endif

/* ========================================================================== */
#endif  /* _AVRIO_SPI_H_ not defined */
