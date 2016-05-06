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
 * @file ds1621.h
 * @brief Thermostat I2c DS1621 de DALLAS/MAXIM
 *
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#ifndef _AVRIO_DS1621_H_
#define _AVRIO_DS1621_H_

#include <avrio/twi.h>

__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup sensor_group
 * @{
 *
 *  @defgroup ds1621_module Thermostat DS1621
 *
 *  Ce module permet la commande d'un circuit thermostat DS1621 de MAXIM/DALLAS.
 *  @{
 */
/* macros =================================================================== */
/* constants ================================================================ */
#define DS1621_BASE 0x90 /**< Adresse I2c de base des circuits DS1621 */
#define DS1621_TEMP_ERROR -32768

typedef enum {
  DONE    = _BV(7), /**< Indique que la conversion est terminée */
  THF     = _BV(6), /**< Indique que la température de seuil haute est dépassée */
  TLF     = _BV(5), /**< Indique que la température de seuil basse est dépassée */
  NVB     = _BV(4), /**< Indique qu'une opération d'écriture est en cours dans la nvram */
  POL     = _BV(1), /**< Permet de sélectionner la polarité de la sortie TOUT (1 active état haut)*/
  ONESHOT = _BV(0), /**< Permet de sélectionner le mode One Shot (1) ou continu (0) */

  TAF     = THF | TLF, /**< Tous les drapeaux */
  DS1621_DEFAULT_CONFIG = 0 /**< Configuration par défaut */
} eDs1621Cmd;

/* internal public functions ================================================ */

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Initialise le circuit DS1621 et démarre la conversion
 * @param xDeviceAddr adresse du DS1621 esclave
 * @param ucConfig Configuration du DS1621
 * @warning Le contrôleur de bus I2C doit être initialisé avant.
 */
void vDs1621Init (xTwiDeviceAddr xDeviceAddr, uint8_t ucConfig);

/**
 * @brief Démarrer la conversion
 * En mode 1SHOT, le circuit retourne en mode sommeil à l'issue
 * de la conversion. En mode continu, une nouvelle conversion est relancée
 * dès la fin de la précédente.
 * @param xDeviceAddr adresse du DS1621 esclave
 */
inline void vDs1621Start (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Arrêter la conversion
 * La conversion en cours se termine avant l'arrêt. Cette fonction n'est
 * donc utilie qu'en mode continu.
 * @param xDeviceAddr adresse du DS1621 esclave
 */
inline void vDs1621Stop (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lecture de la dernière erreur
 * @return TWI_SUCCESS si la trame a pu être transmise, le code erreur sinon.
 */
inline eTwiStatus eDs1621LastError (void);

/**
 * @brief Lecture de la dernière température mesurée
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return la température en dixièmes de degrés Celcius ou DS1621_TEMP_ERROR
 * en cas d'erreur. La résolution est de 0.5°C
 */
inline int16_t iDs1621Temp (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lecture de la dernière température mesurée
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return la température en LSB (0.5°C)  ou DS1621_TEMP_ERROR
 */
inline int16_t iDs1621RawTemp(xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lecture de la dernière température mesurée (haute résolution)
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return la température en degrés Celcius ou NAN en cas d'erreur.
 */
double dDs1621HiResTemp (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Indique si la conversion est terminée
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return true si la conversion est terminée (utile en mode 1SHOT).
 */
inline bool xDs1621IsDone (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Indique si l'écriture dans les registres TH/TL est possible.
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return true si la mémoire EEPROM est occupée.
 */
inline bool xDs1621MemIsBusy (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Ecrire dans le registre de seuil haut
 * Cette fonction est protégée avec \c xDs1621MemIsBusy()
 * @param xDeviceAddr adresse du DS1621 esclave
 * @param iTh Température de seuil haut en dixièmes de degrés
 */
void vDs1621SetTh (xTwiDeviceAddr xDeviceAddr, int16_t iTh);

/**
 * @brief Lire le registre de seuil haut
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Température de seuil haut en dixièmes de degrés ou DS1621_TEMP_ERROR en cas d'erreur
 */
inline int16_t iDs1621Th (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lire le registre de seuil haut
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Température de seuil haut en LSB(0.5°C) ou DS1621_TEMP_ERROR en cas d'erreur
 */
inline int16_t iDs1621RawTh(xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Ecrire dans le registre de seuil bas
 * Cette fonction est protégée avec \c xDs1621MemIsBusy()
 * @param xDeviceAddr adresse du DS1621 esclave
 * @param iTl Température de seuil bas en dixièmes de degrés
 */
void vDs1621SetTl (xTwiDeviceAddr xDeviceAddr, int16_t iTl);

/**
 * @brief Lire le registre de seuil bas
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Température de seuil bas en dixièmes de degrés ou DS1621_TEMP_ERROR en cas d'erreur
 */
inline int16_t iDs1621Tl (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lire le registre de seuil bas
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Température de seuil bas en LSB (0.5°C) ou DS1621_TEMP_ERROR en cas d'erreur
 */
inline int16_t iDs1621RawTl(xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lecture du registre d'état et de configuration (AC).
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Valeur du registre.
 */
inline uint8_t ucDs1621Status (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Remise à zéro des drapeaux THF et/ou TLF
 * @param xDeviceAddr adresse du DS1621 esclave
 * @param ucFlags masque des bits à remettre à zéro (THF, TLF)
 */
void vDs1621ClrFlags (xTwiDeviceAddr xDeviceAddr, uint8_t ucFlags);

/**
 * @brief Lire le registre compteur
 * Fonction utile si une grande précision de mesure est nécessaire. cf la page 4
 * du datasheet du DS1621 (version 090905).
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Valeur du registre compteur
 */
inline uint8_t ucDs1621Counter (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lire le registre de pente
 * Fonction utile si une grande précision de mesure est nécessaire. cf la page 4
 * du datasheet du DS1621 (version 090905).
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Valeur du registre de pente
 */
inline uint8_t ucDs1621Slope (xTwiDeviceAddr xDeviceAddr);

/**
 *   @}
 * @}
 */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
/* macros =================================================================== */
/* Commandes DS1621 */
#define DS1621_RDTEMP 0xAA
#define DS1621_RWTH   0xA1
#define DS1621_RWTL   0xA2
#define DS1621_RWCONF 0xAC
#define DS1621_RDCNT  0xA8
#define DS1621_RDSLOP 0xA9
#define DS1621_STARTC 0xEE
#define DS1621_STOPC  0x22

/* internal public functions ================================================ */
/* Fonctions de haut niveau */
void vDs1621Init (xTwiDeviceAddr xDeviceAddr, uint8_t ucConfig);
void vDs1621SetTh (xTwiDeviceAddr xDeviceAddr, int16_t iTh);
void vDs1621SetTl (xTwiDeviceAddr xDeviceAddr, int16_t iTl);
void vDs1621ClrFlags (xTwiDeviceAddr xDeviceAddr, uint8_t ucFlags);
double dDs1621HiResTemp (xTwiDeviceAddr xDeviceAddr);

/* Fonctions de bas niveau */
int16_t iDs1621ReadTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);
int16_t iDs1621ReadRawTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);
void vDs1621WriteTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd, int16_t iTemp);
uint8_t ucDs1621ReadByte(xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);
void vDs1621SendCmd (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);

/* public variables ========================================================= */
extern eTwiStatus eDs1621LastErrorValue;

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
INLINE uint8_t
ucDs1621Status(xTwiDeviceAddr xDeviceAddr) {

  return ucDs1621ReadByte(xDeviceAddr, DS1621_RWCONF);
}

// -----------------------------------------------------------------------------
INLINE bool
xDs1621IsDone (xTwiDeviceAddr xDeviceAddr) {

  return ((ucDs1621Status (xDeviceAddr) & DONE) != 0);
}

// -----------------------------------------------------------------------------
INLINE bool
xDs1621MemIsBusy (xTwiDeviceAddr xDeviceAddr) {

  return ((ucDs1621Status (xDeviceAddr) & NVB) != 0);
}

// -----------------------------------------------------------------------------
INLINE void
vDs1621Start(xTwiDeviceAddr xDeviceAddr) {

  vDs1621SendCmd (xDeviceAddr, DS1621_STARTC);
}

// -----------------------------------------------------------------------------
INLINE void
vDs1621Stop(xTwiDeviceAddr xDeviceAddr) {

  vDs1621SendCmd (xDeviceAddr, DS1621_STOPC);
}

// -----------------------------------------------------------------------------
INLINE int16_t
iDs1621Temp(xTwiDeviceAddr xDeviceAddr) {

  return iDs1621ReadTemp(xDeviceAddr, DS1621_RDTEMP);
}

// -----------------------------------------------------------------------------
INLINE int16_t
iDs1621Th(xTwiDeviceAddr xDeviceAddr) {

  return iDs1621ReadTemp(xDeviceAddr, DS1621_RWTH);
}

// -----------------------------------------------------------------------------
INLINE int16_t
iDs1621Tl(xTwiDeviceAddr xDeviceAddr) {

  return iDs1621ReadTemp(xDeviceAddr, DS1621_RWTL);
}

// -----------------------------------------------------------------------------
INLINE int16_t
iDs1621RawTemp(xTwiDeviceAddr xDeviceAddr) {

  return iDs1621ReadRawTemp(xDeviceAddr, DS1621_RDTEMP);
}

// -----------------------------------------------------------------------------
INLINE int16_t
iDs1621RawTh(xTwiDeviceAddr xDeviceAddr) {

  return iDs1621ReadRawTemp(xDeviceAddr, DS1621_RWTH);
}

// -----------------------------------------------------------------------------
INLINE int16_t
iDs1621RawTl(xTwiDeviceAddr xDeviceAddr) {

  return iDs1621ReadRawTemp(xDeviceAddr, DS1621_RWTL);
}

// -----------------------------------------------------------------------------
INLINE uint8_t
ucDs1621Counter(xTwiDeviceAddr xDeviceAddr) {

  return ucDs1621ReadByte(xDeviceAddr, DS1621_RDCNT);
}

// -----------------------------------------------------------------------------
INLINE uint8_t
ucDs1621Slope(xTwiDeviceAddr xDeviceAddr) {

  return ucDs1621ReadByte(xDeviceAddr, DS1621_RDSLOP);
}

// -----------------------------------------------------------------------------
INLINE eTwiStatus
eDs1621LastError (void) {

  return eDs1621LastErrorValue;
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_DS1621_H_ defined */
