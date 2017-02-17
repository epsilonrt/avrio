/**
 * @file icounter.h
 * @brief Comptage des signaux d'interruption
 *
 * Copyright © 2015 epsilonRT. All rights reserved.
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
 */
#ifndef _AVRIO_COUNTER_H_
#define _AVRIO_COUNTER_H_
#include <avrio/task.h>
#include <avrio/mutex.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup counter_module Comptage de signaux
 *
 *  Ce module permet de compter des événements sur une
 *  durée donnée (fenêtre temporelle). Il permet d'en déterminer la fréquence.
 *  Il utilise le module Task pour générer la fenêtre de mesure.
 *  @{
 * @example input/counter/demo_counter.c
 * Affiche la fréquence d'un signal sur la liaison série.
 */

/* constants ================================================================ */
/**
 * @brief Mode de fonctionnement d'un compteur
 */
typedef enum eCounterMode {
  eCounterSingle      = 0, /**< Une mesure à la fois */
  eCounterFreeRunning = 1, /**< Mesure permanente */
} eCounterMode;

/* structures =============================================================== */
/**
 * @brief 
 */
struct xCounterOps {
  void (*init) (void);  /***< Initialisation du comptage */
  uint16_t (*read) (void); /**< Lecture nombre événements depuis dernier clear */
  void (*clear) (void); /**< Clear du nombre d'événements */
  void (*enable) (bool); /**< Validation comptage */
};

/**
 * @brief Compteur
 *
 * La structure est opaque pour l'utilisateur
 */
struct xCounter;

/* types ==================================================================== */
typedef struct xCounterOps xCounterOps;

/* internal public functions ================================================ */
/**
 * @brief Initialisation d'un compteur
 *
 * @param c pointeur sur le compteur
 * @param o opérations
 */
void vCounterInit (struct xCounter * c, xCounterOps * o);

/**
 * @brief Démarre la mesure
 *
 * @param c pointeur sur le compteur
 */
void vCounterStart (struct xCounter * c);

/**
 * @brief Modifie le mode de fonctionnement
 *
 * @param c pointeur sur le compteur
 * @param m mode de fonctionnement
 */
void vCounterSetMode (struct xCounter * c, eCounterMode m);

/**
 * @brief Modifie la durée de la fenêtre de mesure
 *
 * @param c pointeur sur le compteur
 * @param usWindowMs durée en ms de la fenêtre de mesure
 */
void vCounterSetWindow (struct xCounter * c, uint16_t usWindowMs);

/**
 * @brief Teste si la mesure est terminée
 *
 * @param c pointeur sur le compteur
 * @return true si la mesure est terminée
 */
bool bCounterIsComplete (struct xCounter * c);

/**
 * @brief Attends que la mesure se termine
 *
 * @param c pointeur sur le compteur
 */
void vCounterWaitForComplete (struct xCounter * c);

/**
 * @brief Dernière fréqunence
 *
 * @param c pointeur sur le compteur
 * @return fréquence en Hertz
 */
double dCounterFreq (struct xCounter * c);

/**
 * @brief Dernier comptage
 *
 * @param c pointeur sur le compteur
 * @return Nombre d'impulsion sur la fenêtre
 */
uint16_t usCounterCount (struct xCounter * c);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Lecture du mode de fonctionnement
 *
 * @param c pointeur sur le compteur
 * @return mode de fonctionnement
 */
inline eCounterMode eCounterGetMode (struct xCounter * c);

/**
 * @brief Lecture de la durée de la fenêtre de mesure
 *
 * @param c pointeur sur le compteur
 * @return  durée en ms de la fenêtre de mesure
 */
inline uint16_t usCounterGetWindow (struct xCounter * c);
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
typedef struct xCounter {
  xTaskHandle xTask;      /**< Tâche gérant la fenêtre de mesure */
  xMutex xReady;          /**< Mutex indiquant la fin de la mesure */
  eCounterMode eMode;     /**< Mode de fonctionnement */
  uint16_t usLastValue;   /**< Dernier décompte d'impulsions */
  uint16_t usWindow;      /**< Largeur de la fenêtre en ms */
  xCounterOps * xOps;       /**< Opérations de comptage */
} xCounter;

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE eCounterMode 
eCounterGetMode (struct xCounter * c) {
  return c->eMode;
}

// -----------------------------------------------------------------------------
INLINE uint16_t 
usCounterGetWindow (struct xCounter * c) {
  return c->usWindow;
}

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_COUNTER_H_ */
