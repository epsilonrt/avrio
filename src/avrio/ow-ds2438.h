/**
 * Copyright © 2017 epsilonRT. All rights reserved.
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
 * @file
 * @brief
 */
#ifndef _AVRIO_OW_DS2438_H_
#define _AVRIO_OW_DS2438_H_

#include <avrio/defs.h>
#include <avrio/ow.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup ow_module
 * @{
 *
 * @defgroup ow_ds2438 Circuit DS2438
 * Ce module permet de gérer des circuits de surveillance batterie sur bus 1-wire.
 * @{
 * @example net/onewire/ds2438/demo_ow_ds2438.c
 * Ce programme recherche les circuits DS2438 de surveillance batterie sur
 * le bus 1-wire puis effectue la lecture des données brutes dans les registres
 * des circuits trouvés. \n
 * Si \c SERIAL_LOG_ENABLE est défini, les valeurs sont affichées sur la liaison
 * série, dans ce cas, l'implémentation choisie pour la gestion du bus 1-wire ne
 * peut pas utiliser l'UART (donc \c OW_DRIVER_POLLED_PIO est obligatoire). \n
 * Si \c FLOAT_MES_ENABLE est défini, la conversion des valeurs brutes en valeurs
 * réelles est effectuée (et leur affichage si \c SERIAL_LOG_ENABLE défini) en 
 * utilisant RSENS comme résistance de mesure de courant. \n
 * Voilà ce que cela donne pour 2 DS2438 sans convertion en valeurs réelles:
 * @code
    #,id
    0,26A2FA17020000B9
    1,26BEF517020000F0
    2 DS2438 devices found

    v0,i0,t0,f0,c0,d0,v1,i1,t1,f1,c1,d1
    420,-2,5496,0,1,15,522,-5,5600,0,0,138
    420,-1,5480,0,1,15,521,-6,5584,0,0,138
 * @endcode
 *
 * Il est prévu pour une carte mto-template, le fichier avrio-board-ow.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */

/* constants ================================================================ */
#define DS2438_FAMILY_ID   0x26 /**< identifiant de la famille */

#define DS2438_IAD  _BV(0) /**< Validation mesure courant et capacité batterie */
#define DS2438_CA   _BV(1) /**< Validation mesure capacité charge/décharge */
#define DS2438_EE   _BV(2) /**< Validation copie CCA/DCA en EEPROM */
#define DS2438_AD   _BV(3) /**< Choix entrée mesure tension (0: VAD, 1: VDD) */
#define DS2438_TB   _BV(4) /**< Flag mesure température en cours */
#define DS2438_NVB  _BV(5) /**< Flag mémoire non volatile occupée */
#define DS2438_ADB  _BV(6) /**< Flag mesure tension en cours */

#define DS2438_DEFAULT_CONFIG (DS2438_IAD|DS2438_CA|DS2438_EE|DS2438_AD) /**< configuration par défaut */

/**
 * @enum eDs2438Convertion
 */
typedef enum {
  eDs2438ConvVolt  = 1, /**< Démarrage convertion de la tension */
  eDs2438ConvTemp  = 2, /**< Démarrage convertion de la température */
  eDs2438ConvAll   = eDs2438ConvVolt | eDs2438ConvTemp /**< Démarrage des toutes les convertions */
} eDs2438Convertion;

/* structures =============================================================== */
/**
 * @brief Données instantanées brutes
 */
typedef struct xDs2438DataRaw {
  int16_t temp; /**< Registre température */
  uint16_t voltage; /**< Registre tension */
  int16_t current; /**< Registre intensité du courant */
} xDs2438DataRaw;

/**
 * @brief Données instantanées en unitées SI
 */
typedef struct xDs2438DataMes {
  double temp; /**< Température en °C */
  double voltage; /**< Tension en V */
  double current; /**< Intensité du courant en A */
} xDs2438DataMes;

/**
 * @brief Capacités cumulées brutes
 */
typedef struct xDs2438CapacityRaw {
  uint16_t ica; /**< Registre capacité restante*/
  uint16_t cca; /**< Registre capacité chargée */
  uint16_t dca; /**< Registre capacité déchargée */
} xDs2438CapacityRaw;

/**
 * @brief Capacités cumulées en unitées SI
 */
typedef struct xDs2438CapacityMes {
  double remaining; /**< Capacité restante en Ah */
  double charging; /**< Capacité chargée en Ah */
  double discharging; /**< Capacité déchargée en Ah */
} xDs2438CapacityMes;

/* internal public functions ================================================ */
/**
 * @brief Configure un DS2438
 * 
 * @param device pointeur sur le circuit
 * @param config registre de configuration
 * @return 0 succès, -1 erreur
 */
int iDs2438Configure (xOwDevice * device, uint8_t config);

/**
 * @brief Démarre une ou plusieurs mesure
 * 
 * @param device pointeur sur le circuit
 * @param conv sélection des convertions à démarrer
 * @return 0, -1 si serreur 
 */
int iDs2438StartConvert (xOwDevice *device, eDs2438Convertion conv);

/**
 * @brief Lecture des données instantanées brutes de la dernière mesure
 * 
 * Si la conversion est en cours, cette fonction attend que les mesures soient
 * disponibles.
 * @param device pointeur sur le circuit
 * @param raw pointeur sur un objet xDs2438DataRaw où seront stockées les valeurs
 * @return 0, -1 si serreur 
 */
int iDs2438DataGetRaw (xOwDevice * device, xDs2438DataRaw * raw);

/**
 * @brief Convertit les valeurs instantanées brutes en valeurs réelles
 * 
 * @param mes pointeur sur un objet xDs2438DataMes où seront stockées les valeurs réelles
 * @param rsens valeur de la résistance de mesure de courant en ohms
 * @param raw pointeur sur un objet xDs2438DataRaw où sont stockées les valeurs brutes
 */
void vDs2438DataRawToMes (xDs2438DataMes * mes, double rsens, const xDs2438DataRaw * raw);

/**
 * @brief Lecture des capacitées brutes
 * 
 * Il faut que les bits IAD et CA soient validés à la configuration.
 * 
 * @param device pointeur sur le circuit
 * @param raw pointeur sur un objet xDs2438CapacityRaw où seront stockées les valeurs
 * @return 0, -1 si serreur 
 */
int iDs2438CapacityGetRaw (xOwDevice * device, xDs2438CapacityRaw * raw);

/**
 * @brief Convertit les capacités brutes en valeurs réelles
 * 
 * @param mes pointeur sur un objet xDs2438CapacityMes où seront stockées les valeurs réelles
 * @param rsens valeur de la résistance de mesure de courant en ohms
 * @param raw pointeur sur un objet xDs2438CapacityRaw où sont stockées les valeurs brutes
 */
void vDs2438CapacityRawToMes (xDs2438CapacityMes * mes, double rsens, const xDs2438CapacityRaw * raw);

/**
 * @brief Lecture d'une page du Scratchpad
 * 
 * Un appel à \c iDs2438RecallPage() peut être nécessaire pour récupérer la
 * page en SRAM/EEPROM avant lecture du Scratchpad. \n
 * Un contrôle CRC des octets lus est effectués (retourne -1 si erreur)
 * @param device pointeur sur le circuit
 * @param page_n numéro de la page
 * @param buffer pointeur sur un buffer où seront stockés les 8 octets de la page
 * et l'octet de CRC.
 * @return 0, -1 si serreur 
 */
int iDs2438ReadPage (xOwDevice * device, uint8_t page_n, uint8_t * buffer);

/**
 * @brief Ecriture d'une page dans le Scratchpad
 * 
 * Un appel à \c iDs2438CopyPage() peut être nécessaire pour copier la
 * page en SRAM/EEPROM après écriture dans le Scratchpad. \n
 * @param device pointeur sur le circuit
 * @param page_n numéro de la page
 * @param buffer pointeur sur un buffer où sont stockés les 8 octets à écrire
 * @return 0, -1 si serreur 
 */
int iDs2438WritePage (xOwDevice * device, uint8_t page_n, const uint8_t * buffer, uint8_t len);

/**
 * @brief Copie une page du scratchpad en EEPROM/SRAM
 * 
 * @param device pointeur sur le circuit
 * @param page_n numéro de la page
 * @return 0, -1 si serreur 
 */
int iDs2438CopyPage (xOwDevice * device, uint8_t page_n);

/**
 * @brief Rappel d'une page de EEPROM/SRAM dans le scratchpad
 * 
 * @param device pointeur sur le circuit
 * @param page_n numéro de la page
 * @return 0, -1 si serreur 
 */
int iDs2438RecallPage (xOwDevice * device, uint8_t page_n);
/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_OW_DS2438_H_ */
