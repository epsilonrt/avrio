/**
 * @file rtc.h
 * @brief Horloge Temps réel
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120402 - Initial version by epsilonRT
 */
#ifndef _AVRIO_RTC_H_
#define _AVRIO_RTC_H_

#include <avrio/defs.h>
#include <avrio/mutex.h>
#include <stdio.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup rtc_module Horloge "Temps réel"
 *  Ce module offre une interface standardisée permettant de gérer les horloges
 *  temps réel matérielle ou logicielle. Une horloge temps réel entretient
 *  une heure et une date de façon permanente et peut suivant l'implémentation
 *  permettre de programmer des alarmes. \n
 *  @{
 */

/* constants ================================================================ */
/**  
 *  @enum eRtcWeekDay
 *  Jours de la semaine 
 *  Ces constantes permettent d'assigner un jour de la semaine
 */
 
typedef enum {

  MONDAY     =  2, /**< Lundi */
  TUESDAY    =  3, /**< Mardi */
  WEDNESDAY  = 4, /**< Mercredi */
  THURSDAY  = 5, /**< Jeudi */
  FRIDAY    = 6, /**< Vendredi */
  SATURDAY  = 7, /**< Samedi */
  SUNDAY     = 1  /**< Dimanche */
} eRtcWeekDay;

/* structures =============================================================== */

/**
 *  @brief Structure Temps
 *  Permet le stockage d'une date et d'une heure
 */
typedef struct xRtcTime {

  uint8_t ucSecond;   /**< secondes 0-59 */
  uint8_t ucMinute;   /**< minutes 0-59 */
  uint8_t ucHour;     /**< heure 0-23 ou 1-12 */
  uint8_t ucWeekDay;  /**<  Jour de la semaine \ref rtcWeekDay */
  uint8_t ucDate;     /**<  Jour du mois 1-31 */
  uint8_t ucMonth;    /**<  Mois 1-12 */
  uint16_t usYear;    /**<  Année en 4 chiffres */
} xRtcTime;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module RTC
 * @return 0 succès. -1 erreur 
 */
int iRtcInit (int iArg1);

/**
 * @brief Mets à l'heure de l'horloge RTC
 * @param time pointeur sur la nouvelle date et heure
 * @return 0 succès. -1 erreur 
 */
int iRtcSetTime (const xRtcTime * pxTime);

/**
 * @brief Lecture de l'horloge RTC
 * @param time pointeur sur la date et l'heure actuelle
 * @return 0 succès. -1 erreur 
 */
int iRtcGetTime (xRtcTime * pxTime);

/**
 * @brief Renvoie un pointeur sur le mutex indiquant la cadence des secondes
 * @return Le mutex renvoyé passe à 1 à chaque seconde et permet de synchroniser
 *  un processus en utilisant les fonctions \c xMutexLock() ou \c xMutexTryLock()
 */
xMutex * pxRtcHeartBeatsMutex (void);

/**
 * @brief Indique si une année est bisextile.
 * @param usYear l'année à tester
 * @return true si bisextile.
 */
bool xRtcYearIsLeap (int usYear);

/**
 * @brief Arme une alarme
 * @param index index le l'alarme à armer
 * @param time pointeur sur la date et l'heure de l'alarme
 * @return 0 succès. -1 erreur 
 */
int iRtcSetAlm (int8_t index, const xRtcTime * pxTime);

/**
 * @brief Lecture d'une alarme
 * @param index index le l'alarme à lire
 * @param time pointeur sur la date et l'heure de l'alarme
 * @return 0 succès. -1 erreur 
 */
int iRtcGetAlm (int8_t index, xRtcTime * pxTime);

/**
 * @brief Renvoie un pointeur sur le mutex de déclenchement d'une alarme
 * @param index index le l'alarme
 * @return Le mutex renvoyé passe à 1 lors du déclenchement de l'alarme permettant
 *  à un processus en utilisant les fonctions \c xMutexLock() ou \c xMutexTryLock()
 */
xMutex * pxRtcAlmMutex (int8_t index);

/**
 * @brief Renvoie la chaîne de caractères associée à un jour de la semaine
 * @param wd jour de la semaine \ref rtcWeekDay
 * @return la chaîne de caractères associée au jour de la semaine
 */
const char *pcRtcWeekdayToStr (uint8_t wd);

/**
 * @brief Renvoie la chaîne de caractères associée à une date
 * @param dest pointeur vers le buffer qui contiendra le résultat DD/MM/YY
 * @return le nombre de caractères écrits dans \c dest, sans compter le 
 *  caractère nul `\0' final.
 */
int iRtcDateToStr (char * pcStr, const xRtcTime * pxDate);

/**
 * @brief Renvoie la chaîne de caractères associée à une heure
 * @param dest pointeur vers le buffer qui contiendra le résultat HH:MM:SS
 * @return le nombre de caractères écrits dans \c dest, sans compter le 
 *  caractère nul `\0' final.
 */
int iRtcTimeToStr (char * pcStr, const xRtcTime * pxTime);

/**
 * @brief Ecrit la chaîne de caractères associée à une date sur un flux
 * @param dest pointeur vers le buffer qui contiendra le résultat DD/MM/YY
 * @return le nombre de caractères écrits dans \c dest, sans compter le 
 *  caractère nul `\0' final.
 */
int iRtcPrintDateToStream (FILE * pxStream, const xRtcTime * pxDate);

/**
 * @brief Ecrit la chaîne de caractères associée à une heure sur un flux
 * @param dest pointeur vers le buffer qui contiendra le résultat HH:MM:SS
 * @return le nombre de caractères écrits dans \c dest, sans compter le 
 *  caractère nul `\0' final.
 */
int iRtcPrintTimeToStream (FILE * pxStream, const xRtcTime * pxTime);

/* macros =================================================================== */
/**
 * @brief Ecrit la chaîne de caractères associée à une date sur stdout
 * @param dest pointeur vers le buffer qui contiendra le résultat DD/MM/YY
 * @return le nombre de caractères écrits dans \c dest, sans compter le 
 *  caractère nul `\0' final.
 */
#define iRtcPrintDate(d) iRtcPrintDateToStream(stdout, d)

/**
 * @brief Ecrit la chaîne de caractères associée à une heure sur stdout
 * @param dest pointeur vers le buffer qui contiendra le résultat HH:MM:SS
 * @return le nombre de caractères écrits dans \c dest, sans compter le 
 *  caractère nul `\0' final.
 */
#define iRtcPrintTime(t) iRtcPrintTimeToStream(stdout, t)

// -----------------------------------------------------------------------------
#  if defined(__DOXYGEN__)
  /* 
   * __DOXYGEN__ defined
   * Partie documentation ne devant pas être compilée.
   * =========================================================================
   */
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
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_RTC_H_ not defined */
