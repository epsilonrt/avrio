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

/* LED ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
/*
 * Noms des broches
 */
#define GPIO_OUTPUTS 0xFF
#define LED1     7

/*
 * Taille du GPIO
 */
#define GPIO_SIZE  8

/*
 * Broches du GPIO
 */
#define GPIO_PINS { \
    { .port = &PORTB, { .pin = 0, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 1, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 2, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 3, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 4, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 5, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 6, .mode = eModeOutput }}, \
    { .port = &PORTB, { .pin = 7, .mode = eModeOutput }}, \
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
