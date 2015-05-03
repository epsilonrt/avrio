/**
 * @file hih6130.c
 * @brief Thermostat I2c DS1621 de DALLAS/MAXIM - Implémentation
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/hih6130.h>
#include <avrio/delay.h>

/* constants ================================================================ */

typedef enum {
  HIH6130_DONE  = 0x00, /*< Données valides renvoyées */
  HIH6130_STALE = 0x40, /*< Donnée déjà lue */
  HIH6130_CMD   = 0x80, /*< Capteur en mode commande */
  HIH6130_DIAG  = 0xC0, /*< Diagnostic en cours */
  HIH6130_STATUS = HIH6130_DIAG /**< Masque des bits d'état */
} eHih6130Status;

enum {
  MR_PROGRESS    = 0x01, /* Demande de mesure en cours */
  DATA_AVAILABLE = 0x02  /* Données disponibles */
};

/* public variables ========================================================= */
eTwiStatus eHih6130LastTwiErrorValue;

/* private variables ======================================================== */
static uint8_t ucHihStatus;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
eHih6130Error
eHih6130Init (uint8_t ucConfig) {

  ucHihStatus = 0;
  delay_ms (100); // Startup-time 60 ms max.
  
  // Démarrage d'une mesure pour vérification de présence du capteur
  if (eHih6130Start () != HIH6130_SUCCESS) {

    /* Erreur sur le bus I2C */
    return HIH6130_TWIERR;
  }
  else {
    eHih6130Error ret;
    xHih6130Data mes;
    
    // Lecture du résultat de la mesure pour vérification
    do {
      ret = eHih6130Read (&mes);
      if (ret < HIH6130_SUCCESS) {
        return ret;
      }
      delay_ms (10);
    }
    while (ret == HIH6130_BUSY);
  }

  return HIH6130_SUCCESS;
}

// -----------------------------------------------------------------------------
eHih6130Error
eHih6130Start (void) {
  xTwiFrame xFrame;

  /* Ecriture d'une trame vide = Measurement Request */
  xFrame.xAddr = HIH6130_ADDR;
  xFrame.xLen = 0;
  eHih6130LastTwiErrorValue = eTwiSend (&xFrame);

  if (eHih6130LastTwiErrorValue != TWI_SUCCESS) {

    /* Erreur sur le bus I2C */
    ucHihStatus = 0;
    return HIH6130_TWIERR;
  }

  ucHihStatus = MR_PROGRESS; /* Demande de mesure en cours ... */
  return HIH6130_SUCCESS;
}

// -----------------------------------------------------------------------------
eHih6130Error
eHih6130Read (xHih6130Data * pxData) {
  static xHih6130Data xLastData;

  if (ucHihStatus == MR_PROGRESS) {
    uint8_t ucBuffer[4];
    xTwiFrame xFrame;

    /* Mesure en cours */

    // Fetch Data
    xFrame.xAddr = HIH6130_ADDR;
    xFrame.pxData = ucBuffer;
    xFrame.xLen = 4;
    eHih6130LastTwiErrorValue = eTwiReceive (&xFrame);

    if (eHih6130LastTwiErrorValue != TWI_SUCCESS) {
      return HIH6130_TWIERR;
    }

    if (xFrame.xLen < 2) {
      return HIH6130_NODATA;
    }

    switch (ucBuffer[0] & HIH6130_STATUS) {

      case HIH6130_DONE: {
        int32_t lValue;

        /* Nouvelles données disponibles */
        ucHihStatus = DATA_AVAILABLE;
        // Calcul de l'humidité relative en dixième de %
        lValue = ( (ucBuffer[0] & ~HIH6130_STATUS) << 8) + ucBuffer[1];
        lValue = (lValue * 1000L) / 16383L;
        xLastData.iHum = (int16_t) lValue;

        if (xFrame.xLen == 4) {

          // Calcul de la température en dixième de oC
          lValue = (ucBuffer[2] << 8) + ucBuffer[3];
          lValue = ( (lValue >> 2) * 1650L) / 16383L - 400L;
          xLastData.iTemp = (int16_t) lValue;
        }
        else {

          xLastData.iTemp = -32768;
        }

      }
      break;

      case HIH6130_STALE:
        return HIH6130_BUSY;

      case HIH6130_CMD:
      case HIH6130_DIAG:
        break;
    }
  }

  if (ucHihStatus == DATA_AVAILABLE) {

    /* Recopie dernières données disponibles */
    pxData->iHum  = xLastData.iHum;
    pxData->iTemp = xLastData.iTemp;
  }
  else {

    /* Aucune mesure n'a encore été démarrée... */
    return HIH6130_NODATA;
  }

  return HIH6130_SUCCESS;
}

/* ========================================================================== */
