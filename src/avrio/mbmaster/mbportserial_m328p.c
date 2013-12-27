/**
 * @file mbportserial_m328p.c
 * @brief MBMaster Serial Interface for ATMega32
 * @author Copyright _ 2008 Christian Walter <cwalter@embedded-solutions.at>
 *          All rights reserved.
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
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

#define UART_BAUD_CALC( UART_BAUD_RATE, F_OSC ) \
    ( ( F_OSC ) / ( ( UART_BAUD_RATE ) * 16UL ) - 1 )

#define HDL_RESET( x ) do { \
	( x )->ubIdx = IDX_INVALID; \
	( x )->pbMBPTransmitterEmptyFN = NULL; \
	( x )->pvMBPReceiveFN = NULL; \
	( x )->xMBMHdl = MB_HDL_INVALID; \
} while( 0 );

/* ----------------------- Type definitions ---------------------------------*/
typedef struct
{
    UBYTE           ubIdx;
    pbMBPSerialTransmitterEmptyAPIV1CB pbMBPTransmitterEmptyFN;
    pvMBPSerialReceiverAPIV1CB pvMBPReceiveFN;
    xMBHandle       xMBMHdl;
} xSerialHandle;

/* ----------------------- Static variables ---------------------------------*/
STATIC xSerialHandle xSerialHdls[1];
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
        MBM_USART0_RS485_INIT(  );
        MBM_USART0_RS485_DISABLE_TX(  );
        bIsInitalized = TRUE;
    }

    if( NULL == pxSerialHdl )
    {
        eStatus = MB_EINVAL;
    }
    else if( ( 0 == ucPort ) && ( IDX_INVALID == xSerialHdls[0].ubIdx ) )
    {
        *pxSerialHdl = NULL;

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
                *pxSerialHdl = &xSerialHdls[0];
                xSerialHdls[0].ubIdx = 0;
                xSerialHdls[0].xMBMHdl = xMBMHdl;
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
        /* Reset to default values (See ATMega168 datasheet ) */
        UCSR0A = _BV( UDRE0 );
        UCSR0B = 0;
        UCSR0C = _BV( UCSZ01 ) | _BV( UCSZ00 );
        UBRR0H = 0;
        UBRR0L = 0;
        MBM_USART0_RS485_DISABLE_TX(  );
        HDL_RESET( pxSerialIntHdl );
        eStatus = MB_ENOERR;
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
            UCSR0B |= _BV( TXEN0 ) | _BV( UDRIE0 );
            MBM_USART0_RS485_ENABLE_TX(  );
        }
        else
        {
            pxSerialIntHdl->pbMBPTransmitterEmptyFN = NULL;
            MBM_USART0_RS485_DISABLE_TX(  );
            UCSR0B &= ~( _BV( UDRIE0 ) );
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
            UCSR0B |= _BV( RXEN0 ) | _BV( RXCIE0 );
        }
        else
        {
            pxSerialIntHdl->pvMBPReceiveFN = NULL;
            UCSR0B &= ~( _BV( RXEN0 ) | _BV( RXCIE0 ) );
        }
    }
    MBP_EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

ISR( USART_UDRE_vect )
{
    MBP_ASSERT( IDX_INVALID != xSerialHdls[0].ubIdx );
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;

    if( NULL != xSerialHdls[0].pbMBPTransmitterEmptyFN )
    {
        bHasMoreData = xSerialHdls[0].pbMBPTransmitterEmptyFN( xSerialHdls[0].xMBMHdl, &ubTxByte );
    }
    if( !bHasMoreData )
    {
        xSerialHdls[0].pbMBPTransmitterEmptyFN = NULL;
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
}

ISR( USART_RX_vect )
{
    UBYTE           ubUDR = UDR0;

    MBP_ASSERT( IDX_INVALID != xSerialHdls[0].ubIdx );
    if( NULL != xSerialHdls[0].pvMBPReceiveFN )
    {
        xSerialHdls[0].pvMBPReceiveFN( xSerialHdls[0].xMBMHdl, ubUDR );
    }
}

ISR( USART_TX_vect )
{
    MBM_USART0_RS485_DISABLE_TX(  );
    UCSR0B &= ~( _BV( TXCIE0 ) | _BV( TXEN0 ) );
}
