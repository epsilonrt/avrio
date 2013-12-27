/**
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_SERIAL_PRIVATE_H_
#  define _AVRIO_SERIAL_PRIVATE_H_
/* ========================================================================== */
#  include "avrio-config.h"

#  ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#    include "avrio-board-serial.h"

/* macros =================================================================== */
#    define SERIAL_BAUD_X1(usBaud) (AVRIO_CPU_FREQ / (1600UL * usBaud) - 1)
#    define SERIAL_BAUD_X2(usBaud) (AVRIO_CPU_FREQ / (800UL * usBaud) - 1)

/* USART Control and Status Register A */
#    ifndef RXC
#      define    RXC          7
#      define    TXC          6
#      define    UDRE         5
#      define    FE           4
#      define    DOR          3
#      define    PE           2
#      define    U2X          1

#    elif ! defined(PE)
#      define    PE           2
#    endif

/* USART Control and Status Register B */
#    ifndef RXCIE
#      define    RXCIE        7
#      define    TXCIE        6
#      define    UDRIE        5
#      define    RXEN         4
#      define    TXEN         3
#      define    UCSZ2        2
#    endif

/* USART Control and Status Register C */
#    ifndef UPM1
#      define    UPM1         5
#      define    UPM0         4
#      define    USBS         3
#      define    UCSZ1        2
#      define    UCSZ0        1
#    endif

#    ifdef SERIAL_TXEN_ENABLE
#      define TXEN_INIT() do {\
    cbi (SERIAL_TXEN_PORT, SERIAL_TXEN_BIT);\
    sbi (SERIAL_TXEN_DDR, SERIAL_TXEN_BIT);\
  }while(0)
#      define TXEN_SET() do {\
    sbi (SERIAL_TXEN_PORT, SERIAL_TXEN_BIT);\
  } while(0)
#      define TXEN_CLR() do {\
    cbi (SERIAL_TXEN_PORT, SERIAL_TXEN_BIT);\
  } while(0)
#    else
#      define TXEN_INIT()
#      define TXEN_SET()
#      define TXEN_CLR()
#    endif

#    ifdef URSEL
#      define UCSRC_SEL _BV(URSEL)
#    else
#      define UCSRC_SEL 0
#    endif

#  endif
       /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
#endif /* _AVRIO_SERIAL_PRIVATE_H_ */
