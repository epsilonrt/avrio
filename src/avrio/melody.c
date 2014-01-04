/**
 * @file melody.c
 * @brief Module de génération de mélodie.
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include "avrio-config.h"

#ifdef AVRIO_MELODY_ENABLE
/* ========================================================================== */
#  include "avrio-board-melody.h"
#  include <avrio/task.h>
#  include <avrio/melody.h>
#  include <util/atomic.h>
#  include <avr/pgmspace.h>

/* constants ================================================================ */
#  define INIT_FLAG 0x80/* Flag indiquant que le module est initialisé */
#  define PGM_FLAG  0x40/* Flag indiquant que le morceau réside en FLASH */

enum {
  IDLE_STATE = 1, /* module au repos */
  PLAY_STATE, /* une note est actuellement jouée */
  PAUSE_STATE /* une internote est actuellement jouée */
};

/* types ==================================================================== */
typedef const xNote * PROGMEM xSystemMelody;

/* private variables ======================================================== */
static uint8_t ucMelodyFlags; /* < Flags du module */
static uint8_t xMelodyDuration; /* < durée du 16ième de note */
static uint8_t xMelodyInternote;  /* < durée de l'espace internote en ms */
static const xNote *pxMelodyNextNote; /* < pointeur sur la note courante */
static xTaskHandle xMelodyTimer;  /* < tâche gérant le cadencement du morceau */
static volatile uint8_t ucMelodyState;

/* ========== FLASH ========== */
/*
 * Table de conversion pitch -> fréquence
 * L'index 0 correspond au pitch A0
 * 7 octaves = 7*12*2+2 = 170 octets
 */
static const uint16_t pxPitchToFreq[] PROGMEM = {
  /*
   * A Am H C Cm D Dm E F Fm G Gm La La# Si Do Do# Ré Ré# Mi Fa Fa# Sol Sol# --
   * Octave */
  55, 58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 104,  // 0
  110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, // 1
  220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, // 2
  440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, // 3
  880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661,  // 4
  1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, // 5
  3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, // 6
  7040, 7459, 7902, 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544, 13290,  // 7
  14080 // 8
};

/* beep système */
static const xNote pxBeep[] PROGMEM = {
  {PITCH_F5, 5},
  {PITCH_END, 0}
};

static const xNote pxWelcome[] PROGMEM = {
  {PITCH_F4, 10},
  {PITCH_F5, 5},
  {PITCH_F4, 20},
  {PITCH_END, 0}
};

static const xNote pxWarning[] PROGMEM = {
  {PITCH_F4, 5},
  {PITCH_F5, 10},
  {PITCH_F4, 20},
  {PITCH_END, 0}
};

static const xNote pxError[] PROGMEM = {
  {PITCH_F4, 5},
  {PITCH_F5, 10},
  {PITCH_F6, 20},
  {PITCH_END, 0}
};

/* la table des sons système */
static const xSystemMelody pxSystemMelodies[] PROGMEM = {
  pxBeep,
  pxWelcome,
  pxWarning,
  pxError
};

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
prvvMelodyTask (xTaskHandle xDummy __attribute__ ((unused))) {

  switch (ucMelodyState) {

    case IDLE_STATE:
    case PAUSE_STATE:
      if (pxMelodyNextNote) {
        const xNote *pxNextNote;
        static xNote xPgmNote;  // copie de la note FLASH en RAM

        if (ucMelodyFlags & PGM_FLAG) {

          memcpy_P (&xPgmNote, pxMelodyNextNote, sizeof (xNote));
          pxNextNote = &xPgmNote;
        } else {

          pxNextNote = pxMelodyNextNote;
        }

        if (pxNextNote->pitch < PITCH_MAX) {

          if (pxNextNote->pitch != PITCH_PAUSE) {

            // si la note n'est pas une pause
            vMelodyHardwarePlay (pgm_read_word_near
                                 (&pxPitchToFreq[pxNextNote->pitch]));
          } else {

            // sinon commencer à jouer une pause
            vMelodyHardwareStop ();
          }
          /* calcule la durée de la note ou de la pause, et modifie la tâche en
           * conséquence */
          vTaskSetInterval (xMelodyTimer,
                            xTaskConvertMs (xMelodyDuration *
                                            pxNextNote->length -
                                            xMelodyInternote));
          pxMelodyNextNote++; // passer à la note suivante
          ucMelodyState = PLAY_STATE;
          vTaskStart (xMelodyTimer);
          return;
        }
      }
      vMelodyStop (); // si note.pitch >= PITCH_MAX ou pointeur nul -> arrêter
                      // le son
      break;

    case PLAY_STATE:
      vMelodyHardwareStop ();
      ucMelodyState = PAUSE_STATE;
      vTaskSetInterval (xMelodyTimer, xTaskConvertMs (xMelodyInternote));
      vTaskStart (xMelodyTimer);
      break;
  }
}

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
void
vMelodyInit (uint8_t xDuration, uint8_t xInternote) {

  vMelodyHardwareInit ();
  ucMelodyState = IDLE_STATE;
  if ((ucMelodyFlags & INIT_FLAG) == 0) {

    xMelodyTimer = xTaskCreate (0, prvvMelodyTask);
  }
  ucMelodyFlags = INIT_FLAG;
  vMelodyStop ();
  xMelodyDuration = xDuration;
  xMelodyInternote = xInternote;
}

// ------------------------------------------------------------------------------
void
vMelodyPlay (const xNote * pxMelody) {

  vMelodyStop ();
  ucMelodyFlags &= ~PGM_FLAG;
  pxMelodyNextNote = pxMelody;
  vTaskStart (xMelodyTimer);
}

// ------------------------------------------------------------------------------
void
vMelodyPlayFlash (const xNote * pxMelody) {

  vMelodyStop ();
  ucMelodyFlags |= PGM_FLAG;
  pxMelodyNextNote = pxMelody;
  vTaskStart (xMelodyTimer);
}

// ------------------------------------------------------------------------------
void
vMelodyPlaySystem (uint8_t ucIndex) {
  const xNote *pxSystemMelody;

  if (ucIndex < MELODY_SYS_MAX) {

    memcpy_P (&pxSystemMelody, &pxSystemMelodies[ucIndex],
              sizeof (const xNote PROGMEM *));
    vMelodyPlayFlash (pxSystemMelody);
  }
}

// ------------------------------------------------------------------------------
void
vMelodyStop (void) {

  if (xMelodyIsPlay ()) {
    /* ressource occupée donc la lecture est en cours ... arrête la lecture */
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      vMelodyHardwareStop ();
      vTaskStop (xMelodyTimer);
      ucMelodyState = IDLE_STATE;
    }
  }
}

/* inline public functions ================================================== */

// ------------------------------------------------------------------------------
inline void
vMelodySetDuration (uint8_t xDuration) {

  xMelodyDuration = xDuration;
}

// ------------------------------------------------------------------------------
inline void
vMelodySetInternote (uint8_t xInternote) {

  xMelodyInternote = xInternote;
}

// ------------------------------------------------------------------------------
inline void
vMelodyBeep (void) {

  vMelodyPlaySystem (MELODY_BEEP);
}

// ------------------------------------------------------------------------------
inline void
vMelodyPause (bool xPause) {

  if (xPause) {

    vTaskStop (xMelodyTimer);
    vMelodyHardwareStop();
  }
  else {

    vTaskStart (xMelodyTimer);
  }
}

// ------------------------------------------------------------------------------
inline bool
xMelodyIsPlay (void) {

  return xTaskIsStarted (xMelodyTimer);
}

// ------------------------------------------------------------------------------
inline uint8_t
ucMelodyDuration (void) {

  return xMelodyDuration;
}

// ------------------------------------------------------------------------------
inline uint8_t
ucMelodyInternote (void) {

  return xMelodyInternote;
}

#endif /* AVRIO_MELODY_ENABLE defined */
/* ========================================================================== */
