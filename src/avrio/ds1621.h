/**
 * @file ds1621.h
 * @brief Thermostat I2c DS1621 de DALLAS/MAXIM
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120303 - Initial version by epsilonRT
 */
#ifndef _AVRIO_DS1621_H_
#define _AVRIO_DS1621_H_

#include <avrio/twi.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup twi_devices_group
 * @{
 *
 *  @defgroup ds1621_module Thermostat IĠc DS1621
 *
 *  Ce module permet la commande d'un circuit thermostat DS1621 de MAXIM/DALLAS.
 *  @{
 */
/* macros =================================================================== */
/* constants ================================================================ */
#define DS1621_BASE	0x90 /**< Adresse I2c de base des circuits DS1621 */
#define DS1621_TEMP_ERROR -32767

typedef enum {
  DONE	  = _BV(7), /**< Indique que la conversion est terminée */
  THF		  = _BV(6), /**< Indique que la température de seuil haute est dépassée */
  TLF		  = _BV(5), /**< Indique que la température de seuil basse est dépassée */
  NVB		  = _BV(4), /**< Indique qu'une opération d'écriture est en cours dans la nvram */
  POL		  = _BV(1), /**< Permet de sélectionner la polarité de la sortie TOUT (1 active état haut)*/
  ONESHOT	= _BV(0), /**< Permet de sélectionner le mode One Shot (1) ou continu (0) */
  
  TAF     = THF | TLF, /**< Tous les drapeaux */
  DS1621_DEFAULT_CONFIG = 0	/**< Configuration par défaut */
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
 * @warning Le contrôleur de bus IĠC doit être initialisé avant.
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
 * @brief Lecture du dernier résultat de conversion
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return la température en dixièmes de degrés Celcius ou DS1621_TEMP_ERROR 
 * en cas d'erreur.
 */
inline int16_t iDs1621GetTemp (xTwiDeviceAddr xDeviceAddr);

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
inline int16_t iDs1621GetTh (xTwiDeviceAddr xDeviceAddr);

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
inline int16_t iDs1621GetTl (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lecture du registre d'état et de configuration (AC).
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Valeur du registre.
 */
inline uint8_t ucDs1621GetStatus (xTwiDeviceAddr xDeviceAddr);

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
inline uint8_t ucDs1621GetCounter (xTwiDeviceAddr xDeviceAddr);

/**
 * @brief Lire le registre de pente
 * Fonction utile si une grande précision de mesure est nécessaire. cf la page 4
 * du datasheet du DS1621 (version 090905).
 * @param xDeviceAddr adresse du DS1621 esclave
 * @return Valeur du registre de pente
 */
inline uint8_t ucDs1621GetSlope (xTwiDeviceAddr xDeviceAddr);

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
#define DS1621_RDTEMP	0xAA
#define DS1621_RWTH		0xA1
#define DS1621_RWTL		0xA2
#define DS1621_RWCONF	0xAC
#define DS1621_RDCNT	0xA8
#define DS1621_RDSLOP	0xA9
#define DS1621_STARTC	0xEE
#define DS1621_STOPC	0x22

/* internal public functions ================================================ */
/* Fonctions de haut niveau */
void vDs1621Init (xTwiDeviceAddr xDeviceAddr, uint8_t ucConfig);
void vDs1621SetTh (xTwiDeviceAddr xDeviceAddr, int16_t iTh);
void vDs1621SetTl (xTwiDeviceAddr xDeviceAddr, int16_t iTl);
void vDs1621ClrFlags (xTwiDeviceAddr xDeviceAddr, uint8_t ucFlags);

/* Fonctions de bas niveau */
int16_t iDs1621ReadTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);
void vDs1621WriteTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd, int16_t iTemp);
uint8_t ucDs1621ReadByte(xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);
void vDs1621SendCmd (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd);

/* public variables ========================================================= */
extern eTwiStatus eDs1621LastErrorValue;

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucDs1621GetStatus(xTwiDeviceAddr xDeviceAddr)) {

  return ucDs1621ReadByte(xDeviceAddr, DS1621_RWCONF);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (bool 
xDs1621IsDone (xTwiDeviceAddr xDeviceAddr)) {

  return ((ucDs1621GetStatus (xDeviceAddr) & DONE) != 0);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (bool 
xDs1621MemIsBusy (xTwiDeviceAddr xDeviceAddr)) {

  return ((ucDs1621GetStatus (xDeviceAddr) & NVB) != 0);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vDs1621Start(xTwiDeviceAddr xDeviceAddr)) {

  vDs1621SendCmd (xDeviceAddr, DS1621_STARTC);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vDs1621Stop(xTwiDeviceAddr xDeviceAddr)) {

  vDs1621SendCmd (xDeviceAddr, DS1621_STOPC);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (int16_t 
iDs1621GetTemp(xTwiDeviceAddr xDeviceAddr)) {

  return iDs1621ReadTemp(xDeviceAddr, DS1621_RDTEMP);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (int16_t 
iDs1621GetTh(xTwiDeviceAddr xDeviceAddr)) {

  return iDs1621ReadTemp(xDeviceAddr, DS1621_RWTH);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (int16_t 
iDs1621GetTl(xTwiDeviceAddr xDeviceAddr)) {

  return iDs1621ReadTemp(xDeviceAddr, DS1621_RWTL);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucDs1621GetCounter(xTwiDeviceAddr xDeviceAddr)) {

  return ucDs1621ReadByte(xDeviceAddr, DS1621_RDCNT);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucDs1621GetSlope(xTwiDeviceAddr xDeviceAddr)) {

  return ucDs1621ReadByte(xDeviceAddr, DS1621_RDSLOP);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (eTwiStatus 
eDs1621LastError (void)) {

  return eDs1621LastErrorValue;
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_DS1621_H_ defined */
