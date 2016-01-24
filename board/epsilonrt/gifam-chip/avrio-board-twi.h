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


/*-----------------------------Mode Maître------------------------------------*/
/* constants ================================================================ */
/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_RXBUFSIZE
 * @brief Taille du buffer de réception en octets.
 */
#  define TWI_MASTER_RXBUFSIZE 16

/*
 * @ingroup twimasterboard_module
 * @def TWI_MASTER_TXBUFSIZE
 * @brief Taille du buffer de transmission en octets.
 */
#  define TWI_MASTER_TXBUFSIZE 16


/*----------------------------Mode Esclave------------------------------------*/
/* constants ================================================================ */
/*
 * @ingroup twislaveboard_module
 * @def TWI_SLAVE_RXBUFSIZE
 * @brief Taille du buffer de réception en octets.
 * 1,2,4,8,16,32,64,128 or 256 bytes are allowed buffer sizes
 */
#  define TWI_SLAVE_RXBUFSIZE 16

/*
 * @ingroup twislaveboard_module
 * @def TWI_SLAVE_TXBUFSIZE
 * @brief Taille du buffer de transmission en octets.
 * 1,2,4,8,16,32,64,128 or 256 bytes are allowed buffer sizes
 */
#  define TWI_SLAVE_TXBUFSIZE 16

/* macros =================================================================== */
#if defined(__AVR_ATtiny24__ ) | defined(__AVR_ATtiny44__ ) | defined(__AVR_ATtiny84__ )
    #define USI_DDR             DDRA
    #define USI_PORT            PORTA
    #define USI_PIN             PINA
    #define USI_SDA             6
    #define USI_SCL             4
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_STR_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect 
#endif

#if defined(__AVR_ATtiny25__ ) | defined(__AVR_ATtiny45__ ) | defined(__AVR_ATtiny85__ )
    #define USI_DDR             DDRB
    #define USI_PORT            PORTB
    #define USI_PIN             PINB
    #define USI_SDA             0
    #define USI_SCL             2
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_START_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect 
#endif

#if defined(__AVR_ATtiny26__)
    #define USI_DDR             DDRB
    #define USI_PORT            PORTB
    #define USI_PIN             PINB
    #define USI_SDA             0
    #define USI_SCL             2
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_STRT_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined(__AVR_ATtiny2313__ ) | defined(__AVR_ATtiny2313A__)
    #define USI_DDR             DDRB
    #define USI_PORT            PORTB
    #define USI_PIN             PINB
    #define USI_SDA             5
    #define USI_SCL             7
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_START_vect
    #define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif

#if defined(__AT90Mega165__) | defined(__ATmega165__) | \
    defined(__ATmega325__) | defined(__ATmega3250__) | \
    defined(__ATmega645__) | defined(__ATmega6450__) | \
    defined(__ATmega329__) | defined(__ATmega3290__) | \
    defined(__ATmega649__) | defined(__ATmega6490__)
    #define USI_DDR             DDRE
    #define USI_PORT            PORTE
    #define USI_PIN             PINE
    #define USI_SDA             5
    #define USI_SCL             4
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_START_vect 
    #define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif

#if defined(__AT90Mega169__) | defined(__ATmega169__)
    #define USI_DDR             DDRE
    #define USI_PORT            PORTE
    #define USI_PIN             PINE
    #define USI_SDA             5
    #define USI_SCL             4
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_START_vect 
    #define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect 
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_TWI_H_ */
