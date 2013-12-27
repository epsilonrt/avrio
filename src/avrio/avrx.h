/**
 * @file avrx.h
 * @brief Noyau temps réel préemptif AvrX
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * 
 * Revision History ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#ifndef _AVRX_H_
#  define _AVRX_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
  /**
   * @addtogroup avrx_module
   *  @{
   */

#  if defined(__DOXYGEN__)
  /* macros ================================================================= */
  /**
   * @ingroup avrx_task_module
   * @brief Déclare et initialise un \ref xTcb  
   * 
   * @param task_name Nom de la tâche
   * @param stack_size Nombre d'octets réservés à la pile de la tâche
   * @param priority Priorité de la tâche (1 est plus prioritaire que 2)
   */
#  define AVRX_DECLARE_TASK(task_name, stack_size, priority)

  /**
   * @ingroup avrx_task_module
   * @brief Fonction exécutée par une tâche
   *
   * Définie le corps d'une fonction exécutée par une tâche . \n
   * Une fonction de ce type de prend aucun paramètre et ne retourne jamais.
   * @param task_name Nom de la tâche
   */
#  define AVRX_FUNC(task_name) 

  /**
   * @ingroup avrx_task_module
   * @brief Déclare et initialise un \ref xTcb et définie le corps 
   *  de la fonction exécutée par la tâche   
   * 
   * Equivalent à AVRX_DECLARE_TASK() suivi de AVRX_FUNC()
   * @param task_name Nom de la tâche
   * @param stack_size Nombre d'octets réservés à la pile de la tâche
   * @param priority Priorité de la tâche (1 est plus prioritaire que 2)
   */
#  define AVRX_TASK(task_name, stack_size, priority)

  /**
   * @ingroup avrx_task_module
   * @brief Déclare un \ref xTcb et une fonction de tâche externe   
   * 
   * @param task_name Nom de la tâche
   */
#  define AVRX_EXTERN_TASK(task_name)

  /**
   * @ingroup avrx_task_module
   * @brief Fonction exécutée par une tâche
   *
   * Définie le corps d'une fonction exécutée par une tâche . \n
   * Une fonction de ce type de prend aucun paramètre et ne retourne jamais. \n
   * NACKED indique que cette fonction ...
   * @param task_name Nom de la tâche
   */
#  define AVRX_NAKED_FUNC(task_name) 

  /**
   * @ingroup avrx_task_module
   * @brief Renvoie le pointeur sur l'identifiant d'une tâche 
   * 
   * @param task_name Nom de la tâche
   */
#  define PID(task_name)

  /**
   * @ingroup avrx_task_module
   * @brief Renvoie le pointeur sur le \ref xTcb d'une tâche
   * 
   * @param task_name Nom de la tâche
   */
#  define TCB(task_name)

  /**
   * @ingroup avrx_task_module
   * @brief Initialise une tâche
   *
   * Initialise une tâche en initialisant et en sauvegardant son contexte sur
   * la pile. Le contexte est constitué de tous les registres de calcul
   * du processeur. Tous ces registres sont initialisés à zéro.
   * @param task_name Nom de la tâche
   * @return le pid ?
   */  
#  define xAvrXInitTask (task_name);

  /**
   * @ingroup avrx_task_module
   * @brief Démarre une tâche
   *
   * Démarre une tâche en appelant d'abord xAvrXInitTask() puis vAvrXResume(). \n
   * vAvrXRunTask() ou vAvrXResume() doivent être appelé par le main() afin 
   * d'insérer la tâche dans la file d'exécution.
   * @param task_name Nom de la tâche
   */
#  define vAvrXRunTask (task_name);

  /* structures ============================================================= */
  /**
   * @ingroup avrx_task_module
   * @brief Un processus
   *
   * Bloc de RAM qui contient l'état de la tâche.
   */
  typedef struct xProcess {} xProcess;

  /**
   * @ingroup avrx_task_module
   * @brief Bloc de contrôle de tâche
   *
   * Le Bloc de contrôle de tâche (Task Control Block) contient toutes les 
   * informations nécessaires à l'initialisation et au démarrage d'une tâche. \n
   * Ce bloc est stocké en FLASH. Il est utilisé uniquement par xAvrXInitTcb()
   * ou vAvrXRunTcb().
   */
  typedef struct xTcb {} xTcb;

  /**
   * @ingroup avrx_message_module
   * @brief Message
   *
   * Une file de messages est en réalité une extension d'une file de sémaphores.
   * Un message peut être utilisé pour gérer des exlusions mutuelles (le message 
   * est un témoin) ou pour passer des informations d'ordre général. \n
   * Cela peut être nécessaire à des tâches qui doivent bloquer en attente de 
   * plusieurs choses à la fois (timer, interruption, etc.).
   */
  typedef struct xMessage {} xMessage;
 
  /**
   * @ingroup avrx_message_module
   * @brief File de messages
   *
   * Permet la communication par message entre tâches.
   */
  typedef struct xMessageQueue {} xMessageQueue;

  /**
   * @ingroup avrx_timer_module
   * @brief Timer
   *
   * Les timers sont des variables compteurs dont la valeur est décomptée par
   * un service en mode noyau (afin de minimiser les retards) à chaque 
   * itération (tick) du noyau.\n
   * Le service gestionnaire est chargé, outre le décomptage, d'insérer et de
   * retirer les timers de la file système.
   */
  typedef struct xTimer {} xTimer;

  /**
   * @ingroup avrx_timer_module
   * @brief Timer Message
   *
   * Une version spéciale de timer qui envoie des messages plutôt que de 
   * modifier un sémaphore.
   */
  typedef struct xTimerMessage {} xTimerMessage;

  /* types ================================================================ */

  /**
   * @ingroup avrx_task_module
   * @brief Identifiant d'une tâche
   *
   * Un identifiant de tâche (pid) est un pointeur sur le bloc de RAM qui 
   * contient l'état de cette tâche.
   */
  typedef xProcess * xPid;

  /**
   * @ingroup avrx_semaphore_module
   * @brief Compteurs permettant de gérer l'accès à des ressources partagées
   *
   * Les sémaphores sont des compteurs permettant de gérer l'accès à des
   * ressources partagées. \n
   * Dans AvrX :
   * - Les sémaphores xSem sont implémentés sous la forme d'une liste 
   *   chaînée simple des tâches en attente.
   * - Un sémaphore peut avoir une des valeurs ci-dessous :
   *  - AVRX_PEND Le sémaphore est occupé en attente d'un événement
   *  - AVRX_DONE Le sémaphore a été libéré
   *  - AVRX_WAIT ??
   *  - Toute autre valeur est l'adresse d'un \ref xProcess
   * .
   * .
   */
  typedef xPid xSem;


#  else   /* __DOXYGEN__ not defined */ 
  //----------------------------------------------------------------------------
  typedef struct xProcess {
  
    struct xProcess * next;
    uint8_t flags;
    uint8_t priority;
    void * ContextPointer;
#   ifdef AVRX_SINGLESTEP_ENABLE
      uint8_t * bp1;
      uint8_t * bp2;
#   endif
  } xProcess;

  //----------------------------------------------------------------------------
  typedef xProcess * xPid;

  //----------------------------------------------------------------------------
  typedef xPid xSem;

  //----------------------------------------------------------------------------
  typedef struct {
  
    void * r_stack;       // Start of stack (top address-1)
    void (*start) (void); // Entry point of code
    xPid pid;             // Pointer to Process block
    uint8_t priority;     // Priority of task (0-255)
  } FLASH const xTcb;

  //----------------------------------------------------------------------------
  typedef struct xMessage {
  
    struct xMessage *next;
    xSem semaphore;
  } xMessage;

  //----------------------------------------------------------------------------
  typedef struct xMessageQueue {
    xMessage * message; // List of messages
    xPid pid;           // List of processes
  } xMessageQueue;

   
  //----------------------------------------------------------------------------
  typedef struct xTimer {
    struct xTimer *next;
    xSem semaphore;
    uint16_t count;
  } xTimer;

  //----------------------------------------------------------------------------
  typedef struct xTimerMessage {
    union {
      struct xMessage mcb;
      struct xTimer tcb;
    } u;
    struct xMessageQueue *queue;
  } xTimerMessage;

#  endif  /* __DOXYGEN__ not defined */

  /* internal public functions ============================================== */
  /**
   *    @defgroup avrx_task_module Gestion des tâches
   *    Ce module regroupe les API de gestion de tâches.
   *    @{
   */
   
  /**
   * Démarre le noyau AvrX et ne retourne jamais
   *
   * Il est nécessaire d'insérer au moins une tâche dans la file d'exécution
   * avec vAvrXRunTask() ou vAvrXResume() avant d'appeler cette fonction.
   */
  void vAvrXStart (void);
  
  /**
   * @brief Reprend l'exécution d'un processus 
   *
   * Obtient une tâche prête à s'exécuter en insérant son PID dans la file 
   * d'exécution 
   * @param xP Pointeur sur la tâche
   */
  void vAvrXResume (xPid xP);

  /**
   * @brief Suspend l'exécution d'une tâche
   *
   * Marque un PID pour la suspension en attente de retrait de la file 
   * d'exécution.
   * @param xP Pointeur sur la tâche
   */
  void vAvrXSuspend (xPid xP);

  /**
   * @brief Retire le processeur à la tâche en cours  
   *
   * Retire le processeur à la tâche en cours et la réinsère dans la file des
   * tâches en exécution. Si d'autres tâches de même priorité sont dans la file,
   * la tâche qui abandonne le processeur est insérée en fin de file 
   * (algorithme Round Robin).
   */
   void vAvrXYield(void);

 /**
   * @brief Tue la tâche en cours
   *
   * Doit être appelée quand une tâche est terminée et ne fonctionnera plus. 
   * Cette fonction agit comme le «return» dans une fonction normale. \n
   * Idéalement, la tâche doit être inactive (pas en attente d'un sémaphore ou
   * d'un message) avant que cette fonction soit appelée.
   */
  void vAvrXTaskExit (void);
  
  /**
   * @brief Tue une tâche
   *
   * Identique à vAvrXTaskExit() si ce n'est que cette fonction permet de tuer
   * une autre tâche.
   * @param xP Pointeur sur la tâche
   */
  void vAvrXTerminate (xPid xP);

  /**
   * @brief Synchronisation bloquante avec une autre tâche
   *
   * Permet d'attendre qu'une tâche passe à l'état d'exécution.
   * @param xP Pointeur sur la tâche
   */
  void vAvrXWaitTask (xPid xP);

  /**
   * @brief Renvoie l'état d'une tâche
   * 
   * @param xP Pointeur sur la tâche
   * @return AVRX_PEND, AVRX_DONE or AVRX_WAIT
   */  
  xSem xAvrXTestPid (xPid xP);

  /**
   * @brief Renvoie la priorité courante d'une tâche
   * 
   * @param xP Pointeur sur la tâche
   * @return La priorité courante (1 est plus prioritaire que 2)
   */  
  uint8_t xAvrXPriority (xPid xP);

  /**
   * @brief Modifie la priorité d'une tâche  
   * 
   * @param xP Pointeur sur la tâche
   * @param xNewPriority Nouvelle valeur de priorité (1 est plus prioritaire que 2)
   * @return La priorité avant modification (1 est plus prioritaire que 2)
   */  
  uint8_t xAvrXChangePriority (xPid xP, uint8_t xNewPriority);

  /**
   * @brief Renvoie le PID de la tâche en cours
   * 
   * @return Le PID de la tâche en cours d'exécution
   */  
  xPid pxAvrXSelf (void);

  /**
   * @brief Modifie la pile AvrX 
   * @param pNewStack Pointeur sur la fin de la nouvelle pile, si 0 la pile 
   *  AvrX est initialisée à la valeur actuelle du pointeur de pile (SPH/SPL).
   * @return Pointeur sur la fin de la nouvelle pile
   */
  void * pvAvrXSetKernelStack (void * pNewStack);

  /**
   * @brief Permet une organisation collaborative au sein de l'organisation
   *  préemptive par priorité.
   *
   * Réordonne la file d'exécution en passant la tâche de début en fin de file.
   * @warning Ne peut être appelée qu'à partir du mode noyau AvrX (par exemple 
   * l'intérieur d'un gestionnaire d'interruption).
   */
  void vAvrXIntReschedule(void);

  /**
   * @brief Initialise une tâche
   *
   * Initialise une tâche en initialisant et en sauvegardant son contexte sur
   * la pile. Le contexte est constitué de tous les registres de calcul
   * du processeur. Tous ces registres sont initialisés à zéro.
   * @param pxTcb Pointeur sur le bloc de contrôle de la tâche
   * @return Identifiant du processus initialisé
   */  
  xPid xAvrXInitTcb (xTcb * pxTcb);

  /**
   * @brief Démarre une tâche
   *
   * Démarre une tâche en appelant d'abord xAvrXInitTcb() puis vAvrXResume(). \n
   * vAvrXRunTcb() ou vAvrXResume() doivent être appelé par le main() afin 
   * d'insérer la tâche dans la file d'exécution.
   * @param pxTcb Pointeur sur le bloc de contrôle de la tâche
   */
  void vAvrXRunTcb (xTcb * pxTcb);

 /**
   *  @defgroup avrx_degug_module Déboguage
   *  Ce module regroupe les API de déboguage.
   *  @{
   */

  /**
   * @brief Arrête le processeur (erreur seulement)
   *
   * Dévalide les interruptions et arrête le processeur en exécutant une 
   * sans fin.
   */
  void vAvrXHalt (void);

  /**
   * @brief Passe une tâche en mode «pas à pas»
   *
   * Réactive une tâche, l'ajoute à la file d'exécution, puis la re-suspend.
   * @param xP Pointeur sur la tâche
   * @return 0 succès, 1 si la tâche n'a pu être suspendue.
   */  
  uint8_t xAvrXSingleStep (xPid xP);

  /**
   * @brief Avance d'un pas une tâche passée en mode «pas à pas»
   *
   * La tâche est emprisonnée en tête de la file d'exécution.
   * @param xP Pointeur sur la tâche
   * @return 0 succès, 1 si la tâche n'a pu être suspendue.
   */  
  uint8_t xAvrXSingleStepNext (xPid xP);
  
  /**
   *    @}
   *  @}
   */
  
  /**
   *  @defgroup avrx_semaphore_module Sémaphores
   *  Ce module regroupe les API permettant de gérer les sémaphores. Les 
   *  sémaphores sont des compteurs permettant de gérer l'accès à des
   *  ressources partagées et de synchroniser des tâches. \n
   *  Lorsqu'une tâche est en attente sur le sémaphore, le sémaphore est défini 
   *  sur l'ID de tâche. \n
   *  AVRX_PEND signifie que la ressource liée au sémaphore est occupée (Sem = 0)
   *  alors que AVRX_DONE signifie que la ressource est libre (Sem > 0).
   *  Une tâche bloquera seulement si elle tente de s'emparer d'un sémaphore à
   *  l'état AVRX_PEND. \n 
   *  Les sémaphores sont initialisées à l'état AVRX_PEND et sont laissés à 
   *  l'état AVRX_PEND après chaque opération complète. \n  
   *  Les processus peuvent attendre qu'un sémaphore soit libre.  
   *  Cela sera possible que si le sémaphore a été mis à AVRX_PEND avant le 
   *  premier appel. Quand un processus a terminé avec le sémaphore, il le 
   *  libère en le mettant à AVRX_PEND.
   *  @{
   */
    /* constants ============================================================ */
    /**
     * @brief Ressource Occupée ! revenir plus tard ...
     */
#   define AVRX_PEND ((xSem)0)
    
    /**
     * @brief Ressource Libre, terminé !
     */
#   define AVRX_DONE ((xSem)1)
    
    /**
     * @brief Une ou plusieurs tâches sont en attente de libération de la ressource
     */
#   define AVRX_WAIT ((xSem)2)
    
    /* internal public functions ============================================ */
    /**
     * @brief Libère un sémaphore si il n'est pas déjà libre
     * 
     * Si sémaphore est à l'état AVRX_PEND ou AVRX_DONE, passe à l'état 
     * AVRX_DONE. \n
     * Si une tâche est en attente, l'exécute et passe le sémaphore
     * à l'état AVRX_PEND.
     *
     * @param pxSem Pointeur sur le semaphore 
     */
    void vAvrXSetSemaphore (xSem * pxSem);

    /**
     * @brief Mets une tâche en attente de libération d'un sémaphore 
     *
     * S'il est utilisé pour à des fins de signalisation, une seule tâche doit 
     * attendre la libération du sémaphore.
     *
     * @param pxSem Pointeur sur le semaphore 
     */
    void vAvrXWaitSemaphore (xSem * pxSem);
  
     /**
     * @brief Renvoie l'état d'un sémaphore
     * 
     * @param pxSem Pointeur sur le semaphore 
     * @return AVRX_PEND, AVRX_DONE ou AVRX_WAIT
     */
    xSem xAvrXTestSemaphore (xSem * pxSem);
    
    /**
     * @brief Force un sémaphore à passer à l'état AVRX_PEND
     * 
     * @param pxSem Pointeur sur le semaphore 
     */
    void vAvrXResetSemaphore (xSem * pxSem);
    
    /**
     * @brief Force un sémaphore d'un objet à passer à l'état AVRX_PEND
     * 
     * @param pxSem Pointeur sur le semaphore 
     */
    void vAvrXResetObjectSemaphore (xSem * pxSem);

    /**
     * @brief Libère un sémaphore si il n'est pas déjà libre (Mode noyau)
     *
     * Identique à vAvrXSetSemaphore() pour une utilisation en mode noyau.
     * @param pxSem Pointeur sur le semaphore 
     * @warning Ne peut être appelée qu'à partir du mode noyau AvrX (par exemple 
     * l'intérieur d'un gestionnaire d'interruption).
     */
    void vAvrXIntSetSemaphore (xSem * pxSem);

    /**
     * @brief Renvoie l'état d'un sémaphore (Mode noyau)
     * 
     * Identique à xAvrXTestSemaphore() pour une utilisation en mode noyau.
     * @param pxSem Pointeur sur le semaphore 
     * @return AVRX_PEND, AVRX_DONE ou AVRX_WAIT
     * @warning Ne peut être appelée qu'à partir du mode noyau AvrX (par exemple 
     * l'intérieur d'un gestionnaire d'interruption).
     */
    xSem xAvrXIntTestSemaphore (xSem * pxSem);

  /**
   *  @}
   */

  /**
   *  @defgroup avrx_message_module Messages
   *  Ce module regroupe les API permettant de permettre aux tâches de 
   *  communiquer par l'intermédiaire de messages.
   *  @{
   */

    /* internal public functions ============================================ */

    /**
     * @brief Envoie un message dans une file
     *
     * @note Fonction non bloquante 
     * @param pxQueue File des messages à utiliser
     * @param pxMessage Message
     */
    void vAvrXSendMessage (xMessageQueue * pxQueue, xMessage * pxMessage);

    /**
     * @brief Envoie un message dans une file (Mode noyau)
     *
     * Identique à vAvrXSendMessage() pour une utilisation en mode noyau.
     * @param pxQueue File des messages à utiliser
     * @param pxMessage Message
     * @warning Ne peut être appelée qu'à partir du mode noyau AvrX (par exemple 
     * l'intérieur d'un gestionnaire d'interruption).
     */
    void vAvrXIntSendMessage (xMessageQueue * pxQueue, xMessage * pxMessage);

    /**
     * @brief Renvoie le premier message d'une file
     * 
     * @param pxQueue File des messages à utiliser
     * @return Pointeur sur le message ou 0 si la file est vide
     */
    xMessage * pxAvrXRecvMessage (xMessageQueue * pxQueue);

    /**
     * @brief Attente de réception d'un message
     *
     * Version bloquante pxAvrXRecvMessage.
     * @param pxQueue File des messages à utiliser
     * @return Pointeur sur le message reçu
     */
    xMessage * pxAvrXWaitMessage (xMessageQueue * pxQueue);

    /**
     * @brief Envoie un accusé réception à travers une file de message
     *
     * La séquence s'enchaîne de la façon suivante :
     * vAvrXSendMessage() -> pxAvrXWaitMessage() -> vAvrXAckMessage() ->
     * vAvrXWaitMessageAck().
     * @param pxMessage Message
     */
    void vAvrXAckMessage (xMessage * pxMessage);

    /**
     * @brief Attente d'un accusé réception d'une file de message
     * 
     * @param pxMessage Message
     */
    void vAvrXWaitMessageAck (xMessage * pxMessage);

    /**
     * @brief Teste si un accusé réception a été reçue
     *
     * Version non bloquante de vAvrXWaitMessageAck()
     * @param pxMessage Message
     * @return AVRX_DONE si reçu, AVRX_PEND sinon
     */
    xSem xAvrXTestMessageAck (xMessage * pxMessage);

  /**
   *  @}
   */
  
  /**
   *  @defgroup avrx_timer_module Timer
   *  Les timers sont des variables compteurs dont la valeur est décomptée par
   *  un service en mode noyau (afin de minimiser les retards) à chaque 
   *  itération (tick) du noyau.\n
   *  Le service gestionnaire est chargé, outre le décomptage, d'insérer et de
   *  retirer les timers de la file système.
   *  @todo Modifier le type de uint16_t à ticks_t dans les fonctions Timer
   *  @{
   */

    /* internal public functions ============================================ */
    /**
     * @brief Ajoute un timer dans la file des timers et le démarre.
     *
     * Cette fonction non-bloquante ajoute le timer dans la file des timers
     * du système et le démarre pour un certain nombre de ticks.
     * @param pxTimer Pointeur sur le timer
     * @param xTicksWait Nombre de ticks. Il est possible d'utiliser 
     *  xDelayMsToTicks() le nombre de ticks associé à une valeur en millisecondes.
     */
    void vAvrXStartTimer (xTimer * pxTimer, uint16_t xTicksWait);

    /**
     * @brief Retire un timer de la file des timers
     *
     * @param pxTimer Pointeur sur le timer
     * @return Pointeur sur le timer retiré ou 0 en cas d'erreur 
     */
    xTimer * pxAvrXCancelTimer (xTimer * pxTimer);

    /**
     * @brief Attente de l'expiration d'un timer
     *
     * Fonction bloquante qui attend l'expiration d'un timer.
     * @param pxTimer Pointeur sur le timer
     */
    void vAvrXWaitTimer (xTimer * pxTimer);

    /**
     * @brief Version bloquante de vAvrXStartTimer()
     *
     * C'est une version bloquante de vAvrXStartTimer() qui prend les mêmes
     * paramètres. Elle est implémentée par un appel à vAvrXStartTimer() suivi 
     * d'un appel à  AvrXWaitTimer().\n
     *  
     * @param pxTimer Pointeur sur le timer
     * @param xTicksWait Nombre de ticks. Il est possible d'utiliser 
     *  xDelayMsToTicks() le nombre de ticks associé à une valeur en millisecondes.
     */
    void vAvrXDelay (xTimer * pxTimer, uint16_t xTicksWait);

    /**
     * @brief Teste l'état d'un timer
     *
     * @param pxTimer Pointeur sur le timer
     * @return AVRX_WAIT, AVRX_PEND ou AVRX_DONE
     */
    xSem xAvrXTestTimer (xTimer * pxTimer);

    /**
     * @brief Ajoute un timer un timer qui envoie un message à son expiration 
     *  dans la file des timers et le démarre.
     *
     * Cela peut être utile quand vous souhaitez envoyer un message au bout d'un
     * certain temps. 
     * @param pxTimer Pointeur sur le timer
     * @param xTicksWait Nombre de ticks. Il est possible d'utiliser 
     *  xDelayMsToTicks() le nombre de ticks associé à une valeur en millisecondes.
     * @param pxQueue Pointeur sur la file de message dans laquelle le message
     *  sera envoyé.
     */
    void vAvrXStartTimerMessage ( xTimerMessage * pxTimer,
                                  uint16_t xTicksWait, xMessageQueue * pxQueue);

    /**
     * @brief Retire un timer démarré avec vAvrXStartTimerMessage() de la file 
     *  des timers
     *
     * @param pxTimer Pointeur sur le timer
     * @param pxQueue Pointeur sur la file de message dans laquelle le message
     *  sera envoyé.
     * @return Le pointeur sur le timer retiré ou 0 en cas d'erreur.
     */
    xMessage * pxAvrXCancelTimerMessage (xTimerMessage * pxTimer,
                                                     xMessageQueue * pxQueue);
  /**
   *  @}
   */

  /**
   *  @defgroup avrx_eeprom_module EEprom
   *  AvrX fournit quelques routines d'accès mémoire EEPROM qui contrôlent 
   *  l'accès au matériel via un sémaphore. Ce sémaphore doit être libre 
   *  pour valider l'accès à la mémoire.
   *  @{
   */
    /* internal public functions ============================================ */
    
    /**
     * @brief Lecture d'un octet en EEPROM
     * 
     * @param pucAddress Pointeur sur l'octet en EEPROM
     * @return Valeur de l'octet
     */
    uint8_t ucAvrXReadEEProm (const uint8_t * pucAddress);
    
    /**
     * @brief Lecture d'un mot en EEPROM
     * 
     * @param pusAddress Pointeur du mot en EEPROM
     * @return Valeur du mot
     */
    uint16_t usAvrXReadEEPromWord (const uint16_t * pusAddress);
    
    /**
     * @brief Ecriture d'un octet en EEPROM
     * 
     * @param Pointeur sur l'octet en EEPROM
     * @param Valeur de l'octet
     */
    void vAvrXWriteEEProm (uint8_t * pucAddress, uint8_t ucByte);
  /**
   *  @}
   */
 
/**
 *   @}
 * @}
 */
#  if !defined(__DOXYGEN__)
  /* 
   * __DOXYGEN__ not defined
   * Partie ne devant pas être documentée.
   * =========================================================================
   */
#  define MINCONTEXT 35  // 32 registers, return address and SREG

#  define AVRX_NAKED_FUNC(function) void function(void) NAKED;\
    void function(void)

#  define AVRX_FUNC(function) void function(void) CTASK;\
    void function(void)

#  define AVRX_DECLARE_TASK(task_name, stack_size, priority)	\
    char task_name ## Stk [stack_size + MINCONTEXT] ; \
    AVRX_FUNC(task_name); \
    xProcess task_name ## Pid; \
    xTcb task_name ## Tcb = \
    { \
        &task_name##Stk[sizeof(task_name##Stk)-1] , \
        task_name, \
        &task_name##Pid, \
        priority \
    }

#  define AVRX_TASK(task_name, stack_size, priority) \
    AVRX_DECLARE_TASK(task_name, stack_size, priority); \
    AVRX_FUNC(task_name)

#  define AVRX_EXTERN_TASK(task_name)	\
    AVRX_FUNC(task_name);				\
    extern xTcb task_name##Tcb; \
    extern xProcess task_name##Pid

#  define PID(task_name) (&task_name##Pid)

#  define TCB(task_name) (&task_name##Tcb)
  
#  define xAvrXInitTask(task_name) xAvrXInitTcb(TCB(task_name))

#  define vAvrXRunTask(task_name) vAvrXRunTcb(TCB(task_name))

#  endif /* __DOXYGEN__ not defined */

  /* ======================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRX_H_ not defined */
