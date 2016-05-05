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
 * @file mbportserial_m2561.c
 * @brief MBMaster Serial Interface for ATMega2561
 * @author  Copyright _ 2009 Christian Walter <cwalter@embedded-solutions.at>
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
    ( ( F_OSC ) / ( ( UART_BAUD_RATE ) * 16UL ) - 1 )

#define HDL_RESET( x ) do { \
	( x )->ubIdx = IDX_INVALID; \
	( x )->pbMBPTransmitterEmptyFN = NULL; \
	( x )->pvMBPReceiveFN = NULL; \
	( x )->xMBMHdl = MB_HDL_INVALID; \
} while( 0 );

#define SERIAL_DEBUG            ( 0 )
#if ( SERIAL_DEBUG == 1 )
#include <avrio/led.h>
#define SERIAL_DEBUG_INIT  do { vLedInit(); } while( 0 )
#define SERIAL_DEBUG_SET   do { vLedSet (MBM_DEBUG_LED_WORKING); } while( 0 )
#define SERIAL_DEBUG_CLEAR do { vLedClear (MBM_DEBUG_LED_WORKING); } while( 0 )
#else
#define SERIAL_DEBUG_INIT        
#define SERIAL_DEBUG_SET         
#define SERIAL_DEBUG_CLEAR       
#endif

/* ----------------------- Type definitions ---------------------------------*/
typedef struct
{
    UBYTE           ubIdx;
    pbMBPSerialTransmitterEmptyAPIV1CB pbMBPTransmitterEmptyFN;
    pvMBPSerialReceiverAPIV1CB pvMBPReceiveFN;
    xMBHandle       xMBMHdl;
    volatile uint8_t *pubUCSRXA;
    volatile uint8_t *pubUCSRXB;
    volatile uint8_t *pubUCSRXC;
    volatile uint8_t *pubUBRRXL;
    volatile uint8_t *pubUBRRXH;
} xSerialHandle;

/* ----------------------- Static variables ---------------------------------*/
STATIC xSerialHandle xSerialHdls[UART_NUARTS];
STATIC BOOL     bIsInitalized = FALSE;

/* ----------------------- Static functions ---------------------------------*/
STATIC void
vMBPSerialTransmitCtl( UBYTE ubPort, BOOL bDriverEnable )
{
    switch ( ubPort )
    {
#if ( MBM_USART0_ENABLED == 1 ) && ( MBM_USART0_ENABLED == 1 )
    case MBM_USART0_IDX:
        if( bDriverEnable )
        {
            MBM_USART0_RS485_ENABLE_TX(  );
        }
        else
        {
            MBM_USART0_RS485_DISABLE_TX( );
        }
        break;
#endif
#if ( MBM_USART1_ENABLED == 1 ) && ( MBM_USART1_ENABLED == 1 )
    case MBM_USART1_IDX:
        if( bDriverEnable )
        {
            MBM_USART1_RS485_ENABLE_TX( );
        }
        else
        {
            MBM_USART1_RS485_DISABLE_TX( );
        }
        break;
#endif
    }
}

/* ----------------------- Start implementation -----------------------------*/

eMBErrorCode
eMBPSerialInit( xMBPSerialHandle * pxSerialHdl, UCHAR ucPort, ULONG ulBaudRate,
                UCHAR ucDataBits, eMBSerialParity eParity, UCHAR ucStopBits, xMBHandle xMBMHdl )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    xSerialHandle  *pxSerialIntHdl = NULL;
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
        SERIAL_DEBUG_INIT;
        bIsInitalized = TRUE;
    }

    if( NULL == pxSerialHdl )
    {
        eStatus = MB_EINVAL;
    }
    else if( ( MBM_USART0_IDX == ucPort ) && ( IDX_INVALID == xSerialHdls[MBM_USART0_IDX].ubIdx ) )
    {
#if ( MBM_USART0_ENABLED == 1 ) && ( MBM_USART0_ENABLED == 1 )
        MBM_USART0_RS485_INIT( );
        MBM_USART0_RS485_DISABLE_TX( );
#endif
        xSerialHdls[MBM_USART0_IDX].ubIdx = MBM_USART0_IDX;
        xSerialHdls[MBM_USART0_IDX].pubUCSRXA = &UCSR0A;
        xSerialHdls[MBM_USART0_IDX].pubUCSRXB = &UCSR0B;
        xSerialHdls[MBM_USART0_IDX].pubUCSRXC = &UCSR0C;
        xSerialHdls[MBM_USART0_IDX].pubUBRRXL = &UBRR0L;
        xSerialHdls[MBM_USART0_IDX].pubUBRRXH = &UBRR0H;
        pxSerialIntHdl = &xSerialHdls[MBM_USART0_IDX];
    }
    else if( ( MBM_USART1_IDX == ucPort ) && ( IDX_INVALID == xSerialHdls[MBM_USART1_IDX].ubIdx ) )
    {
#if ( MBM_USART1_ENABLED == 1 ) && ( MBM_USART1_ENABLED == 1 )
        MBM_USART1_RS485_INIT( );
        MBM_USART1_RS485_DISABLE_TX( );
#endif
        xSerialHdls[MBM_USART1_IDX].ubIdx = MBM_USART1_IDX;
        xSerialHdls[MBM_USART1_IDX].pubUCSRXA = &UCSR1A;
        xSerialHdls[MBM_USART1_IDX].pubUCSRXB = &UCSR1B;
        xSerialHdls[MBM_USART1_IDX].pubUCSRXC = &UCSR1C;
        xSerialHdls[MBM_USART1_IDX].pubUBRRXL = &UBRR1L;
        xSerialHdls[MBM_USART1_IDX].pubUBRRXH = &UBRR1H;
        pxSerialIntHdl = &xSerialHdls[MBM_USART1_IDX];
    }
    if( NULL != pxSerialIntHdl )
    {
        if( ( ulBaudRate >= MBM_USART_BAUDRATE_MIN ) && ( ulBaudRate <= MBM_USART_BAUDRATE_MAX ) && ( MB_HDL_INVALID != xMBMHdl ) )
        {

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
                *pxSerialHdl = pxSerialIntHdl;
                pxSerialIntHdl->xMBMHdl = xMBMHdl;
                *( pxSerialIntHdl->pubUCSRXA ) = 0;
                *( pxSerialIntHdl->pubUCSRXB ) = 0;
                *( pxSerialIntHdl->pubUBRRXL ) = ( UBYTE ) ( usUBRR & 0xFF );
                *( pxSerialIntHdl->pubUBRRXH ) = ( UBYTE ) ( usUBRR >> 8U );
                *( pxSerialIntHdl->pubUCSRXC ) = ucUCSRC;
            }
        }
        else
        {
            eStatus = MB_EINVAL;
        }
    }
    else
    {
        eStatus = MB_ENORES;
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
        if( ( *( pxSerialIntHdl->pubUCSRXB ) & ( _BV( TXEN0 ) | _BV( RXEN0 ) ) ) != 0 )
        {
            eStatus = MB_EAGAIN;
        }
        else if( ( pxSerialIntHdl->pbMBPTransmitterEmptyFN == NULL ) && ( pxSerialIntHdl->pvMBPReceiveFN == NULL ) )
        {
            *( pxSerialIntHdl->pubUCSRXA ) = _BV( UDRE0 );
            *( pxSerialIntHdl->pubUCSRXB ) = 0;
            *( pxSerialIntHdl->pubUCSRXC ) = _BV( UCSZ01 ) | _BV( UCSZ00 );
            *( pxSerialIntHdl->pubUBRRXL ) = 0;
            *( pxSerialIntHdl->pubUBRRXH ) = 0;
            vMBPSerialTransmitCtl( pxSerialIntHdl->ubIdx, FALSE );
            HDL_RESET( pxSerialIntHdl );
            eStatus = MB_ENOERR;
        }
        else
        {
            eStatus = MB_EIO;
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBPSerialTxEnable( xMBPSerialHandle xSerialHdl, pbMBPSerialTransmitterEmptyCB pbMBPTransmitterEmptyFN )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    xSerialHandle  *pxSerialIntHdl = xSerialHdl;

    MBP_ENTER_CRITICAL_SECTION(  );
    if( MB_IS_VALID_HDL( pxSerialIntHdl, xSerialHdls ) )
    {
        eStatus = MB_ENOERR;
        if( NULL != pbMBPTransmitterEmptyFN )
        {
            MBP_ASSERT( NULL == pxSerialIntHdl->pbMBPTransmitterEmptyFN );
            pxSerialIntHdl->pbMBPTransmitterEmptyFN = pbMBPTransmitterEmptyFN;
            *( pxSerialIntHdl->pubUCSRXB ) |= _BV( TXEN0 ) | _BV( UDRIE0 );
            vMBPSerialTransmitCtl( pxSerialIntHdl->ubIdx, TRUE );
        }
        else
        {
            pxSerialIntHdl->pbMBPTransmitterEmptyFN = NULL;
            vMBPSerialTransmitCtl( pxSerialIntHdl->ubIdx, FALSE );
            *( pxSerialIntHdl->pubUCSRXB ) &= ~( _BV( UDRIE0 ) );
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eMBErrorCode
eMBPSerialRxEnable( xMBPSerialHandle xSerialHdl, pvMBPSerialReceiverCB pvMBPReceiveFN )
{
    eMBErrorCode    eStatus = MB_EINVAL;
    xSerialHandle  *pxSerialIntHdl = xSerialHdl;

    MBP_ENTER_CRITICAL_SECTION(  );
    if( MB_IS_VALID_HDL( pxSerialIntHdl, xSerialHdls ) )
    {
        eStatus = MB_ENOERR;
        if( NULL != pvMBPReceiveFN )
        {
            MBP_ASSERT( NULL == pxSerialIntHdl->pvMBPReceiveFN );
            pxSerialIntHdl->pvMBPReceiveFN = pvMBPReceiveFN;
            *( pxSerialIntHdl->pubUCSRXB ) |= _BV( RXEN0 ) | _BV( RXCIE0 );
        }
        else
        {
            pxSerialIntHdl->pvMBPReceiveFN = NULL;
            *( pxSerialIntHdl->pubUCSRXB ) &= ~( _BV( RXEN0 ) | _BV( RXCIE0 ) );
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

#if ( MBM_USART0_ENABLED == 1 )
ISR( USART0_UDRE_vect )
{
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;
    
    SERIAL_DEBUG_SET;
    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART0_IDX].ubIdx );
    if( NULL != xSerialHdls[MBM_USART0_IDX].pbMBPTransmitterEmptyFN )
    {
        bHasMoreData = xSerialHdls[MBM_USART0_IDX].pbMBPTransmitterEmptyFN( xSerialHdls[MBM_USART0_IDX].xMBMHdl, &ubTxByte );
    }
    if( !bHasMoreData )
    {
        xSerialHdls[MBM_USART0_IDX].pbMBPTransmitterEmptyFN = NULL;
        /* The transmitter is disabled when the last frame has been sent.
         * This is necessary for RS485 with a hald-duplex bus.
         */
        UCSR0B &= ~( _BV( UDRIE0 ) );
        UCSR0B |= _BV( TXCIE0 );
    }
    else
    {
        UCSR0A |= _BV( TXC0 );
        UDR0 = ubTxByte;
    }
    SERIAL_DEBUG_CLEAR;
}

ISR( USART0_RX_vect )
{
    UBYTE           ubUDR = UDR0;

    SERIAL_DEBUG_SET;
    MBP_ASSERT( IDX_INVALID != xSerialHdls[0].ubIdx );
    if( NULL != xSerialHdls[MBM_USART0_IDX].pvMBPReceiveFN )
    {
        xSerialHdls[MBM_USART0_IDX].pvMBPReceiveFN( xSerialHdls[MBM_USART0_IDX].xMBMHdl, ubUDR );
    }
    SERIAL_DEBUG_CLEAR;
}

ISR( USART0_TX_vect )
{
    SERIAL_DEBUG_SET;
    UCSR0B &= ~( _BV( TXCIE0 ) | _BV( TXEN0 ) );
    vMBPSerialTransmitCtl( MBM_USART0_IDX, FALSE );
    SERIAL_DEBUG_CLEAR;
}
#endif

#if ( MBM_USART1_ENABLED == 1 )
ISR( USART1_UDRE_vect )
{
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;

    SERIAL_DEBUG_SET;
    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART1_IDX].ubIdx );
    if( NULL != xSerialHdls[MBM_USART1_IDX].pbMBPTransmitterEmptyFN )
    {
        bHasMoreData = xSerialHdls[MBM_USART1_IDX].pbMBPTransmitterEmptyFN( xSerialHdls[MBM_USART1_IDX].xMBMHdl, &ubTxByte );
    }
    if( !bHasMoreData )
    {
        xSerialHdls[MBM_USART1_IDX].pbMBPTransmitterEmptyFN = NULL;
        /* The transmitter is disabled when the last frame has been sent.
         * This is necessary for RS485 with a hald-duplex bus.
         */
        UCSR1B &= ~( _BV( UDRIE0 ) );
        UCSR1B |= _BV( TXCIE0 );
    }
    else
    {
        UCSR1A |= _BV( TXC0 );
        UDR1 = ubTxByte;
    }
    SERIAL_DEBUG_CLEAR;
}

ISR( USART1_RX_vect )
{
    UBYTE           ubUDR = UDR1;

    SERIAL_DEBUG_SET;
    MBP_ASSERT( IDX_INVALID != xSerialHdls[1].ubIdx );
    if( NULL != xSerialHdls[MBM_USART1_IDX].pvMBPReceiveFN )
    {
        xSerialHdls[MBM_USART1_IDX].pvMBPReceiveFN( xSerialHdls[MBM_USART1_IDX].xMBMHdl, ubUDR );
    }
    SERIAL_DEBUG_CLEAR;
}

ISR( USART1_TX_vect )
{
    SERIAL_DEBUG_SET;
    UCSR1B &= ~( _BV( TXCIE0 ) | _BV( TXEN0 ) );
    vMBPSerialTransmitCtl( MBM_USART1_IDX, FALSE );
    SERIAL_DEBUG_CLEAR;
}
#endif
