/* 
 * MODBUS Library: AVR port
 * Copyright (c) 2008 Christian Walter <cwalter@embedded-solutions.at>
 * All rights reserved.
 *
 * $Id$
 */

/* ----------------------- System includes ----------------------------------*/
#if defined(  __CODEVISIONAVR__ )
#else
#include <avr/interrupt.h>
#endif
#include <stdlib.h>

/* ----------------------- Platform includes --------------------------------*/
#include "mbport.h"

/* ----------------------- Modbus includes ----------------------------------*/

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

static UBYTE    ubNesting = 0;
static UBYTE    ubOldSREG;

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

void
vMBPAssert( void )
{
    volatile BOOL   bBreakOut = FALSE;

    vMBPEnterCritical(  );
    while( !bBreakOut );
}

void
vMBPEnterCritical( void )
{
    UBYTE           mySREG = SREG;

    cli(  );
    if( ubNesting == 0 )
    {
        ubOldSREG = mySREG;
    }
    ubNesting++;
}

void
vMBPExitCritical( void )
{
    ubNesting--;
    if( 0 == ubNesting )
    {
        if( ubOldSREG & _BV( SREG_I ) )
        {
            sei(  );
        }
    }
}
