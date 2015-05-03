/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file afsk.c
 * @brief Modem Afsk 1200 HDLC - Implementation
 *
 * Revision History ------------------------------------------------------------
 *    20131208 - Initial version by epsilonRT
 */
#include <errno.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "afsk.h"

/* constants ================================================================ */
// Nombre max. de bit consécutifs à '1'
#define BIT_STUFF_LIMIT 5

/* public variables ========================================================= */
xAfsk af;

/* =============================================================================
 *
 *                        Modulateur / Transmetteur
 *
 * ===========================================================================*/
#ifndef AFSK_TX_DISABLE
/* constants ================================================================ */

#if CONFIG_AFSK_PREAMBLE_LEN < 1
#undef CONFIG_AFSK_PREAMBLE_LEN
#warning "CONFIG_AFSK_PREAMBLE_LEN must be greater or equal to 1"
#define CONFIG_AFSK_PREAMBLE_LEN 1
#endif

#if CONFIG_AFSK_TRAILER_LEN < 1
#undef CONFIG_AFSK_TRAILER_LEN
#warning "CONFIG_AFSK_TRAILER_LEN must be greater or equal to 1"
#define CONFIG_AFSK_TRAILER_LEN 1
#endif

/* private variables ======================================================== */

// Table sinus, elle contient 128 valeurs pour les phases 0 et 90°
// Les autres valeurs sont calculées par symétrie par ucSin()
static const uint8_t PROGMEM ucSinWave[] = {
  128, 129, 131, 132, 134, 135, 137, 138, 140, 142, 143, 145, 146, 148, 149, 151,
  152, 154, 155, 157, 158, 160, 162, 163, 165, 166, 167, 169, 170, 172, 173, 175,
  176, 178, 179, 181, 182, 183, 185, 186, 188, 189, 190, 192, 193, 194, 196, 197,
  198, 200, 201, 202, 203, 205, 206, 207, 208, 210, 211, 212, 213, 214, 215, 217,
  218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
  234, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 243, 244, 245,
  245, 246, 246, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 252,
  253, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255
};

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
// Valeur du sinus entre 0 et 255 -> sin(0) = 128
uint8_t
ucSin (uint16_t usTxPhase) {
  uint16_t usIndex;
  uint8_t ucValue;

  usIndex = usTxPhase % (AFSK_SINWAVE_LEN / 2);
  usIndex = (usIndex >= (AFSK_SINWAVE_LEN / 4)) ?  (AFSK_SINWAVE_LEN / 2 - usIndex - 1) :
                                              usIndex;

  ucValue = pgm_read_byte(&ucSinWave[usIndex]);

  return (usTxPhase >= (AFSK_SINWAVE_LEN / 2)) ? (255 - ucValue) : ucValue;
}

// -----------------------------------------------------------------------------
static inline void
vStartTx(void) {

  if (!af.bTxEnable) {

    af.ucTxStuffCnt  = 0;
    af.ucTxSampleCnt = 0;
    af.ucTxBit = 0;
    af.usTxPhaseInc = AFSK_MARK_INC;
    af.usTxPhase = 0;
    af.ucPreambleCnt = CONFIG_AFSK_PREAMBLE_LEN;
    vAfskHwTxEnable();
  }
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

    af.bTxEnable = 1;
    af.ucTrailerCnt  = CONFIG_AFSK_TRAILER_LEN;
  }
}

// -----------------------------------------------------------------------------
static inline void
vUpdateTxTone (void) {

  /* Vérifies si le temps de bit est terminé */
  if (af.ucTxSampleCnt == 0) {

    if (af.ucTxBit == 0) {

      // Tous les ucLastBits de l'octet en cours transmis...
      if (xQueueIsEmpty (&af.xTxFifo) && (af.ucTrailerCnt == 0)) {

        // Si plus rien à transmettre, arrêter l'émetteur
        vAfskHwTxDisable();
        af.bTxEnable = 0;
        return; // EOF
      }
      else {
        // Il reste des octets à transmettre

        if (!af.bStuffEnable) {

          // Si nous venons de terminer l'envoi d'un octet non 'stuffé',
          // il faut réinitialiser le compteur de ucLastBits de stuffing
          af.ucTxStuffCnt = 0;
        }

        // Le stuffing doit être activé par défaut sauf pour les codes HDLC
        af.bStuffEnable = 1;

        // Gestion des flags de préambule et fin
        if (af.ucPreambleCnt != 0) {

          // Transmission du préambule
          af.ucPreambleCnt--;
          af.ucTxByte = HDLC_FLAG;
        }
        else {

          // Préambule transmis
          if (xQueueIsEmpty (&af.xTxFifo)) {

            // Plus d'info à transmettre, on passe à la fin de trame
            af.ucTrailerCnt--;
            af.ucTxByte = HDLC_FLAG;
          }
          else {

            // Il reste des infos, on dépile l'octet suivant
            af.ucTxByte = ucQueuePull (&af.xTxFifo);
          }
        }

        if (af.ucTxByte == HDLC_ESC) {

          // L'octet est un escape (ESC)
          if (xQueueIsEmpty (&af.xTxFifo)) {

            // Un ESC doit être suivi d'un octet, ici ce n'est pas le cas,
            // c'est une erreur -> arrêt transmission
            vAfskHwTxDisable ();
            af.bTxEnable = 0;
            return; // EOF
          }
          else {

            // Transmission de l'octet qui suit l'escape
            af.ucTxByte = ucQueuePull (&af.xTxFifo);
          }
        }
        else {

          // L'octet n'est pas un escape (ESC)
          if (af.ucTxByte == HDLC_FLAG || af.ucTxByte == HDLC_RESET) {

            // Si ces caractères ne sont pas échappés, dévalider le bit stuffing
            // car on transmet un flag HDLC ou un reset HDLC
            af.bStuffEnable = 0;
          }
        }
      }

      // Démarre avec le masque du bit de poids faible
      af.ucTxBit = 0x01;
      // --- Fin du passage à l'octet suivant, si nous sommes arrivé là, c'est
      // que la trame n'est pas finie !
    }

    // Vérifies si bit stuffing nécessaire
    if (af.bStuffEnable && af.ucTxStuffCnt >= BIT_STUFF_LIMIT) {

      // Si il y a plus de 5 '1' dde suite inséres un 0
      af.ucTxStuffCnt = 0;

      // Changement de tonalité
      af.usTxPhaseInc = (af.usTxPhaseInc == AFSK_MARK_INC) ?  AFSK_SPACE_INC :
                                                          AFSK_MARK_INC;
    }
    else {

      // NRZI: si l'on veut transmettre un 1, la modulation de fréquence va
      // rester inchangée; avec un 0, il y aura un changement de ton.
      if (af.ucTxByte & af.ucTxBit) {

        // Transmission d'un '1' :
        // - Reste sur la tonalité précédente
        // - Incrémente le compteur de bit stuffing
        af.ucTxStuffCnt++;
        vAfskDebugData(true);
      }
      else {

        // Transmission d'un '0' :
        // - Change la tonalité
        // - Clear le compteur de bit stuffing
        af.ucTxStuffCnt = 0;
        af.usTxPhaseInc = (af.usTxPhaseInc == AFSK_MARK_INC) ?  AFSK_SPACE_INC :
                                                            AFSK_MARK_INC;
        vAfskDebugData(false);
      }

      // Passe au bit suivant
      af.ucTxBit <<= 1;
    }
    // Redémarre le compteur pour le temps de bit
    af.ucTxSampleCnt = AFSK_SAMPLES_PER_BIT;
  }

  af.ucTxSampleCnt--;
}

// -----------------------------------------------------------------------------
static inline void
vTransmitter(void) {

  if (af.bTxEnable) {

    if (!af.bTestEnable) {

      vUpdateTxTone();
    }
    // Calcul de la phase du prochain échantillon
    af.usTxPhase += af.usTxPhaseInc;
    af.usTxPhase %= AFSK_SINWAVE_LEN;

    // Envoi en sortie
    vAfskHwDacWrite (AFSK_DAC_VALUE(ucSin(af.usTxPhase)));
  }
  else {

    vAfskHwDacWrite (AFSK_DAC_VALUE(128)); // sin(0) -> VREF_DAC/2
  }
}

/* avr-libc stdio interface ================================================= */

// -----------------------------------------------------------------------------
static int
iPutChar (char c, FILE * pxStream) {

  vQueueWaitUntilIsFull (&af.xTxFifo);
  vQueuePush (&af.xTxFifo, c);
  vStartTx();

  return 0;
}
#else
#define vTransmitter()
#endif  // AFSK_TX_DISABLE not defined
/* ========================Fin Modulateur/Transmetteur========================*/

/* =============================================================================
 *
 *                        Démodulateur / Récepteur
 *
 * ===========================================================================*/
#ifndef AFSK_RX_DISABLE

/* constants ================================================================ */
#define PHASE_BIT    8
#define PHASE_INC    1
#define PHASE_MAX    (AFSK_SAMPLES_PER_BIT * PHASE_BIT) // 64
#define PHASE_THRES  (PHASE_MAX / 2)  // 32
#define DCD_THRES    30

#define BIT_DIFFER(bitline1, bitline2) (((bitline1) ^ (bitline2)) & 0x01)
#define EDGE_FOUND(bitline)            BIT_DIFFER((bitline), (bitline) >> 1)

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static inline int
iPush (uint8_t ucByte) {

  if (!xQueueIsFull (&af.xRxFifo)) {

    vQueuePush (&af.xRxFifo, ucByte);
  }
  else {

    // Débordement de pile
    af.bFrameStarted = 0;
    vAfskHwDcdOff();
    return AFSK_RXFIFO_OVERRUN;
  }
  return 0;
}

// -----------------------------------------------------------------------------
static inline int
iHdlcParse (bool bBit) {
  int  iRet = 0;

  af.ucRxBitStream <<= 1;
  if (bBit)
    af.ucRxBitStream |= 1;

  if (af.ucRxBitStream == HDLC_FLAG) {

    //if (af.bFrameStarted == 0) {

      iRet = iPush (HDLC_FLAG);
      if (iRet == 0)  {

        // Début de trame détecté
        af.bFrameStarted = 1;
        vAfskHwDcdOn();
      }
    //}
    af.ucRxByte = 0;
    af.ucRxBitCnt = 0;
    return iRet;
  }

  if ((af.ucRxBitStream & HDLC_RESET) == HDLC_RESET) {

    // Fin de trame
    af.bFrameStarted = 0;
    vAfskHwDcdOff();
    return iRet;
  }

  if (af.bFrameStarted == 0) {

    // Liaison non synchronisée
    return iRet;
  }

  // bFrameStarted = 1, Traitement des octets un fois la liaison synchronisée...
  vAfskDebugData(bBit);
  if ((af.ucRxBitStream & 0x3F) == 0x3E) {

    // Bit de stuffing: on doit l'ignorer
    return iRet;
  }

  if (af.ucRxBitStream & 0x01) {

    // Injecte le dernier bit par la gauche
    af.ucRxByte |= _BV(7);
  }

  af.ucRxBitCnt++;
  if (af.ucRxBitCnt >= 8) {

    if ((af.ucRxByte == HDLC_FLAG) ||
        (af.ucRxByte == HDLC_RESET) ||
        (af.ucRxByte == HDLC_ESC)) {

      // Réception d'un caractère spécial, ajout d'un code d'échappement
      // dans la pile. Ce code devra être supprimé par la couche supérieure Ax25
      iRet = iPush (HDLC_ESC);
    }

    // Ajout de l'octet reçu dans la pile
    iRet = iPush (af.ucRxByte);
    af.ucRxByte = 0;
    af.ucRxBitCnt = 0;
  }
  else {

    af.ucRxByte >>= 1;
  }

  return iRet;
}

#if (CONFIG_AFSK_FILTER == AFSK_FIR)
// -----------------------------------------------------------------------------
// Filtre non récursif (Filtre à réponse impulsionnelle finie)
#warning "The FIR filter is not yet functional"

/* constants ================================================================ */
#define FIR_DCD_LEVEL 5
#define FIR_MAX_TAPS 16

/* structures =============================================================== */
typedef struct xFir {

  int8_t iTaps;
  int8_t iCoef[FIR_MAX_TAPS];
  int16_t iMem[FIR_MAX_TAPS];
} xFir;

/* types ==================================================================== */
typedef enum {

  FIR_1200_BP=0,
  FIR_2200_BP=1,
  FIR_1200_LP=2
} eFirFilters;

/* private variables ======================================================== */
static xFir xFirTable[] = {
  [FIR_1200_BP] = {
    .iTaps = 11,
    .iCoef = { -12, -16, -15, 0, 20, 29, 20, 0, -15, -16, -12},
    .iMem  = { 0, },
  },
  [FIR_2200_BP] = {
    .iTaps = 11,
    .iCoef = { 11, 15, -8, -26, 4, 30, 4, -26, -8, 15, 11 },
    .iMem  = { 0, },
  },
  [FIR_1200_LP] = {
    .iTaps = 8,
    .iCoef = { -9, 3, 26, 47, 47, 26, 3, -9 },
    .iMem  = { 0, },
  },
};

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static int8_t
iFirFilter (int8_t s, eFirFilters f) {
  int8_t   Q = xFirTable[f].iTaps - 1;
  int8_t  *B = xFirTable[f].iCoef;
  int16_t *Bmem = xFirTable[f].iMem;

  int8_t i;
  int16_t y;

  Bmem[0] = s;
  y = 0;

  for (i = Q; i >= 0; i--) {

    y += Bmem[i] * B[i];
    Bmem[i + 1] = Bmem[i];
  }

  return (int8_t) (y / 128);
}
// -----------------------------------------------------------------------------
#endif  // CONFIG_AFSK_FILTER == AFSK_FIR

/* -----------------------------------------------------------------------------
 * La discrimination en fréquence est effectuée par une simple multiplication
 * de l'échantillon avec l'échantillon précédent puis divisé par 2.
 * Le signal est ensuite filtré par un passe-bas du 1er ordre à 600 Hz
 * L'implémentation du filtre peut être choisi par CONFIG_AFSK_FILTER
 */
static inline void
vDiscriminator (void) {
  static int16_t iFilterY[2]; // Valeurs de sortie du filtre
  /*
   * Bits échantilloné par le discriminateur
   * Comme la fréquence d'échantillonage de l'ADC est plus grande que la vitesse
   * de transmission, il faut pouvoir stocker AFSK_SAMPLES_PER_BIT ucLastBits.
   * TODO: modifier la taille en fonction de AFSK_SAMPLES_PER_BIT
   */
  static uint8_t ucSampledBits;
  /*
   * Accumulateur de phase actuelle
   * Permet de savoir quand le bit doit être échantillonné
   */
  static int8_t iRxPhase;
  /*
   * Bits trouvés à la vitesse de modulation correcte
   */
  static uint8_t ucDataBits;

  // Lecture de l'échantillon
  int8_t iSample = iAfskHwAdcRead();

#if (CONFIG_AFSK_FILTER != AFSK_FIR)
// -----------------------------------------------------------------------------
// Filtre récursif (Filtre à réponse impulsionnelle infinie)
  static int16_t iFilterX[2];

  iFilterX[0] = iFilterX[1];

  /*
   * TODO:
   * - ci-dessous la division se fait par 4 !
   */
  #if (CONFIG_AFSK_FILTER == AFSK_BUTTERWORTH)
    iFilterX[1] = ((int8_t)ucQueuePull(&af.xSampleFifo) * iSample) >> 2;
    //iFilterX[1] = ((int8_t)ucQueuePull(&af.xSampleFifo) * iSample) / 6.027339492;
  #elif (CONFIG_AFSK_FILTER == AFSK_CHEBYSHEV)
    iFilterX[1] = ((int8_t)ucQueuePull(&af.xSampleFifo) * iSample) >> 2;
    //iFilterX[1] = ((int8_t)ucQueuePull(&af.xSampleFifo) * iSample) / 3.558147322;
  #else
    #error "Filter type not found!"
  #endif

  iFilterY[0] = iFilterY[1];

  #if CONFIG_AFSK_FILTER == AFSK_BUTTERWORTH
    /*
     * This strange sum + shift is an optimization for iFilterY[0] * 0.668.
     * iir * 0.668 ~= (iir * 21) / 32 =
     * = (iir * 16) / 32 + (iir * 4) / 32 + iir / 32 =
     * = iir / 2 + iir / 8 + iir / 32 =
     * = iir >> 1 + iir >> 3 + iir >> 5
     */
    iFilterY[1] = iFilterX[0] + iFilterX[1] + (iFilterY[0] >> 1) +
                  (iFilterY[0] >> 3) + (iFilterY[0] >> 5);
    //iFilterY[1] = iFilterX[0] + iFilterX[1] + iFilterY[0] * 0.6681786379;
  #elif CONFIG_AFSK_FILTER == AFSK_CHEBYSHEV
    /*
     * This should be (iFilterY[0] * 0.438) but
     * (iFilterY[0] >> 1) is a faster approximation :-)
     */
    iFilterY[1] = iFilterX[0] + iFilterX[1] + (iFilterY[0] >> 1);
    //iFilterY[1] = iFilterX[0] + iFilterX[1] + iFilterY[0] * 0.4379097269;
  #endif

  /* Le bit échantillonné dépend de la sortie du filtre */
  ucSampledBits <<= 1;
  if (iFilterY[1] > 0)
    ucSampledBits |= 1;

#if 0
  // Détection de la porteuse
  // TODO: non fonctionnel
  if (ABS(iFilterY[1]) - 20 > 0) {

    af.ucCarrierCnt++;
    if (af.ucCarrierCnt > DCD_THRES) {

      af.ucCarrierCnt = DCD_THRES;
      vAfskHwDcdOn();
      af.bDcd = 1;
    }
  }
  else {

    if (af.ucCarrierCnt > 0) {

      af.ucCarrierCnt --;
      if (af.ucCarrierCnt == 0) {

        vAfskHwDcdOff();
        af.bDcd = 0;
      }
    }
  }
#endif // Détection de la porteuse

  /* Store current ADC sample in the af.xSampleFifo */
  vQueuePush(&af.xSampleFifo, iSample);

#elif (CONFIG_AFSK_FILTER == AFSK_FIR)
// -----------------------------------------------------------------------------
// Filtre non récursif (Filtre à réponse impulsionnelle finie)

  iFilterY[0] = ABS(iFirFilter(iSample, FIR_1200_BP));
  iFilterY[1] = ABS(iFirFilter(iSample, FIR_2200_BP));

  ucSampledBits <<= 1;
  ucSampledBits |= iFirFilter(iFilterY[1] - iFilterY[0], FIR_1200_LP) > 0;

#if 0
  // Détection de la porteuse
  // TODO: non fonctionnel
  if ((iFilterY[1] > FIR_DCD_LEVEL) || (iFilterY[0] > FIR_DCD_LEVEL)) {

    af.ucCarrierCnt++;
    if (af.ucCarrierCnt > DCD_THRES) {

      af.ucCarrierCnt = DCD_THRES;
      vAfskHwDcdOn();
      af.bDcd = 1;
    }
  }
  else {

    if (af.ucCarrierCnt > 0) {

      af.ucCarrierCnt --;
      if (af.ucCarrierCnt == 0) {

        vAfskHwDcdOff();
        af.bDcd = 0;
      }
    }
  }
#endif // Détection de la porteuse
// -----------------------------------------------------------------------------
#endif  // CONFIG_AFSK_FILTER == AFSK_FIR

  /* Ajuste la phase d'échantillonnage si un front est détecté */
  if (EDGE_FOUND(ucSampledBits)) {

    vAfskDebugEdgeOn();
    if (iRxPhase < PHASE_THRES)
      iRxPhase += PHASE_INC;
    else
      iRxPhase -= PHASE_INC;
  }
  else {

    vAfskDebugEdgeOff();
  }
  iRxPhase += PHASE_BIT;

  if (iRxPhase >= PHASE_MAX) {

    /* Echantillonnage du bit de donnée */
    iRxPhase %= PHASE_MAX;
    ucDataBits <<= 1;

    /*
     * Détermine la valeur du bit de donnée en fonction des 3 derniers bits
     * échantillonnés.
     * Si le nombre de 1 est supérieur ou égal à 2, le bit vaut '1', sinon '0'
     * Cet algorithme suppose qu'il y a 8 échantillons par bit de donnée.
     */
    uint8_t ucLastBits = ucSampledBits & 0x07;
    if (ucLastBits == 0x07 // 111, 3 bits set to 1
     || ucLastBits == 0x06 // 110, 2 bits
     || ucLastBits == 0x05 // 101, 2 bits
     || ucLastBits == 0x03 // 011, 2 bits
    )
      ucDataBits |= 1;

    /*
     * Codage NRZI: si 2 bits consécutifs ont la même valeur, c'est un '1',
     * sinon c'est un '0'
     */
    af.iError = iHdlcParse(!EDGE_FOUND(ucDataBits));
  }

}

/* avr-libc stdio interface ================================================= */

// -----------------------------------------------------------------------------
static int
iGetChar (FILE * pxStream) {

  if (af.iError) {

    errno = af.iError;
    af.iError = 0;
    return _FDEV_ERR;
  }

  if (af.ucMode & AFSK_MODE_NOBLOCK) {

    // Mode non bloquant
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      if (!xQueueIsEmpty (&af.xRxFifo)) {

        return (unsigned int) ucQueuePull (&af.xRxFifo);
      }
    }
  }
  else {

    // Mode bloquant
    while (xQueueIsEmpty (&af.xRxFifo)) {

      // TODO: Timeout ?
      wdt_reset();
    }
    return (unsigned int) ucQueuePull (&af.xRxFifo);
  }

  return _FDEV_EOF;
}
#else
#define vDiscriminator()
#endif  // AFSK_RX_DISABLE not defined
/* ========================Fin Démodulateur/Récepteur=========================*/

// -----------------------------------------------------------------------------
ISR(AFSK_vect) {

  vDiscriminator ();
  vTransmitter();
}

/* public variables ========================================================= */

#if !defined(AFSK_RX_DISABLE) && !defined(AFSK_TX_DISABLE)
FILE xAfskPort = FDEV_SETUP_STREAM (iPutChar, iGetChar, _FDEV_SETUP_RW);
#elif defined(AFSK_TX_DISABLE)
FILE xAfskPort = FDEV_SETUP_STREAM (NULL, iGetChar, _FDEV_SETUP_READ);
#elif defined(AFSK_RX_DISABLE)
FILE xAfskPort = FDEV_SETUP_STREAM (iPutChar, NULL, _FDEV_SETUP_WRITE);
#endif

/* ========================================================================== */
