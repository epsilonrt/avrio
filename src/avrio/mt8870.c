/**
 * @file mt8870.c
 * @brief Gestion d'une ligne téléphonique avec décodage DTMF
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

 */
#include "avrio-config.h"

#ifdef AVRIO_PHONE_ENABLE
/* ========================================================================== */
#  include "avrio-board-phone.h"
#  include <avr/eeprom.h>
#  include <avrio/task.h>
#  include <util/atomic.h>
#  include <avrio/queue.h>
#  include <avrio/mt8870.h>
// Pour le debug uniquement
#  include <avrio/led.h>

/* constants ================================================================ */
// Période max du signal de sonnerie en ms (20 ms typique)
#  define RING_PERIOD             22
// Durée max du blanc entre 2 sonneries en ms (3500 typique)
#  define RING_INTERLEAVE         4000
// Durée min d'une sonnerie en période du signal (50 = 1 s)
#  define RING_LENGTH_MIN         25
// Réglage max du nombre de sonnerie max avant de décrocher
#  define PHONE_HOOKOFF_DELAY_MAX  9
// Réglage min du nombre de sonnerie min avant de décrocher
#  define PHONE_HOOKOFF_DELAY_MIN  4

/* constants ================================================================ */
enum {
  IDLE_STATE = 0,
  RING_FLAG = 0x01,
  CALL_FLAG = 0x02,
  FASTHOOKOFF_FLAG = 0x04,
  HOOKOFF_REQUEST_FLAG = 0x08,
  HANGUP_REQUEST_FLAG = 0x10,
  HOOKOFF_FLAG = 0x80
};

/* private variables ======================================================== */
static volatile uint8_t ePhoneState = IDLE_STATE;
static xTaskHandle xPhoneTimer;
static xTaskHandle xFastHookoffTimer;
static uint8_t ucRingCount;
static uint16_t usRingLength;

// File des messages envoyés par ce module
QUEUE_STATIC_DECLARE (xPhoneQueue, PHONE_BUFSIZE);

// nombre de sonneries avant décrochage (copie de ucHookOffDelayEemem en RAM)
static uint8_t ucHookOffDelay;

/* 
 * EEPROM
 *********************************/
static uint8_t ucHookOffDelayEemem EEMEM = PHONE_HOOKOFF_DELAY;

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static uint8_t
prvucAtomicReadByte (volatile uint8_t * pucByte) {
  uint8_t ucValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    ucValue = *pucByte;
  }
  return ucValue;
}

// ------------------------------------------------------------------------------
// Arrête et réinitialise la tâche prvvClearFastHookoff
static void
prvvClearFastHookoff (void) {

  vTaskSetInterval (xFastHookoffTimer, xTaskConvertMs (PHONE_FASTHOOK_DELAY));
  vTaskStop (xFastHookoffTimer);
}

// ------------------------------------------------------------------------------
// Dévalidation du décrochage rapide
static void
prvvDisableFastHookoff (void) {

  ePhoneState &= ~FASTHOOKOFF_FLAG;
  DEBUG_BLOCK () {
    vLedClear (LED_LED2);
  }
  prvvClearFastHookoff ();
}

// ------------------------------------------------------------------------------
// Tâche exécuté lors du déclenchement de la tâche xFastHookoffTimer
static void
prvvFastHookoffTask (xTaskHandle xDummy __attribute__ ((unused))) {

  if ((ePhoneState & FASTHOOKOFF_FLAG) == 0) {

    // Validation du décrochage rapide
    // PHONE_FASTHOOK_DELAY secondes après première sonnerie
    ePhoneState = FASTHOOKOFF_FLAG;
    vTaskSetInterval (xFastHookoffTimer,
                      xTaskConvertMs (PHONE_FASTHOOK_LENGTH));
    vTaskStart (xFastHookoffTimer);
    vQueuePush (&xPhoneQueue, 'F');
    DEBUG_BLOCK () {
      vLedSet (LED_LED2);
    }
  } else {

    // Dévalidation du décrochage rapide au bout de PHONE_FASTHOOK_LENGTH
    // (1 minute par défaut)
    vQueuePush (&xPhoneQueue, 'f');
    prvvDisableFastHookoff ();
  }
}

// ------------------------------------------------------------------------------
// fin d'appel
static void
prvvEndOfCall (void) {

  vTaskStop (xPhoneTimer);
  ePhoneState &= ~CALL_FLAG;
}

// ------------------------------------------------------------------------------
// Convertit le code dtmf en code ascii
static inline char
prvcDtmfToAscii (uint8_t ucDtmf) {

  if (ucDtmf < 0x0A)
    return (ucDtmf + '0');
  if (ucDtmf == 0x0A)
    return '0';
  if (ucDtmf == 0x0B)
    return '*';
  return '#';
}

// ------------------------------------------------------------------------------
static void
prvvPhoneTask (xTaskHandle xDummy __attribute__ ((unused))) {

  if ((ePhoneState & RING_FLAG) != 0) { // Fin de sonnerie

    ePhoneState &= ~RING_FLAG;  // la sonnerie n'est plus en cours
    DEBUG_BLOCK () {
      vLedClear (LED_LED1);
    }

    if (usRingLength >= RING_LENGTH_MIN) {  // sonnerie conforme

      // init xPhoneTimer pour détection fin d'appel
      vTaskSetInterval (xPhoneTimer, xTaskConvertMs (RING_INTERLEAVE));
      if (((ePhoneState & FASTHOOKOFF_FLAG) && (ucRingCount >= 1)) ||
          (ucRingCount >= ucHookOffDelay)) {

        if ((ePhoneState & HOOKOFF_REQUEST_FLAG) == 0) {

          // demande décrochage ligne
          // transmet la demande par la file des messages
          ePhoneState |= HOOKOFF_REQUEST_FLAG;
          vQueuePush (&xPhoneQueue, 'H');
        }
      } else if (ucRingCount <= PHONE_FASTHOOK_RINGS) {

        // c'est la fin d'une sonnerie n <= PHONE_FASTHOOK_RINGS
        // déclenche le processus de validation du décrochage rapide
        // Délai d'enclenchement PHONE_FASTHOOK_DELAY (10 s par défaut) sans
        // sonnerie
        vTaskStart (xFastHookoffTimer);
      } else {  // c'est la fin d'une sonnerie n > PHONE_FASTHOOK_RINGS

        prvvDisableFastHookoff ();
      }
    } else {  // sonnerie non conforme

      // Si la première sonnerie est plus courte que RING_LENGTH_MIN ...
      if (ucRingCount == 0) {

        // C'est un parasite ou sonnerie non conforme:
        // il faut l'ignorer et revenir en "attente appel"
        prvvEndOfCall ();
      } else {

        // init xPhoneTimer pour détection fin d'appel
        vTaskSetInterval (xPhoneTimer, xTaskConvertMs (RING_INTERLEAVE));
      }
    }
  } else if ((ePhoneState & CALL_FLAG) != 0) {  // Fin d'appel

    prvvEndOfCall ();
  } else if ((ePhoneState & HOOKOFF_FLAG) != 0) {

    // Demande raccrochage ligne
    // transmet la demande de raccrocher par la file des messages
    ePhoneState |= HANGUP_REQUEST_FLAG;
    vQueuePush (&xPhoneQueue, 'G');
  }
}

// ------------------------------------------------------------------------------
ISR (PHONE_RING_VECT) {

  if ((ePhoneState & RING_FLAG) == 0) { // nouvelle sonnerie

    usRingLength = 0;
    ePhoneState |= RING_FLAG; // sonnerie en cours
    // init xPhoneTimer pour la détection fin de sonnerie
    vTaskSetInterval (xPhoneTimer, xTaskConvertMs (RING_PERIOD));
    DEBUG_BLOCK () {
      vLedSet (LED_LED1);
    }
  }

  if (usRingLength == RING_LENGTH_MIN) {

    // la sonnerie a une durée minimale et peut être validée
    if ((ePhoneState & CALL_FLAG) == 0) { // nouvel appel

      ePhoneState |= CALL_FLAG; // appel en cours
      ucRingCount = 0;  // raz nb sonneries
      // réinitialise et arrête xPhoneTimerFastHook, mais n'efface pas
      // FASTHOOKOFF_FLAG !
      prvvClearFastHookoff ();
    }
    vQueuePush (&xPhoneQueue, 'R');
    vQueuePush (&xPhoneQueue, ++ucRingCount);
  }

  vTaskStart (xPhoneTimer);
  usRingLength++;
}

// ------------------------------------------------------------------------------
ISR (PHONE_DTMF_VECT) {
  uint8_t ucDtmfCode;

  ucDtmfCode = ucPhoneBoardGetDtmfCode ();
  vQueuePush (&xPhoneQueue, 'K');
  vQueuePush (&xPhoneQueue, prvcDtmfToAscii (ucDtmfCode));
  vTaskStart (xPhoneTimer); // action utilisateur, raz délai avant raccrochage
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
// Init
void
vPhoneInit (void) {

  xPhoneTimer = xTaskCreate (xTaskConvertMs (RING_PERIOD), prvvPhoneTask);
  xFastHookoffTimer = xTaskCreate (xTaskConvertMs (PHONE_FASTHOOK_DELAY),
                                   prvvFastHookoffTask);

  /* Restauration des variables en EEPROM */
  ucHookOffDelay = eeprom_read_byte (&ucHookOffDelayEemem);
  if ((ucHookOffDelay < PHONE_HOOKOFF_DELAY_MIN) ||
      (ucHookOffDelay > PHONE_HOOKOFF_DELAY_MAX)) {

    iPhoneSetHookoffDelay (PHONE_HOOKOFF_DELAY);
  }

  DEBUG_BLOCK () {
    vLedInit ();
  }
  vPhoneBoardInit ();
  vPhoneBoardDisable (PHONE_DTMF_IRQ);
  vPhoneBoardEnable (PHONE_RING_IRQ);
}

// ------------------------------------------------------------------------------
// Raccroche ligne FT
void
vPhoneHangup (void) {

  if ((prvucAtomicReadByte (&ePhoneState) & HOOKOFF_FLAG) != 0) {

    vPhoneBoardDisable (PHONE_DTMF_IRQ | PHONE_RING_IRQ);
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      vPhoneBoardClearHookoff ();
      prvvEndOfCall ();
      ucRingCount = 0;
      usRingLength = 0;
      ePhoneState = IDLE_STATE;
    }
    delay_ms (40);  // attente fin de rebond du relais, avant validation
                    // interruption
    vPhoneBoardEnable (PHONE_RING_IRQ);
  }
}

// ------------------------------------------------------------------------------
// Décroche ligne FT
void
vPhoneHookoff (void) {

  if ((prvucAtomicReadByte (&ePhoneState) & HOOKOFF_FLAG) == 0) {

    vPhoneBoardDisable (PHONE_DTMF_IRQ | PHONE_RING_IRQ);
    prvvDisableFastHookoff ();  // clear FastHook
    vPhoneBoardSetHookoff ();
    ePhoneState = HOOKOFF_FLAG;

    // raccroche si pas d'activité pendant PHONE_HANGUP_DELAY
    vTaskSetInterval (xPhoneTimer, xTaskConvertMs (PHONE_HANGUP_DELAY));
    vTaskStart (xPhoneTimer);

    delay_ms (40);  // attente fin de rebond du relais, avant validation
                    // interruption
    // Validation décodage DTMF
    vPhoneBoardEnable (PHONE_DTMF_IRQ);
  }
}

// ------------------------------------------------------------------------------
uint8_t
ucPhoneHookoffDelay (void) {

  return ucHookOffDelay;
}

// ------------------------------------------------------------------------------
int8_t
iPhoneSetHookoffDelay (uint8_t ucDelay) {

  if ((ucDelay >= PHONE_HOOKOFF_DELAY_MIN) &&
      (ucDelay <= PHONE_HOOKOFF_DELAY_MAX)) {

    ucHookOffDelay = ucDelay;
    eeprom_busy_wait ();
    eeprom_write_byte (&ucHookOffDelayEemem, ucHookOffDelay);
    return 0;
  }
  return -1;
}

// ------------------------------------------------------------------------------
uint8_t
ucPhoneMsgLength (void) {

  return (uint8_t) xQueueLength (&xPhoneQueue);
}

// ------------------------------------------------------------------------------
char
cPhoneMsgGetChar (void) {

  while (xQueueIsEmpty (&xPhoneQueue) == true) ;
  return ucQueuePull (&xPhoneQueue);
}

// ------------------------------------------------------------------------------
bool
xPhoneOffHook (void) {

  return (prvucAtomicReadByte (&ePhoneState) & HOOKOFF_FLAG) != 0;
}

// ------------------------------------------------------------------------------
bool
xPhoneHangupRequest (void) {

  return (prvucAtomicReadByte (&ePhoneState) & HANGUP_REQUEST_FLAG) != 0;
}

#endif /* AVRIO_PHONE_ENABLE defined */
/* ========================================================================== */
