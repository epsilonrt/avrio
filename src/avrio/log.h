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
 * @file log.h
 * @brief Log (compatibilité sysio)
 */
#ifndef _AVRIO_LOG_H_
#define _AVRIO_LOG_H_
#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup log_module Log
 *  @{
 */

/* constants ================================================================ */
#define LOG_EMERG   3 /**< system is unusable */
#define LOG_ALERT   3 /**< action must be taken immediately */
#define LOG_CRIT    3 /**< critical conditions */
#define LOG_ERR     3 /**< error conditions */
#define LOG_WARNING 4 /**< warning conditions */
#define LOG_NOTICE  5 /**< normal but significant condition */
#define LOG_INFO    6 /**< informational */
#define LOG_DEBUG   7 /**< debug-level messages */

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/* macros =================================================================== */
/**
 * @brief Masque de validation du bit correspondant à la priorité p
 */
#define LOG_MASK(p)

/**
 * @brief Masque de validation de toutes les priorités jusqu'à p incluses
 */
#define LOG_UPTO(p)

/**
 * @brief Affiche ou écrit un message de log niveau LOG_DEBUG
 * 
 * Cette macro est inactivée si la constante DEBUG n'est pas définie à la 
 * compilation ou si la constante NLOG est définie à la compilation.
 * 
 * @param fmt chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels. Il s'agit d'une chaîne statique qui sera
 * stockée en FLASH (pour utiliser un format déjà stocké en RAM utiliser vLog(),
 * pour utiliser un format déjà stocké en FLASH utiliser vLog_P())
 */
#define PDEBUG(fmt,...)

/**
 * @brief Affiche ou écrit un message de log niveau LOG_INFO
 * 
 * Cette macro est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @param fmt chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels. Il s'agit d'une chaîne statique qui sera
 * stockée en FLASH (pour utiliser un format déjà stocké en RAM utiliser vLog(),
 * pour utiliser un format déjà stocké en FLASH utiliser vLog_P())
 */
#define PINFO(fmt,...)

/**
 * @brief Affiche ou écrit un message de log niveau LOG_NOTICE
 * 
 * Cette macro est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @param fmt chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels. Il s'agit d'une chaîne statique qui sera
 * stockée en FLASH (pour utiliser un format déjà stocké en RAM utiliser vLog(),
 * pour utiliser un format déjà stocké en FLASH utiliser vLog_P())
 */
#define PNOTICE(fmt,...)

/**
 * @brief Affiche ou écrit un message de log niveau LOG_WARNING
 * 
 * Cette macro est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @param fmt chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels. Il s'agit d'une chaîne statique qui sera
 * stockée en FLASH (pour utiliser un format déjà stocké en RAM utiliser vLog(),
 * pour utiliser un format déjà stocké en FLASH utiliser vLog_P())
 */
#define PWARNING(fmt,...)

/**
 * @brief Affiche ou écrit un message de log niveau LOG_ERROR
 * 
 * Cette macro est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @param fmt chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels. Il s'agit d'une chaîne statique qui sera
 * stockée en FLASH (pour utiliser un format déjà stocké en RAM utiliser vLog(),
 * pour utiliser un format déjà stocké en FLASH utiliser vLog_P())
 */
#define PERROR(fmt,...)

/* internal public functions ================================================ */
/**
 * @brief Affichage ou écriture d'un message de log
 *
 * Le message est écrit dans le fichier de log scpécifié par pxLogFile(), par
 * défaut stderr.
 *
 * Cette fonction est inactivée si la constante NLOG est définie à la compilation.
 *
 * @param priority priorité du message
 * @param format chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels.
 */
void vLog (int priority, const char *format, ...);

/**
 * @brief Affichage ou écriture d'un message de log, version FLASH
 *
 * Le message est écrit dans le fichier de log scpécifié par pxLogFile(), par
 * défaut stderr.
 *
 * Cette fonction est inactivée si la constante NLOG est définie à la compilation.
 *
 * @param priority priorité
 * @param format chaîne de format conforme à la syntaxe de la fonction printf()
 * suivie des paramètres optionnels. La chaîne réside en FLASH
 */
void vLog_P (int priority, const char *format, ...);

/**
 * @brief Chaîne de caractère correspondant à la priorité
 * 
 * @param priority priorité
 * @return chaîne constante en RAM
 */
const char * sLogPriorityString (int priority);

/**
 * @brief Fixe le masque des priorités
 * 
 * Les  huit  priorités sont LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR,
 * LOG_WARNING, LOG_NOTICE, LOG_INFO et LOG_DEBUG. \n
 *
 * Cette fonction est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @param mask le bit correspondant à la priorité p est LOG_MASK(p), LOG_UPTO(p)
 * pour le masque de toutes les priorités jusqu'à p incluses.
 */
static inline void vLogSetMask (int mask);

/**
 * @brief Lecture du masque des priorités en cours
 *
 * Cette fonction est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @return la valeur du masque
 */
static inline int iLogMask (void);

/**
 * @brief Modifie le fichier de log
 *
 * Cette fonction est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @param f pointeur sur le fichier
 */
static inline void vLogSetFile (FILE * f);

/**
 * @brief Fichier de log courant
 *
 * Cette fonction est inactivée si la constante NLOG est définie à la compilation.
 * 
 * @return pointeur sur le fichier, stderr par défaut
 */
static inline FILE * pxLogFile (void);

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

/* macros =================================================================== */

/* Macros de compatibilité SysIO ne faisant rien */
#define __progname ""
#define vLogInit (m)
#define vLogDaemonize (d)
#define bLogIsDaemonized() (0)
#define LOG_MASK(p) (1<<((p)&0x07))
#define LOG_UPTO(p) ((LOG_MASK(p) << 1) - 1)

#ifndef NLOG
#include <stdio.h>
#include <avr/pgmspace.h>

/* structures =============================================================== */
typedef struct xLog xLog;
struct xLog {

  FILE * stream;
  int mask;
};

/* public variables ========================================================= */
extern xLog xAvrioLog;

/* macros =================================================================== */

#define PERROR(fmt,...) vLog_P (LOG_ERR,PSTR("%s():%d: "fmt),\
                                __FUNCTION__, __LINE__,##__VA_ARGS__)

#ifdef DEBUG
#define PDEBUG(fmt,...) vLog_P (LOG_DEBUG,PSTR("%s():%d: "fmt),\
                                __FUNCTION__, __LINE__,##__VA_ARGS__)
#define PINFO(fmt,...) vLog_P (LOG_INFO,PSTR("%s():%d: "fmt),\
                               __FUNCTION__, __LINE__,##__VA_ARGS__)
#define PNOTICE(fmt,...) vLog_P (LOG_NOTICE,PSTR("%s():%d: "fmt),\
                                 __FUNCTION__, __LINE__,##__VA_ARGS__)
#define PWARNING(fmt,...) vLog_P (LOG_WARNING,PSTR("%s():%d: "fmt),\
                                  __FUNCTION__, __LINE__,##__VA_ARGS__)
#else /* DEBUG not defined */
#define PDEBUG(fmt,...)
#define PINFO(fmt,...) vLog_P (LOG_INFO,PSTR(fmt),##__VA_ARGS__)
#define PNOTICE(fmt,...) vLog_P (LOG_NOTICE,PSTR(fmt),##__VA_ARGS__)
#define PWARNING(fmt,...) vLog_P (LOG_WARNING,PSTR(fmt),##__VA_ARGS__)
#endif /* DEBUG not defined */

/* internal public functions ================================================ */
void vLog (int priority, const char *format, ...);
void vLog_P (int priority, const char *format, ...);
const char * sLogPriorityString (int priority);

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE void
vLogSetMask (int mask) {

  xAvrioLog.mask = mask;
}

// -----------------------------------------------------------------------------
INLINE int
iLogMask (void) {

  return xAvrioLog.mask;
}

// -----------------------------------------------------------------------------
INLINE void
vLogSetFile (FILE * f) {

  xAvrioLog.stream = f;
}

// -----------------------------------------------------------------------------
INLINE FILE *
pxLogFile (void) {

  return (xAvrioLog.stream) ? xAvrioLog.stream : stderr;
}

#else /* NLOG defined */

/* macros =================================================================== */
#define PWARNING(fmt,...)
#define PERROR(fmt,...)
#define PINFO(fmt,...)
#define PNOTICE(fmt,...)
#define PDEBUG(fmt,...)

/* internal public functions ================================================ */
#define vLog(p,fmt,...)
#define vLog_P(p,fmt,...)
#define sLogPriorityString(p) "Unsupported"

/* inline public functions ================================================== */
#define vLogSetMask(m)
#define iLogMask() (-1)
#define vLogSetFile(f)
#define pxLogFile() (NULL)

#endif /* NLOG defined */

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_LOG_H_ */
