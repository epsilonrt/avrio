/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file mbportserial_m644p.c
 * @brief MBMaster Serial Interface for ATMega32
 * Copyright _ 2010 Christian Walter <cwalter@embedded-solutions.at>
 *          All rights reserved.
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120604 - Initial version by epsilonRT from MBmaster 2.7.0
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* ----------------------- Platform includes --------------------------------*/
#include "mbport.h"
#include "mbmconfig.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "common/mbtypes.h"
#include "common/mbportlayer.h"
#include "common/mbframe.h"
#include "common/mbutils.h"

/* ----------------------- Defines ------------------------------------------*/

#define IDX_INVALID				( 255 )
#ifndef MBM_USART_BAUDRATE_MIN
# define MBM_USART_BAUDRATE_MIN		( ( F_CPU ) / ( 16UL * ( 0xFFFUL + 1UL ) ) )
#endif
#ifndef MBM_USART_BAUDRATE_MAX
# define MBM_USART_BAUDRATE_MAX		( ( F_CPU ) / ( 16UL ) )
#endif

#define MBM_USART0_IDX              ( 0 )
#define MBM_USART1_IDX              ( MBM_USART0_ENABLED * 1 + MBM_USART0_IDX )
#define UART_NUARTS            ( MBM_USART0_ENABLED + MBM_USART1_ENABLED )

#define UART_BAUD_CALC( UART_BAUD_RATE, F_OSC ) \
    ( ( F_OSC + ( ( UART_BAUD_RATE ) * 8UL ) ) / ( ( UART_BAUD_RATE ) * 16UL ) - 1 )

/* ----------------------- Defines ------------------------------------------*/
/* ----------------------- Defines (Internal - Don't change) ----------------*/
#define HDL_RESET( x ) do { \
	( x )->ubIdx = IDX_INVALID; \
	( x )->pbMBMTransmitterEmptyFN = NULL; \
	( x )->pvMBMReceiveFN = NULL; \
	( x )->xMBMHdl = MB_HDL_INVALID; \
} while( 0 );

/* ----------------------- Type definitions ---------------------------------*/
typedef struct
{
    UBYTE           ubIdx;
    pbMBPSerialTransmitterEmptyAPIV1CB pbMBMTransmitterEmptyFN;
    pvMBPSerialReceiverAPIV1CB pvMBMReceiveFN;
    xMBHandle       xMBMHdl;
} xSerialHandle;

/* ----------------------- Static variables ---------------------------------*/
STATIC xSerialHandle xSerialHdls[UART_NUARTS];
STATIC BOOL     bIsInitalized = FALSE;

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

eMBErrorCode
eMBPSerialInit( xMBPSerialHandle * pxSerialHdl, UCHAR ucPort, ULONG ulBaudRate,
                UCHAR ucDataBits, eMBSerialParity eParity, UCHAR ucStopBits, xMBHandle xMBMHdl )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    UBYTE           ubIdx;
    UCHAR           ucUCSRC = 0;
    USHORT          usUBRR;

    MBP_ENTER_CRITICAL_SECTION(  );
    if( !bIsInitalized )
    {
        for( ubIdx = 0; ubIdx < MB_UTILS_NARRSIZE( xSerialHdls ); ubIdx++ )
        {
            HDL_RESET( &xSerialHdls[ubIdx] );
        }
#if MBM_USART0_ENABLED == 1
        MBM_USART0_RS485_INIT(  );
        MBM_USART0_RS485_DISABLE_TX(  );
#endif
#if MBM_USART1_ENABLED == 1
        MBM_USART1_RS485_INIT(  );
        MBM_USART1_RS485_DISABLE_TX(  );
#endif
        bIsInitalized = TRUE;
    }

    if( NULL == pxSerialHdl )
    {
        eStatus = MB_EINVAL;
    }
    else
    {
        eStatus = MB_ENORES;
        switch ( ucPort )
        {
#if MBM_USART0_ENABLED == 1
        case MBM_USART0_IDX:
            if( IDX_INVALID == xSerialHdls[MBM_USART0_IDX].ubIdx )
            {
                *pxSerialHdl = NULL;

                if( ( ulBaudRate >= MBM_USART_BAUDRATE_MIN ) && ( ulBaudRate <= MBM_USART_BAUDRATE_MAX )
                    && ( MB_HDL_INVALID != xMBMHdl ) )
                {
                    eStatus = MB_ENOERR;
                    usUBRR = UART_BAUD_CALC( ulBaudRate, F_CPU ) & 0x0FFFU;
                    switch ( eParity )
                    {
                    case MB_PAR_EVEN:
                        ucUCSRC |= _BV( UPM01 );
                        break;
                    case MB_PAR_ODD:
                        ucUCSRC |= _BV( UPM01 ) | _BV( UPM00 );
                        break;
                    case MB_PAR_NONE:
                        break;
                    default:
                        eStatus = MB_EINVAL;
                    }

                    switch ( ucDataBits )
                    {
                    case 8:
                        ucUCSRC |= _BV( UCSZ00 ) | _BV( UCSZ01 );
                        break;
                    case 7:
                        ucUCSRC |= _BV( UCSZ01 );
                        break;
                    default:
                        eStatus = MB_EINVAL;
                    }

                    switch ( ucStopBits )
                    {
                    case 1:
                        break;
                    case 2:
                        ucUCSRC |= _BV( USBS0 );
                        break;
                    default:
                        eStatus = MB_EINVAL;
                    }

                    if( MB_ENOERR == eStatus )
                    {
                        *pxSerialHdl = &xSerialHdls[MBM_USART0_IDX];
                        xSerialHdls[MBM_USART0_IDX].ubIdx = MBM_USART0_IDX;
                        xSerialHdls[MBM_USART0_IDX].xMBMHdl = xMBMHdl;
                        UCSR0A = 0;
                        UCSR0B = 0;
                        UBRR0L = ( UBYTE ) ( usUBRR & 0xFF );
                        UBRR0H = ( UBYTE ) ( usUBRR >> 8U );

                        UCSR0C = ucUCSRC;
                    }
                }
                else
                {
                    eStatus = MB_EINVAL;
                }
            }
            else
            {
                eStatus = MB_EINVAL;
            }
            break;
#endif
#if MBM_USART1_ENABLED == 1
        case MBM_USART1_IDX:
            if( IDX_INVALID == xSerialHdls[MBM_USART1_IDX].ubIdx )
            {
                *pxSerialHdl = NULL;

                if( ( ulBaudRate >= MBM_USART_BAUDRATE_MIN ) && ( ulBaudRate <= MBM_USART_BAUDRATE_MAX )
                    && ( MB_HDL_INVALID != xMBMHdl ) )
                {
                    eStatus = MB_ENOERR;
                    usUBRR = UART_BAUD_CALC( ulBaudRate, F_CPU ) & 0x0FFFU;
                    switch ( eParity )
                    {
                    case MB_PAR_EVEN:
                        ucUCSRC |= _BV( UPM11 );
                        break;
                    case MB_PAR_ODD:
                        ucUCSRC |= _BV( UPM11 ) | _BV( UPM10 );
                        break;
                    case MB_PAR_NONE:
                        break;
                    default:
                        eStatus = MB_EINVAL;
                    }

                    switch ( ucDataBits )
                    {
                    case 8:
                        ucUCSRC |= _BV( UCSZ10 ) | _BV( UCSZ11 );
                        break;
                    case 7:
                        ucUCSRC |= _BV( UCSZ11 );
                        break;
                    default:
                        eStatus = MB_EINVAL;
                    }

                    switch ( ucStopBits )
                    {
                    case 1:
                        break;
                    case 2:
                        ucUCSRC |= _BV( USBS1 );
                        break;
                    default:
                        eStatus = MB_EINVAL;
                    }

                    if( MB_ENOERR == eStatus )
                    {
                        *pxSerialHdl = &xSerialHdls[MBM_USART1_IDX];
                        xSerialHdls[MBM_USART1_IDX].ubIdx = MBM_USART1_IDX;
                        xSerialHdls[MBM_USART1_IDX].xMBMHdl = xMBMHdl;
                        UCSR1A = 0;
                        UCSR1B = 0;
                        UBRR1L = ( UBYTE ) ( usUBRR & 0xFF );
                        UBRR1H = ( UBYTE ) ( usUBRR >> 8U );

                        UCSR1C = ucUCSRC;
                    }
                }
                else
                {
                    eStatus = MB_EINVAL;
                }
            }

#endif
        default:
            break;
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBPSerialClose( xMBPSerialHandle xSerialHdl )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    xSerialHandle  *pxSerialIntHdl = xSerialHdl;

    MBP_ENTER_CRITICAL_SECTION(  );
    if( MB_IS_VALID_HDL( pxSerialIntHdl, xSerialHdls ) )
    {
        switch ( pxSerialIntHdl->ubIdx )
        {
#if MBM_USART0_ENABLED == 1
        case MBM_USART0_IDX:
            if( ( UCSR0B & ( _BV( TXEN0 ) | _BV( RXEN0 ) ) ) != 0 )
            {
                eStatus = MB_EAGAIN;
            }
            else if( ( NULL == pxSerialIntHdl->pbMBMTransmitterEmptyFN ) && ( NULL == pxSerialIntHdl->pvMBMReceiveFN ) )
            {
                UCSR0A = 0;
                UCSR0B = 0;
                UCSR0C = 0;
                UBRR0H = 0;
                UBRR0L = 0;
                MBM_USART0_RS485_DISABLE_TX(  );
                HDL_RESET( pxSerialIntHdl );
                eStatus = MB_ENOERR;
            }
            else
            {
                eStatus = MB_EIO;
            }
            break;
#endif
#if MBM_USART1_ENABLED == 1
        case MBM_USART1_IDX:
            if( ( UCSR1B & ( _BV( TXEN1 ) | _BV( RXEN1 ) ) ) != 0 )
            {
                eStatus = MB_EAGAIN;
            }
            else if( ( NULL == pxSerialIntHdl->pbMBMTransmitterEmptyFN ) && ( NULL == pxSerialIntHdl->pvMBMReceiveFN ) )
            {
                UCSR1A = 0;
                UCSR1B = 0;
                UCSR1C = 0;
                UBRR1H = 0;
                UBRR1L = 0;
                MBM_USART0_RS485_DISABLE_TX(  );
                HDL_RESET( pxSerialIntHdl );
                eStatus = MB_ENOERR;
            }
            else
            {
                eStatus = MB_EIO;
            }
            break;
#endif
        default:
            MBP_ASSERT( 0 );
            break;
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBPSerialTxEnable( xMBPSerialHandle xSerialHdl, pbMBPSerialTransmitterEmptyCB pbMBMTransmitterEmptyFN )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    xSerialHandle  *pxSerialIntHdl = xSerialHdl;

    MBP_ENTER_CRITICAL_SECTION(  );
    if( MB_IS_VALID_HDL( pxSerialIntHdl, xSerialHdls ) )
    {
        eStatus = MB_ENOERR;
        if( NULL != pbMBMTransmitterEmptyFN )
        {
            MBP_ASSERT( NULL == pxSerialIntHdl->pbMBMTransmitterEmptyFN );
            pxSerialIntHdl->pbMBMTransmitterEmptyFN = pbMBMTransmitterEmptyFN;
            switch ( pxSerialIntHdl->ubIdx )
            {
#if MBM_USART0_ENABLED == 1
            case MBM_USART0_IDX:				
                UCSR0B |= _BV( TXEN0 ) | _BV( UDRE0 );
                MBM_USART0_RS485_ENABLE_TX(  );
                break;
#endif
#if MBM_USART1_ENABLED == 1
            case MBM_USART1_IDX:
                UCSR1B |= _BV( TXEN1 ) | _BV( UDRE1 );
                MBM_USART1_RS485_ENABLE_TX(  );
                break;
#endif
            default:
                MBP_ASSERT( 0 );
            }

        }
        else
        {
            pxSerialIntHdl->pbMBMTransmitterEmptyFN = NULL;
            /* The transmitter is disable when the last frame has been sent.
             * This is necessary for RS485 with a half-duplex bus.
             */
            switch ( pxSerialIntHdl->ubIdx )
            {
#if MBM_USART0_ENABLED == 1
            case MBM_USART0_IDX:
                UCSR0B &= ~( _BV( UDRE0 ) );
				MBM_USART0_RS485_DISABLE_TX(  );
                break;
#endif
#if MBM_USART1_ENABLED == 1
            case MBM_USART1_IDX:
                UCSR1B &= ~( _BV( UDRE1 ) );
				MBM_USART1_RS485_DISABLE_TX(  );
                break;
#endif
            default:
                MBP_ASSERT( 0 );
            }
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBPSerialRxEnable( xMBPSerialHandle xSerialHdl, pvMBPSerialReceiverCB pvMBMReceiveFN )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    xSerialHandle  *pxSerialIntHdl = xSerialHdl;

    MBP_ENTER_CRITICAL_SECTION(  );
    if( MB_IS_VALID_HDL( pxSerialIntHdl, xSerialHdls ) )
    {
        eStatus = MB_ENOERR;
        if( NULL != pvMBMReceiveFN )
        {
            MBP_ASSERT( NULL == pxSerialIntHdl->pvMBMReceiveFN );
            pxSerialIntHdl->pvMBMReceiveFN = pvMBMReceiveFN;
            switch ( pxSerialIntHdl->ubIdx )
            {
#if MBM_USART0_ENABLED == 1
            case MBM_USART0_IDX:
                UCSR0B |= _BV( RXEN0 ) | _BV( RXCIE0 );
                break;
#endif
#if MBM_USART1_ENABLED == 1
            case MBM_USART1_IDX:
                UCSR1B |= _BV( RXEN1 ) | _BV( RXCIE1 );
                break;
#endif
            default:
                MBP_ASSERT( 0 );
            }
        }
        else
        {
            pxSerialIntHdl->pvMBMReceiveFN = NULL;
            switch ( pxSerialIntHdl->ubIdx )
            {
#if MBM_USART0_ENABLED == 1
            case MBM_USART0_IDX:
                UCSR0B &= ~( _BV( RXEN0 ) | _BV( RXCIE0 ) );
                break;
#endif
#if MBM_USART1_ENABLED == 1
            case MBM_USART1_IDX:
                UCSR1B &= ~( _BV( RXEN1 ) | _BV( RXCIE1 ) );
                break;
#endif
            default:
                MBP_ASSERT( 0 );
            }
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

#if MBM_USART0_ENABLED == 1
ISR( USART0_UDRE_vect )
{
    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART0_IDX].ubIdx );
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;

    if( NULL != xSerialHdls[MBM_USART0_IDX].pbMBMTransmitterEmptyFN )
    {
        bHasMoreData = xSerialHdls[MBM_USART0_IDX].pbMBMTransmitterEmptyFN( xSerialHdls[MBM_USART0_IDX].xMBMHdl, &ubTxByte );
    }
    if( !bHasMoreData )
    {
        xSerialHdls[MBM_USART0_IDX].pbMBMTransmitterEmptyFN = NULL;
        /* The transmitter is disabled when the last frame has been sent.
         * This is necessary for RS485 with a hald-duplex bus.
         */
        UCSR0B &= ~( _BV( UDRE0 ) );
        UCSR0B |= _BV( TXCIE0 );
    }
    else
    {
		UCSR0A |= _BV( TXC0 );
        UDR0 = ubTxByte;
    }
}
#endif

#if MBM_USART0_ENABLED == 1
ISR( USART0_RX_vect )
{
    UBYTE           ubUDR = UDR0;

    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART0_IDX].ubIdx );
    if( NULL != xSerialHdls[MBM_USART0_IDX].pvMBMReceiveFN )
    {
        xSerialHdls[MBM_USART0_IDX].pvMBMReceiveFN( xSerialHdls[MBM_USART0_IDX].xMBMHdl, ubUDR );
    }
}
#endif

#if MBM_USART0_ENABLED == 1
ISR( USART0_TX_vect )
{
    MBM_USART0_RS485_DISABLE_TX(  );
    UCSR0B &= ~( _BV( TXCIE0 ) | _BV( TXEN0 ) );
}
#endif

#if MBM_USART1_ENABLED == 1
ISR( USART1_UDRE_vect )
{
    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART1_IDX].ubIdx );
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;

    if( NULL != xSerialHdls[MBM_USART1_IDX].pbMBMTransmitterEmptyFN )
    {
        bHasMoreData = xSerialHdls[MBM_USART1_IDX].pbMBMTransmitterEmptyFN( xSerialHdls[MBM_USART1_IDX].xMBMHdl, &ubTxByte );
    }
    if( !bHasMoreData )
    {
        xSerialHdls[MBM_USART1_IDX].pbMBMTransmitterEmptyFN = NULL;
        /* The transmitter is disabled when the last frame has been sent.
         * This is necessary for RS485 with a hald-duplex bus.
         */
        UCSR1B &= ~( _BV( UDRE1 ) );
        UCSR1B |= _BV( TXCIE1 );
    }
    else
    {
		UCSR1A |= _BV( TXC0 );
        UDR1 = ubTxByte;
    }
}
#endif

#if MBM_USART1_ENABLED == 1
ISR( USART1_RX_vect )
{
    UBYTE           ubUDR = UDR1;

    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART1_IDX].ubIdx );
    if( NULL != xSerialHdls[MBM_USART1_IDX].pvMBMReceiveFN )
    {
        xSerialHdls[MBM_USART1_IDX].pvMBMReceiveFN( xSerialHdls[MBM_USART1_IDX].xMBMHdl, ubUDR );
    }
}
#endif

#if MBM_USART1_ENABLED == 1
ISR( USART1_TX_vect )
{
    UCSR1B &= ~( _BV( TXCIE1 ) | _BV( TXEN1 ) );
	MBM_USART1_RS485_DISABLE_TX(  );
}
#endif
