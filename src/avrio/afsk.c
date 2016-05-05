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
 * @file afsk.c
 * @brief Modem Afsk 1200 HDLC - Implementation
 *
 * Revision History ------------------------------------------------------------
 *    20131208 - Initial version by epsilonRT
 */
#include <errno.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avrio/afsk.h>

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
  usIndex = (usIndex >= (AFSK_SINWAVE_LEN / 4)) ? (AFSK_SINWAVE_LEN / 2 - usIndex - 1) :
            usIndex;

  ucValue = pgm_read_byte (&ucSinWave[usIndex]);

  return (usTxPhase >= (AFSK_SINWAVE_LEN / 2)) ? (255 - ucValue) : ucValue;
}

// -----------------------------------------------------------------------------
static inline void
vStartTx (void) {

  if (!af.bTxEnable) {

    af.ucTxStuffCnt  = 0;
    af.ucTxSampleCnt = 0;
    af.ucTxBit = 0;
    af.usTxPhaseInc = AFSK_MARK_INC;
    af.usTxPhase = 0;
    af.ucPreambleCnt = CONFIG_AFSK_PREAMBLE_LEN;
    vAfskHwTxEnable();
  }
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

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
        vAfskDebugData (true);
      }
      else {

        // Transmission d'un '0' :
        // - Change la tonalité
        // - Clear le compteur de bit stuffing
        af.ucTxStuffCnt = 0;
        af.usTxPhaseInc = (af.usTxPhaseInc == AFSK_MARK_INC) ?  AFSK_SPACE_INC :
                          AFSK_MARK_INC;
        vAfskDebugData (false);
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
vTransmitter (void) {

  if (af.bTxEnable) {

    if (!af.bTestEnable) {

      vUpdateTxTone();
    }
    // Calcul de la phase du prochain échantillon
    af.usTxPhase += af.usTxPhaseInc;
    af.usTxPhase %= AFSK_SINWAVE_LEN;

    // Envoi en sortie
    vAfskHwDacWrite (AFSK_DAC_VALUE (ucSin (af.usTxPhase)));
  }
  else {

    vAfskHwDacWrite (AFSK_DAC_VALUE (128)); // sin(0) -> VREF_DAC/2
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

/* private variables ======================================================== */
/*
 * Coefficients des filtres numériques
 */
static const int8_t iCoefMkI[] = { 64,  45,   0, -45, -64, -45,   0,  45};
static const int8_t iCoefMkQ[] = {  0,  45,  64,  45,   0, -45, -64, -45};
static const int8_t iCoefSpI[] = { 64,   8, -62, -24,  55,  39, -45, -51};
static const int8_t iCoefSpQ[] = {  0,  63,  17, -59, -32,  51,  45, -39};

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
  if (bBit) {
    af.ucRxBitStream |= 1;
  }

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

  if ( (af.ucRxBitStream & HDLC_RESET) == HDLC_RESET) {

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
  vAfskDebugData (bBit);
  if ( (af.ucRxBitStream & 0x3F) == 0x3E) {

    // Bit de stuffing: on doit l'ignorer
    return iRet;
  }

  if (af.ucRxBitStream & 0x01) {

    // Injecte le dernier bit par la gauche
    af.ucRxByte |= _BV (7);
  }

  af.ucRxBitCnt++;
  if (af.ucRxBitCnt >= 8) {

    if ( (af.ucRxByte == HDLC_FLAG) ||
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

/* -----------------------------------------------------------------------------
 * Filtrage du signal à l'aide de 2 filtres passe-bande numériques:
 * - 1 centré sur 1200 Hz (Mk) -> MARK 1L
 * - 1 centré sur 2200 Hz (Sp) -> SPACE 0L
 * @return une valeur négative si Mark, nulle ou positive si SPACE
 */
static inline int16_t
iFskFilter (void) {
  int16_t iOutMkI = 0, iOutMkQ = 0, iOutSpI = 0, iOutSpQ = 0, iOut;

  for (uint8_t i = 0; i < AFSK_SAMPLES_PER_BIT; i++) {
    int8_t iSample = af.iSample[ (i + af.ucSampleIdx) % AFSK_SAMPLES_PER_BIT];

    // Calcul des composantes complexes de chaque filtre (I+jQ)
    iOutMkI += iSample * iCoefMkI[i];
    iOutMkQ += iSample * iCoefMkQ[i];
    iOutSpI += iSample * iCoefSpI[i];
    iOutSpQ += iSample * iCoefSpQ[i];
  }
  /* On calcule l'énergie en sortie des filtres (E=I²+Q²), on fait la différence
   * entre l'énergie du filtre Sp et l'énergie du filtre Mk, si le résultat est
   * négatif, c'est qu'on est sur la fréquence basse (l'énergie Mk est plus
   * grande que l'énergie Sp) et donc qu'on a un MARK
   */
  iOut = (iOutSpI >> 8) * (iOutSpI >> 8) + (iOutSpQ >> 8) * (iOutSpQ >> 8) -
         (iOutMkI >> 8) * (iOutMkI >> 8) - (iOutMkQ >> 8) * (iOutMkQ >> 8);
  return iOut;
}

// -----------------------------------------------------------------------------
static inline void
vDiscriminator (void) {

#ifndef AVRIO_AFSK_USE_RSSI
  af.iSample[af.ucSampleIdx] = iAfskHwSignalRead();
#else
  // ---------------------------------------------------------------------------
  // Version avec contrôle de qualité du signal

  if ( (af.ucSampleCnt % AFSK_SAMPLES_PER_BIT) == 0) {

    // Tous les AFSK_SAMPLES_PER_BIT échantillons, mesure la qualité
    if (ucAfskHwAdcChan() == AFSK_ADC_SIGNAL) {

      // Voie signal sélectionnée, on stocke l'échantillon
      af.iSample[af.ucSampleIdx] = iAfskHwSignalRead();
      // Puis on sélectionne la voie RSSI
      vAfskHwAdcSetChan (AFSK_ADC_RSSI);
      // On démarre une conversion simple
      vAfskHwAdcStartConvert();
      // On retourne, la prochaine interruption ADC sera pour RSSi
      return;
    }
    else {

      // Voie RSSI sélectionnée, on stocke la mesure
      af.ucRssi = ucAfskHwRssiRead();
      // Puis on re-sélectionne la voie signal
      vAfskHwAdcSetChan (AFSK_ADC_SIGNAL);
      // On passe à l'échantillon suivant
      af.ucSampleCnt++;
    }

    if (af.ucRssi < af.ucRssiTh) {
      // Qualité de signal insuffisante
      vAfskHwDcdOff();
      return;
    }
    // Bonne qualité de signal
    vAfskHwDcdOn();
  }
  else {

    // on stocke l'échantillon
    af.iSample[af.ucSampleIdx] = iAfskHwSignalRead();
    // On passe à l'échantillon suivant
    af.ucSampleCnt++;
    if (af.ucRssi < af.ucRssiTh) {
      // Qualité de signal insuffisante
      return;
    }
  }
#endif

  // Index sur l'échantillon le plus ancien
  af.ucSampleIdx = (af.ucSampleIdx + 1) % AFSK_SAMPLES_PER_BIT;

  /* Le bit échantillonné dépend de la sortie du filtre */
  int16_t iOut = iFskFilter();
  af.ucSampleBits <<= 1;
  if (iOut < 0) {

    af.ucSampleBits |= 1; // MARK
    vAfskDebugMark();
  }
  else {

    vAfskDebugSpace();
  }

  /* Ajuste la phase d'échantillonnage si un front est détecté */
  if (EDGE_FOUND (af.ucSampleBits)) {

    vAfskDebugEdgeOn();
    if (af.iRxPhaseAcc < PHASE_THRES) {
      af.iRxPhaseAcc += PHASE_INC;
    }
    else {
      af.iRxPhaseAcc -= PHASE_INC;
    }
  }
  else {

    vAfskDebugEdgeOff();
  }
  af.iRxPhaseAcc += PHASE_BIT;

  if (af.iRxPhaseAcc >= PHASE_MAX) {

    /* Echantillonnage du bit de donnée */
    af.iRxPhaseAcc %= PHASE_MAX;
    af.ucDataBits <<= 1;

    /*
     * Détermine la valeur du bit de donnée en fonction des 3 derniers bits
     * échantillonnés.
     * Si le nombre de 1 est supérieur ou égal à 2, le bit vaut '1', sinon '0'
     * Cet algorithme suppose qu'il y a 8 échantillons par bit de donnée.
     */
    uint8_t ucLastBits = af.ucSampleBits & 0x07;
    if (ucLastBits == 0x07 // 111, 3 bits set to 1
        || ucLastBits == 0x06 // 110, 2 bits
        || ucLastBits == 0x05 // 101, 2 bits
        || ucLastBits == 0x03 // 011, 2 bits
       ) {
      af.ucDataBits |= 1;
    }

    /*
     * Codage NRZI: si 2 bits consécutifs ont la même valeur, c'est un '1',
     * sinon c'est un '0'
     */
    af.iError = iHdlcParse (!EDGE_FOUND (af.ucDataBits));
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
ISR (AFSK_vect) {

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
