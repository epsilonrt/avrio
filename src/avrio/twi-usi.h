/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file twi-usi.h
 * @brief Bus I2C avec coupleur USI
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120427 - Initial version by epsilonRT from AVR312 1.14 Revision
 */
#include <avrio/defs.h>
#include <avr/interrupt.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup twi_group
 * @{
 *
 *  @defgroup twi_usi_module Bus I2C avec coupleur USI
 *
 *  Ce module permet de gérer un bus I2C avec un coupleur de type USI.
 *  @{
 */

/**
 * @brief Fonction de gestion d'un événement Esclave
 */
typedef void (*vTwiUsiSlaveHandler) (void);

/* internal public functions ================================================ */
/**
 * @brief Initialise l'interface USI en mode I2C Esclave
 *
 * Les interruptions doivent être validées pour que le module fonctionne.
 * @param Adresse I2C à laquelle l'esclave doit répondre
 */
void vTwiUsiSlaveInit (uint8_t ucOwnAddress);

/**
 * @brief Ecrit un octet dans le buffer de transmission
 *
 * L'octet sera transmis au maître lors de son prochaine accès en lecture. Cette
 * fonction bloque en attente d'espace libre si le buffer est plein.
 * @param ucData Octet à écrire
 */
void vTwiUsiSlaveWrite (uint8_t ucData);

/**
 * @brief Lit un octet dans le buffer de réception
 *
 * Cette fonction bloque en attente d'un octet transmis par le maître. Il est
 * possible d'utiliser xTwiUsiSlaveCharIsReceived() pour éviter ce blocage.
 * @return Octet lu
 */
uint8_t ucTwiUsiSlaveRead (void);

/**
 * @brief Renvoie true si le buffer de réception contient des données.
 */
bool xTwiUsiSlaveCharIsReceived (void);

/**
 * @brief Renvoie true si le buffer de transmission est vide.
 */
bool xTwiUsiSlaveTxBufferIsEmpty (void);

/**
 * @brief Enregistre la fonction de gestion de transmission utilisateur
 *
 * Le gestionnaire est appelé par la routine d'interruption du module USI
 * esclave lorsqu'elle reçoit une demande de lecture et que le buffer de
 * transmission est vide. Il faut donc écrire un octet dans le buffer de
 * transmission avec vTwiUsiSlaveWrite(). \n
 * @warning Le fonction de gestion est exécutée sous interruption, il faut
 * donc être très bref !
 * @param pxUxerTxHandler Pointeur sur la fonction ou 0 pour retirer la fonction.
 */
void vTwiUsiSlaveRegisterTxHandler (vTwiUsiSlaveHandler pxUxerTxHandler);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
