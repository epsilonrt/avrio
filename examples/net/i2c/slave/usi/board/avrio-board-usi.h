/*
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
 */
#ifndef _AVRIO_BOARD_USI_H_
#define _AVRIO_BOARD_USI_H_
/* ========================================================================== */

#include <avr/io.h>

#define USI_TWI_BUFFER_SIZE 32

#if   defined(__AVR_ATtiny2313__)
# define DDR_USI        DDRB
# define PORT_USI       PORTB
# define PIN_USI        PINB
# define PORT_USI_SDA     PB5
# define PORT_USI_SCL     PB7
# define PIN_USI_SDA      PINB5
# define PIN_USI_SCL      PINB7
# define USI_OVERFLOW_VECTOR  USI_OVERFLOW_vect
#endif

#if   defined(__AVR_ATtiny24__) | \
    defined(__AVR_ATtiny44__) | \
    defined(__AVR_ATtiny84__)
# define DDR_USI        DDRA
# define PORT_USI       PORTA
# define PIN_USI        PINA
# define PORT_USI_SDA     PA6
# define PORT_USI_SCL     PA4
# define PIN_USI_SDA      PINA6
# define PIN_USI_SCL      PINA4
# define USI_OVERFLOW_VECTOR  USI_OVF_vect
#endif

#if   defined(__AVR_ATtiny25__) | \
    defined(__AVR_ATtiny45__) | \
    defined(__AVR_ATtiny85__)
# define DDR_USI        DDRB
# define PORT_USI       PORTB
# define PIN_USI        PINB
# define PORT_USI_SDA     PB0
# define PORT_USI_SCL     PB2
# define PIN_USI_SDA      PINB0
# define PIN_USI_SCL      PINB2
# define USI_OVERFLOW_VECTOR  USI_OVF_vect
#endif

#if defined(__AVR_ATtiny26__)
# define DDR_USI        DDRB
# define PORT_USI       PORTB
# define PIN_USI        PINB
# define PORT_USI_SDA     PB0
# define PORT_USI_SCL     PB2
# define PIN_USI_SDA      PINB0
# define PIN_USI_SCL      PINB2
# define USI_OVERFLOW_VECTOR  USI_OVF_vect
#endif

#if   defined(__AVR_ATtiny261__) | \
    defined(__AVR_ATtiny261a__) | \
    defined(__AVR_ATtiny461__) | \
    defined(__AVR_ATtiny461a__) | \
    defined(__AVR_ATtiny861__) | \
    defined(__AVR_ATtiny861a__)
# if    defined(USI_ON_PORT_A)
#   define DDR_USI        DDRA
#   define PORT_USI       PORTA
#   define PIN_USI        PINA
#   define PORT_USI_SDA     PA0
#   define PORT_USI_SCL     PA2
#   define PIN_USI_SDA      PINA0
#   define PIN_USI_SCL      PINA2
# else
#   define DDR_USI        DDRB
#   define PORT_USI       PORTB
#   define PIN_USI        PINB
#   define PORT_USI_SDA     PB0
#   define PORT_USI_SCL     PB2
#   define PIN_USI_SDA      PINB0
#   define PIN_USI_SCL      PINB2
# endif
# define USI_OVERFLOW_VECTOR    USI_OVF_vect
#endif

#if   defined(__AVR_ATmega165__) | \
    defined(__AVR_ATmega325__) | \
    defined(__AVR_ATmega3250__) | \
    defined(__AVR_ATmega645__) | \
    defined(__AVR_ATmega6450__) | \
    defined(__AVR_ATmega329__) | \
    defined(__AVR_ATmega3290__)
# define DDR_USI        DDRE
# define PORT_USI       PORTE
# define PIN_USI        PINE
# define PORT_USI_SDA     PE5
# define PORT_USI_SCL     PE4
# define PIN_USI_SDA      PINE5
# define PIN_USI_SCL      PINE4
# define USI_OVERFLOW_VECTOR  USI_OVERFLOW_vect
#endif

#if defined(__AVR_ATmega169__)
# define DDR_USI        DDRE
# define PORT_USI       PORTE
# define PIN_USI        PINE
# define PORT_USI_SDA     PE5
# define PORT_USI_SCL     PE4
# define PIN_USI_SDA      PINE5
# define PIN_USI_SCL      PINE4
# define USI_OVERFLOW_VECTOR  USI_OVERFLOW_vect
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_USI_H_ */
