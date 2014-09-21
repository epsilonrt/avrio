/**
 * @file battery.c
 * @brief Chargeur de batterie
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120511 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_BATTERY_ENABLE
/* ========================================================================== */
# include "avrio-board-battery.h"
# include "task.h"
# include "mutex.h"
# include "battery.h"
# include <util/atomic.h>

/* macros =================================================================== */

/* constants ================================================================ */
#define BAT_VALUE_MIN  ADC_VALUE(BAT_CELL_VOLTAGE_MIN*BAT_CELLS, BAT_SCALE)
#define BAT_VALUE_MAX  ADC_VALUE(BAT_CELL_VOLTAGE_MAX*BAT_CELLS, BAT_SCALE)
#define BAT_VALUE_LOW  ADC_VALUE(BAT_CELL_VOLTAGE_LOW*BAT_CELLS, BAT_SCALE)
#define DV_VALUE_MAX   ADC_VALUE(BAT_CELL_DV_MAX*BAT_CELLS,BAT_SCALE)
#define PSEC_VALUE_MIN ADC_VALUE(BAT_PSEC_VOLTAGE_MIN,BAT_PSEC_SCALE)

#define BUFFER_SIZE       10
#define DROP_TIMES        6
#define DROP_TIMES_THRES  3
#define DROP_EQ_TIMES     4
#define EQU_TIMES         4

// -----------------------------------------------------------------------------
enum {
  FLAG_ENABLE = 0x80

};

// -----------------------------------------------------------------------------
enum {

  MUTEX_MONITOR  = 0x80,
  MUTEX_PROCESS  = 0x40,
};

/* structures =============================================================== */

/* types ==================================================================== */

/* private variables ======================================================== */
static uint8_t ucBatFlag;
static volatile eBatState eState;

static uint8_t ucBatCounterFast;
static uint8_t ucBatCounterDvDt;

static uint8_t ucTopTimes, ucDropTimes;
static uint16_t usDropGap;

static uint16_t usBat, usBatMax, usBatAv;
static uint8_t  ucBatIndex;
static uint16_t usBatBuffer[BUFFER_SIZE];

static xTaskHandle xTaskProcess;
static xTaskHandle xTaskMonitor;
static xMutex xBatMutex;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
prvvSetToIdle (void) {

  ucBatCounterFast = ucBatCounterDvDt = 0;
  ucTopTimes = ucDropTimes = 0;
  usBat = usBatMax = usBatAv = usDropGap = 0;
  ucBatIndex = 0;
  ucBatFlag = 0;
  eState = BAT_STATE_IDLE;
  vTaskStop (xTaskProcess);
//  vTaskStop (xTaskMonitor);
  xBatMutex = 0;
}

// -----------------------------------------------------------------------------
void
prvvSetState (eBatState eNewState) {

  if (xTaskIsStarted (xTaskProcess))
    vTaskRewind (xTaskProcess);
  eState = eNewState;
}

// -----------------------------------------------------------------------------
// loop until you have a stable value
static void
prvvWaitVoltageStabilization (void) {
  uint8_t ucIndex;
  uint16_t usMax, usMin;
  static uint16_t usBat[4];

  // Loop until the ADC value is stable. (usMax <= (usMin+1))
  for (usMax = BUFFER_SIZE, usMin = 0; usMax > (usMin + 1); ) {

    usBat[3] = usBat[2];
    usBat[2] = usBat[1];
    usBat[1] = usBat[0];
    usBat[0] = usAdcRead (BAT);
    usMin = usBat[0]; // usMin is the lower VOLTAGE
    usMax = usBat[0]; // usMax is the higher VOLTAGE

    /* Save the max and min voltage */
    for (ucIndex = 0; ucIndex <= 3; ucIndex++) {

      if (usBat[ucIndex] > usMax)
        usMax = usBat[ucIndex];
      if (usBat[ucIndex] < usMin)
        usMin = usBat[ucIndex];
    }
  }
}

// -----------------------------------------------------------------------------
// Tâche de surveillance
static void
prvvTaskProcess (xTaskHandle xDummy __attribute__ ((unused))) {

  vMutexUnlockBit (&xBatMutex, MUTEX_PROCESS);
  vTaskStart (xTaskProcess);
}

// -----------------------------------------------------------------------------
// Tâche de mesure
static void
prvvTaskMonitor (xTaskHandle xDummy __attribute__ ((unused))) {

  vMutexUnlockBit (&xBatMutex, MUTEX_MONITOR);
  vTaskStart (xTaskMonitor);
}

// -----------------------------------------------------------------------------
void
prvvPreCharging (void) {
  static uint8_t ucCounterPreCharge;

  if (xMutexTryLockBit(&xBatMutex, MUTEX_PROCESS) == 0) {

    ucCounterPreCharge++;
  }
  if (ucCounterPreCharge >= BAT_COUNTER_PRE_CHARGE) {

    // Si après 8 heures la tension batterie n'a toujours pas dépassé 0.8V
    // il faut peut être passer en erreur ?
    prvvSetState (BAT_STATE_TRICKLE);
    ucCounterPreCharge = 0;
  }
  else {

    vBatChargeSet (BAT_CHARGE_TOPOFF);
  }
}

// -----------------------------------------------------------------------------
void
prvvTopOffCharge (void) {
  static uint8_t ucCounterTopOff;

  if (xMutexTryLockBit(&xBatMutex, MUTEX_PROCESS) == 0) {

    ucCounterTopOff++;
  }
  if (ucCounterTopOff >= BAT_COUNTER_TOPOFF_CHARGE) {

    // Si après 8 heures la tension batterie n'a toujours pas dépassé 0.8V
    // il faut peut être passer en erreur ?
    prvvSetState (BAT_STATE_TRICKLE);
    ucCounterTopOff = 0;
  }
  else {

    vBatChargeSet (BAT_CHARGE_TOPOFF);
  }
}

// -----------------------------------------------------------------------------
void
prvvTrickleCharge (void) {

  vBatChargeSet (BAT_CHARGE_TRICKLE);
}

// -----------------------------------------------------------------------------
void
prvvFastCharge (void) {

  /* Toutes les 30 secondes à partir de 10 minutes
   * vérifie le 0dv -dv/dt
   */
  if ( (ucBatCounterDvDt == BAT_COUNTER_DVDT)  &&
      ( ucBatCounterFast >= 1 ) )    {

    ucBatCounterDvDt = 0;

    if (  (usBatAv >= (usBatMax - 1))  &&
          (usBatAv <= (usBatMax + 1))  ) {

      // Comptage du nombre de valeur 0dv
      ucTopTimes ++;
    }

    if ( usBatAv > (usBatMax + 1) ) {

      // tension moyenne supérieure à la tension max.

      if (ucDropTimes >= DROP_TIMES_THRES) {

          // Si le nombre de valeurs inférieures au max a franchi un seuil
          // égal à la moitié du nombre de valeur validant l'arrêt alors

          if ((usBatAv - usBatMax ) > DV_VALUE_MAX) {

            // Si l'écart par rapport au max dépasse un seuil
            ucDropTimes -= 2;
            ucTopTimes -= 2;
          }
          else {

            ucDropTimes --;
            ucTopTimes --;
          }

          // Totalise l'écart de tension
          usDropGap -= (usBatAv - usBatMax); // Pour le debug
      }
      else {

          usDropGap = 0;
          ucDropTimes = 0;
      }

      ucTopTimes = 0;
      usBatMax = usBatAv;
    }
    else if (usBatAv < (usBatMax - 1))  {

      // tension moyenne inférieure à la tension max.
      usDropGap += (usBatMax - usBatAv); // Pour le debug

      if((usBatMax - usBatAv) > DV_VALUE_MAX) {

        ucDropTimes += 2;
      }
      else {

        ucDropTimes ++;
      }

      if (ucDropTimes >= DROP_TIMES) {

        // Si le nombre de valeurs inférieures au max atteind la limite
        // passer en top-off
        prvvSetState (BAT_STATE_TOPOFF);
      }

      if ((ucTopTimes  >= EQU_TIMES ) && (ucDropTimes >= DROP_EQ_TIMES)) {

        prvvSetState (BAT_STATE_TOPOFF);
      }

      // usBatMax = usBatAv;
    }
  }

  if (xMutexTryLockBit(&xBatMutex, MUTEX_PROCESS) == 0) {

    ucBatCounterFast++;
  }

  if (ucBatCounterFast >= BAT_COUNTER_FAST_CHARGE) {

    prvvSetState (BAT_STATE_TOPOFF);
    ucBatCounterFast = 0;
  }
  else {

    vBatChargeSet (BAT_CHARGE_FAST);
  }
}

/* public variables ========================================================= */

/* internal public functions ================================================ */
void
vBatInit (void) {

  vBatChargeInit();
  vAdcInit();

  if (xTaskProcess == 0) {
    xTaskProcess = xTaskCreate (
                        xTaskConvertMs (MINUTES(BAT_PROCESS_INTERVAL_MIN)),
                        prvvTaskProcess);
  }
  if (xTaskMonitor == 0) {
    xTaskMonitor = xTaskCreate (xTaskConvertMs (BAT_MONITOR_INTERVAL_MS),
                                prvvTaskMonitor);
  }
  prvvSetToIdle();
  vTaskStart (xTaskMonitor);
}

// -----------------------------------------------------------------------------
eBatState
eBatProcess (void) {

  if (eState != BAT_STATE_ERROR) {

    // Test présence de la tension d'alim chargeur
    if (usBatVoltage(BAT_PSEC) < PSEC_VALUE_MIN) {

      // Tension d'alimentation du MAX1640 non conforme ou absente
      if ((eState > BAT_STATE_IDLE) && (eState != BAT_STATE_ERROR)) {

        prvvSetToIdle();
      }
      if (usBatVoltage(BAT) <= BAT_VALUE_LOW) {

        prvvSetState (BAT_STATE_LOW);
      }
    }
    else if (eState <= BAT_STATE_IDLE) {

      prvvSetState (BAT_STATE_PUTON);
      ucBatFlag  |= FLAG_ENABLE;
    }

    if (xMutexTryLockBit(&xBatMutex, MUTEX_MONITOR) == 0) {

      // Mesure tension batterie
      usBat = usBatVoltage (BAT);
      // Mémorisation dans le buffer
      usBatBuffer[ucBatIndex++] = usBat;

      // Tous les 10 échantillons, calcul de la moyenne pondérée des 2 extremes
      // Un échantillon toutes les 200 ms, donc un moyennage toutes les 2 secondes
      if (ucBatIndex == BUFFER_SIZE) {
        uint8_t ucIndex;
        uint16_t usSum = 0;
        uint16_t usMax;
        uint16_t usMin;

        ucBatIndex = 0;
        usMax = usBatBuffer[0];
        usMin = usBatBuffer[0];

        for (ucIndex = 0; ucIndex < BUFFER_SIZE; ucIndex++) {

          // Recherche du max.
          if(usBatBuffer[ucIndex] > usMax)
            usMax = usBatBuffer[ucIndex];

          // Recherche du min.
          if(usBatBuffer[ucIndex] < usMin)
            usMin = usBatBuffer[ucIndex];

          // Calcul de la somme
          usSum += usBatBuffer[ucIndex];
        }

        // Calcul de la moyenne pondérée des 2 extremes
        usBatAv = (usSum - usMax - usMin)  / 8;
        if ((usBatAv <= BAT_VALUE_LOW) && (eState == BAT_STATE_TRICKLE)) {

          prvvSetState (BAT_STATE_PUTON);
        }
      }

      if ((eState == BAT_STATE_FAST) && (ucBatCounterFast >= 1)) {

        // Après une occurence de compteur FAST (10 min.), incrémente le
        // compteur qui cadence le contrôle du dv/dt toutes les 30 secondes
        ucBatCounterDvDt++;
      }

      // Test erreur batterie
      if (usBat >= BAT_VALUE_MAX) {

        /* Tension batterie trop grande, action humaine requise !
         * La poursuite du processus ne pourra se faire qu'après une
         * réinitialisation par vBatInit()
         */
        vTaskStop (xTaskProcess);
        prvvSetState (BAT_STATE_ERROR);
        ucBatFlag  &= ~FLAG_ENABLE;
      }
    }

    if (ucBatFlag & FLAG_ENABLE) {

      switch (eState) {

        case BAT_STATE_PUTON:
        case BAT_STATE_PRECHARGE:
        case BAT_STATE_FAST:
         if (usBat < BAT_VALUE_MIN) {

            if (eState != BAT_STATE_PRECHARGE) {

              prvvSetState (BAT_STATE_PRECHARGE);
              vTaskStart (xTaskProcess);
            }
            prvvPreCharging();
          }
          else {

            if (eState != BAT_STATE_FAST) {

              prvvSetState (BAT_STATE_FAST);
              vTaskStart (xTaskProcess);
            }
            prvvFastCharge();
          }
          break;

        case BAT_STATE_TOPOFF:
          prvvTopOffCharge();
          break;

        case BAT_STATE_TRICKLE:
          prvvTrickleCharge();
          break;

        default:
          break;
      }
    }
    else {

      vBatChargeSet (BAT_CHARGE_OFF);
    }
  }
  else {

    // Batterie en erreur
    vBatChargeSet (BAT_CHARGE_OFF);
  }

  return eState;
}

// -----------------------------------------------------------------------------
uint16_t
usBatVoltage (uint8_t ucChannel) {
  uint16_t usVoltage;
  eBatCharge eCurrentCharge = -1;

  if (ucChannel == BAT) {

    eCurrentCharge = eBatChargeGet();
    vBatChargeSet (BAT_CHARGE_OFF);
    prvvWaitVoltageStabilization();
  }

  usVoltage = usAdcReadAverage (ucChannel, 16);

  if (ucChannel == BAT)
    vBatChargeSet (eCurrentCharge); // Restore charge

  return usVoltage;
}

// -----------------------------------------------------------------------------
uint16_t
usBatVoltageAverage (void) {

  if (usBatAv == 0)
    return usBat;
  else
    return usBatAv;
}

// -----------------------------------------------------------------------------
uint16_t
usBatVoltageMax (void) {

  return usBatMax;
}

/* ========================================================================== */
#endif /* AVRIO_BATTERY_ENABLE defined */
