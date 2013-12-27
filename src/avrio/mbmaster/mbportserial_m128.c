/**
 * @file mbportserial_m128.c
 * @brief MBMaster Serial Interface for ATMega128
 * @author Copyright _ 2010 Christian Walter <cwalter@embedded-solutions.at>
 *          All rights reserved.
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120604 - Initial version by epsilonRT from MBmaster 2.7.0
 */

/* ----------------------- System includes ----------------------------------*/
#if defined( __IMAGECRAFT__  )
#include <iccioavr.h>
#elif defined(  __CODEVISIONAVR__ )
#else
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#include <stdlib.h>

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
#define UART_NUARTS             ( MBM_USART0_ENABLED + MBM_USART1_ENABLED )

#define UART_BAUD_CALC( UART_BAUD_RATE, F_OSC ) \
    ( ( F_OSC ) / ( ( UART_BAUD_RATE ) * 16UL ) - 1 )

/* ----------------------- Defines ------------------------------------------*/
/* ----------------------- Defines (Internal - Don't change) ----------------*/
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
            else if( ( NULL == pxSerialIntHdl->pbMBPTransmitterEmptyFN ) && ( NULL == pxSerialIntHdl->pvMBPReceiveFN ) )
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
            else if( ( NULL == pxSerialIntHdl->pbMBPTransmitterEmptyFN ) && ( NULL == pxSerialIntHdl->pvMBPReceiveFN ) )
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
            switch ( pxSerialIntHdl->ubIdx )
            {
#if MBM_USART0_ENABLED == 1
            case MBM_USART0_IDX:				
				UCSR0A |= _BV( TXC0 );
                UCSR0B |= _BV( TXEN0 ) | _BV( UDRE0 );
                MBM_USART0_RS485_ENABLE_TX(  );
                break;
#endif
#if MBM_USART1_ENABLED == 1
            case MBM_USART1_IDX:
				UCSR1A |= _BV( TXC0 );
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
            pxSerialIntHdl->pbMBPTransmitterEmptyFN = NULL;
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
            pxSerialIntHdl->pvMBPReceiveFN = NULL;
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
#if defined( __IMAGECRAFT__  )
#pragma interrupt_handler vUART0Data:iv_USART0_UDRE 
void vUART0Data( void )
#elif defined(  __CODEVISIONAVR__ )
interrupt[USART0_DRE] void vUART0Data( void ) 
#else
ISR( USART0_UDRE_vect )
#endif
{
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;
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
#if defined( __IMAGECRAFT__  )
#pragma interrupt_handler vUART0Recv:iv_USART0_RX 
void vUART0Recv( void )
#elif defined(  __CODEVISIONAVR__ )
interrupt[USART0_RXC] void vUART0Recv( void ) 
#else
ISR( USART0_RX_vect )
#endif
{
    UBYTE           ubUDR = UDR0;

    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART0_IDX].ubIdx );
    if( NULL != xSerialHdls[MBM_USART0_IDX].pvMBPReceiveFN )
    {
        xSerialHdls[MBM_USART0_IDX].pvMBPReceiveFN( xSerialHdls[MBM_USART0_IDX].xMBMHdl, ubUDR );
    }
}
#endif

#if MBM_USART0_ENABLED == 1
#if defined( __IMAGECRAFT__  )
#pragma interrupt_handler vUART0Trans:iv_USART0_TX 
void vUART0Trans( void )
#elif defined(  __CODEVISIONAVR__ )
interrupt[USART0_TXC] void vUART0Trans( void ) 
#else
ISR( USART0_TX_vect )
#endif
{
    MBM_USART0_RS485_DISABLE_TX(  );
    UCSR0B &= ~( _BV( TXCIE0 ) | _BV( TXEN0 ) );
}
#endif

#if MBM_USART1_ENABLED == 1
#if defined( __IMAGECRAFT__  )
#pragma interrupt_handler vUART0Data:iv_USART1_UDRE 
void vUART1Data( void )
#elif defined(  __CODEVISIONAVR__ )
interrupt[USART1_DRE] void vUART1Data( void ) 
#else
ISR( USART1_UDRE_vect )
#endif
{
    BOOL            bHasMoreData = TRUE;
    UBYTE           ubTxByte;
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
#if defined( __IMAGECRAFT__  )
#pragma interrupt_handler vUART1Recv:iv_USART1_RX 
void vUART1Recv( void )
#elif defined(  __CODEVISIONAVR__ )
interrupt[USART1_RXC] void vUART1Recv( void ) 
#else
ISR( USART1_RX_vect )
#endif
{
    UBYTE           ubUDR = UDR1;

    MBP_ASSERT( IDX_INVALID != xSerialHdls[MBM_USART1_IDX].ubIdx );
    if( NULL != xSerialHdls[MBM_USART1_IDX].pvMBPReceiveFN )
    {
        xSerialHdls[MBM_USART1_IDX].pvMBPReceiveFN( xSerialHdls[MBM_USART1_IDX].xMBMHdl, ubUDR );
    }
}
#endif

#if MBM_USART1_ENABLED == 1
#if defined( __IMAGECRAFT__  )
#pragma interrupt_handler vUART0Trans:iv_USART1_TX 
void vUART1Trans( void )
#elif defined(  __CODEVISIONAVR__ )
interrupt[USART1_TXC] void vUART1Trans( void ) 
#else
ISR( USART1_TX_vect )
#endif
{
  UCSR1B &= ~( _BV( TXCIE1 ) | _BV( TXEN1 ) );
	MBM_USART1_RS485_DISABLE_TX(  );
}
#endif
