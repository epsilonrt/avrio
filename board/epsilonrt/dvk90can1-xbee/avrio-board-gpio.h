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
#ifndef _AVRIO_BOARD_GPIO_H_
#define _AVRIO_BOARD_GPIO_H_
/* ========================================================================== */

/* LED ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
/*
 * Noms des broches
 */
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7
#define BUTC 8
#define BUTN 9
#define BUTE 10
#define BUTW 11
#define BUTS 12
/*
 * Taille du GPIO
 */
#define GPIO_SIZE  13

/*
 * Broches du GPIO
 */
#define GPIO_PINS { \
    { .port = &PORTA, { .pin = 0, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 1, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 2, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 3, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 4, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 5, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 6, .mode = eModeOutput }}, \
    { .port = &PORTA, { .pin = 7, .mode = eModeOutput }}, \
    { .port = &PORTE, { .pin = 2, .mode = eModeInputPullUp }}, \
    { .port = &PORTE, { .pin = 4, .mode = eModeInputPullUp }}, \
    { .port = &PORTE, { .pin = 5, .mode = eModeInputPullUp }}, \
    { .port = &PORTE, { .pin = 6, .mode = eModeInputPullUp }}, \
    { .port = &PORTE, { .pin = 7, .mode = eModeInputPullUp }}, \
}

/*
 * Délai de l'anti-rebond en ms
 */
#define GPIO_POLL_DELAY 20

/* ========================================================================== */
#endif /* _AVRIO_BOARD_GPIO_H_ */
