/*
 * Copyright © 2016 epsilonRT, All rights reserved.
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

/* GPIO ===================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
/*
 * Taille du GPIO
 */
#define GPIO_SIZE  14

/*
 * Broches du GPIO
 */
#define GPIO_PINS { \
    { .port = &PORTD, { .pin = 0, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 1, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 2, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 3, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 4, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 5, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 6, .mode = eModeInput }}, \
    { .port = &PORTD, { .pin = 7, .mode = eModeInput }}, \
    { .port = &PORTB, { .pin = 0, .mode = eModeInput }}, \
    { .port = &PORTB, { .pin = 1, .mode = eModeInput }}, \
    { .port = &PORTB, { .pin = 2, .mode = eModeInput }}, \
    { .port = &PORTB, { .pin = 3, .mode = eModeInput }}, \
    { .port = &PORTB, { .pin = 4, .mode = eModeInput }}, \
    { .port = &PORTB, { .pin = 5, .mode = eModeInput }}, \
}

/*
 * Délai entre 2 scrutations
 * Périoide d'éxecution de la tâche chargée de surveiller les changements
 * d'état. La tâche est exécutée sous interruption.
 */
#define GPIO_POLL_DELAY 20

/*
 * Taille de la pile de messages
 * La fonction vGpioPoll() utilise une tâche qui surveille les changements
 * d'état. Cette tâche transmet les changements d'état à vGpioPoll() par une
 * pile de messages (un octet par changement d'état).
 */
#define GPIO_FIFO_SIZE 8

/* ========================================================================== */
#endif /* _AVRIO_BOARD_GPIO_H_ */
