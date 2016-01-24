/*
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
 */
#ifndef _AVRIO_BOARD_TWI_H_
#  define _AVRIO_BOARD_TWI_H_
/* ========================================================================== */

/* TWI ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
/*
 * @ingroup twiboard_module
 * @def TWI_MASTER_ENABLE
 * @brief Valide le module \ref twimaster_module.
 */
// #define TWI_MASTER_ENABLE

/*
 * @ingroup twiboard_module
 * @def TWI_SLAVE_ENABLE
 * @brief Valide le module \ref twislave_module.
 */
// #define TWI_SLAVE_ENABLE



/* inline public functions ================================================== */
/*
 * @ingroup twiboard_module
 * @brief Valide les r�sistances de tirage � l'�tat haut des lignes SDA et SCL
 *
 * Doit �tre r�impl�ment�e par l'utilisateur dans le cas d'une carte
 * personnalis�e.
 */
static inline void
vTwiEnablePullup (void) {

}

/*-----------------------------Mode Ma�tre------------------------------------*/
/* constants ================================================================ */
/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_RXBUFSIZE
 * @brief Taille du buffer de r�ception en octets.
 */
#  define TWI_MASTER_RXBUFSIZE 32

/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_TXBUFSIZE
 * @brief Taille du buffer de transmission en octets.
 */
#  define TWI_MASTER_TXBUFSIZE 32

/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_CTRLBUFSIZE
 * @brief Taille du buffer de contr�le en octets.
 *
 * Le buffer de contr�le permet de transmettre les messages de contr�le � la
 * routine d'interruption du module.\n
 * Chaque message de contr�le est constitu� d'un identifiant de trame
 * (xTwiId), d'une adresse circuit (xTwiDeviceAddr) et d'un nombre d'octets
 * (xTwiLength).
 */
#  define TWI_MASTER_CTRLBUFSIZE 15

/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_STATUSBUFSIZE
 * @brief Taille du buffer d'�tat en octets.
 *
 * Le buffer d'�tat permet de r�cup�rer les messages d'�tat �mis par la
 * routine d'interruption du module.\n
 * Chaque message d'�tat est constitu� d'un identifiant de trame
 * (\ref xTwiId), d'un �tat (xTwiStatus) et d'un nombre d'octets (xTwiLength).
 */
#  define TWI_MASTER_STATUSBUFSIZE 15

/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_ADDR_NACK_RETRY
 * @brief Nombre d'essais avant de d�clarer un esclave absent du bus.
 *
 *  Permet en mode ma�tre, de renvoyer un certain nombre de demandes � un
 *  esclave qui n'a pas r�pondu la premi�re fois (esclave lent � la d�tente !).
 */
#  define TWI_MASTER_ADDR_NACK_RETRY 1

/*----------------------------Mode Esclave------------------------------------*/
/* constants ================================================================ */
/*
 * @ingroup twislaveboard_module
 * @def TWI_SLAVE_RXBUFSIZE
 * @brief Taille du buffer de r�ception en octets.
 */
#  define TWI_SLAVE_RXBUFSIZE 32

/*
 * @ingroup twislaveboard_module
 * @def TWI_SLAVE_TXBUFSIZE
 * @brief Taille du buffer de transmission en octets.
 */
#  define TWI_SLAVE_TXBUFSIZE 32

/* ========================================================================== */
#endif /* _AVRIO_BOARD_TWI_H_ */
