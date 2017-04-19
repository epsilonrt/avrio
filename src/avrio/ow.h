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
#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup ow_module Bus 1-wire
 * 
 * Ce module permet de gérer le bus 1-wire. Il est possible de choisir
 * l'implémentation en ajoutant une des macros suivantes à AVRIO_CONFIG :
 * 
 * - \c OW_DRIVER_POLLED_PIO implémentation en scrutation utilisant une simple
 * broche de port. La broche utilisée sera définie dans les fonctions
 * \c vOwBusPullLow(), \c vOwBusRelease() et \c bOwBusRead() dans le fichier
 * avrio-board-ow.h. Aucun ajout de composant n'est nécessaire.
 * - \c OW_DRIVER_POLLED_UART implémentation en scrutation utilisant l'UART.
 * Dans ce cas, un montage d'adaptation est nécessaire afin que la broche
 * TXD puisse gérer la liaison DQ en collecteur ouvert (une simple diode suffit)
 * .
 * 
 *  @{
 * @example net/onewire/scan/demo_ow_scan.c
 * Ce programme recherche les circuits sur le bus 1-wire.
 * Si \c SERIAL_LOG_ENABLE est défini, les identifiants des circuits sont 
 * affichés sur la liaison série, dans ce cas, l'implémentation choisie pour 
 * la gestion du bus 1-wire ne peut pas utiliser l'UART (donc 
 * \c OW_DRIVER_POLLED_PIO est obligatoire). \n
 * Voilà ce que cela donne si 2 circuits DS2438 sont détectés:
 * @code
 
    2 devices found
    #,id
    0,26A2FA17020000B9
    1,26BEF517020000F0
 * @endcode
 *
 * Il est prévu pour une carte mto-template, le fichier avrio-board-ow.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */

/* structures =============================================================== */
/**
 * @brief Circuit sur le bus 1wire
 */
typedef struct {
  uint8_t id[8];  /**< Identifiant 64 bits, octet de poids faible en premier */
} xOwDevice;

/* internal public functions ================================================ */
/**
 * @brief Initialisation du module 1wire
 */
void vOwInit(void);

/**
 * @brief Recherche des circuits sur le bus
 * 
 * @param devices pointeur sur un tableau qui contiendra les circuits trouvés
 * @param len nombre de circuits pouvant être stockés dans devices
 * @return le nombre de circuit trouvés sur le bus, -1 si erreur
 */
int8_t iOwSearchDevices (xOwDevice * devices, uint8_t len);

/**
 * @brief Sélection d'un circuit sur le bus
 * 
 * @param device circuit à sélectionner
 */
void vOwMatch (const xOwDevice * device);

/**
 * @brief Accès sans identification
 * 
 * Cette commande permet de gagner du temps mais ne peut être utilisée que si
 * un seul circuit est présent sur le bus 1wire
 */
void vOwSkip (void);

/**
 * @brief Lecture de l'identifiant d'un circuit unique
 * 
 * Cette commande ne peut être utilisée que si un seul circuit est présent sur 
 * le bus 1wire.
 * 
 * @param device pointeur sur un objet circuit où sera stocké l'identifiant
 */
void vOwReadRomId (xOwDevice * device);

/**
 * @brief Ecriture d'un octet sur le bus
 * 
 * Le sélection du circuit doit être effectuée préalablement à l'aide de \c
 * vOwMatch() ou \c vOwSkip()
 * 
 * @param data octet
 */
void vOwWrite (uint8_t data);

/**
 * @brief Lecture d'un octet sur le bus
 * 
 * Le sélection du circuit doit être effectuée préalablement à l'aide de \c
 * vOwMatch() ou \c vOwSkip()
 * @return l'octet
 */
uint8_t ucOwRead (void);

/**
 * @brief Génère un reset sur le bus et vérifie la présence de circuits
 * @return true si des circuits sont présents
 */
bool bOwReset(void);

/**
 * @brief Calcul et vérification du CRC8 d'une suite d'octets
 * 
 * @param buffer pointeur sur les données à vérifier, le dernier octet doit 
 * contenir le CRC8 reçu
 * @param len nombre d'octets pointés par buffer y compris le CRC final
 * @return 0 succès, -1 erreur
 */
int8_t iOwCheckCRC8(uint8_t * buffer, uint8_t len);

/**
 * @brief Calcul du CRC8 
 * 
 * Pour calculer le CRC8 d'une suite d'octets, il suffit d'appeller cette 
 * fonction pour chaque octets, seed devra être 0 au premier appel puis la 
 * valeur du calcul précédent pour les appels suivants.
 * 
 * @param data octet de donnée
 * @param seed valeur de départ
 * @return CRC8 calculé
 */
uint8_t ucOwComputeCRC8(uint8_t data, uint8_t seed);

// -----------------------------------------------------------------------------
#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Teste si un circuit appartient à une famille
 * 
 * @param d pointeur sur le circuit
 * @param family octet identifiant la famille (LSB d'un identifiant 64 bits)
 * @return true si vrai
 */
static inline bool bOwIsPartOfFamily (const xOwDevice * d, uint8_t family);
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
INLINE bool
bOwIsPartOfFamily (const xOwDevice * d, uint8_t family) {
  return d->id[0] == family;
}
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
