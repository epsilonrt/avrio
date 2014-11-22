/**
 * @file serial_sw.c
 * @ingroup serial_module
 * @brief Liaison série asynchrone logicielle
 *
 * @author Atmel Corporation: http://www.atmel.com (AVR274 application note)
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_SW_ENABLE
/* ========================================================================== */
#include "avrio-board-serial-sw.h"
#include "serial_sw.h"
#include <util/atomic.h>
#include <avrio/queue.h>

/* constants ================================================================ */
/*
 * Cycles from the start bit is sent (from UART_transmit) to the timer is
 * started plus cycles in the timer interrupt before first data bit is sent.
 * TODO:  A priori, il n'est pas nécessaire de modifier cette valeur en fonction
 *        de la vitesse ?
 */
#define TRANSMIT_DELAY        70
/*
 * Cycles from the start bit is detected to the timer is started plus cycles in
 * timer interrupt before first data bit is received.
 * TODO:  A priori, il n'est pas nécessaire de modifier cette valeur en fonction
 *        de la vitesse ?
 */
#define RECEIVE_DELAY         76

# define EOL_CR
//# define EOL_LF
//# define EOL_CRLF

/* Counter values */
#define UART_STATE_IDLE       0

// Réception impaire
#define RECEIVE_FIRST_DATA    3
#define RECEIVE_LAST_DATA     17
#define RECEIVE_STOP_1        19
#define RECEIVE_FINISH        23

// Transmission paire
#define TRANSMIT_FIRST_DATA   2
#define TRANSMIT_LAST_DATA    16
#define TRANSMIT_STOP_1       18
#define TRANSMIT_STOP_2       20
#define TRANSMIT_FINISH       22

// Conséquence, il n'est pas possible de faire du full duplex,
/* macros =================================================================== */
#define OCR(b,n) ((F_CPU / (b) / (n)) - 1)

/* Flag macros. */
//< Use this macro when reading a flag in a register.
#define READ_FLAG(flag_register,flag_bit)   ( (flag_register) & (1 << (flag_bit)) )


/* setup timing ============================================================= */
#if (OCR(AVRIO_SERIAL_SW_BAUD,1) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS1)
#define PRESCALER 1
#define TIMER_CS SERIAL_SW_CS1
#elif (OCR(AVRIO_SERIAL_SW_BAUD,8) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS8)
#define PRESCALER 8
#define TIMER_CS SERIAL_SW_CS8
#elif (OCR(AVRIO_SERIAL_SW_BAUD,32) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS32)
#define PRESCALER 32
#define TIMER_CS SERIAL_SW_CS32
#elif (OCR(AVRIO_SERIAL_SW_BAUD,64) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS64)
#define PRESCALER 64
#define TIMER_CS SERIAL_SW_CS64
#elif (OCR(AVRIO_SERIAL_SW_BAUD,128) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS128)
#define PRESCALER 128
#define TIMER_CS SERIAL_SW_CS128
#elif (OCR(AVRIO_SERIAL_SW_BAUD,256) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS256)
#define PRESCALER 256
#define TIMER_CS SERIAL_SW_CS256
#elif (OCR(AVRIO_SERIAL_SW_BAUD,1024) <= SERIAL_SW_TIMER_MAX) && defined(SERIAL_SW_CS1024)
#define PRESCALER 1024
#define TIMER_CS SERIAL_SW_CS1024
#else
#error Too slow baudrate
#endif

#define WAIT_ONE        OCR(AVRIO_SERIAL_SW_BAUD,PRESCALER)
#define WAIT_ONEHALF   (WAIT_ONE + WAIT_ONE/2)
#define START_TRANSMIT (WAIT_ONE - (TRANSMIT_DELAY/PRESCALER))    //< Sets the timer compare register to the correct value when a transmission is started.
#define START_RECEIVE  (WAIT_ONEHALF - (RECEIVE_DELAY/PRESCALER)) //< Sets the timer compare register to the correct value when a reception is started.

#if ( (START_RECEIVE > SERIAL_SW_TIMER_MAX) || ((WAIT_ONE) > SERIAL_SW_TIMER_MAX))
#error WAIT_ONE is set too high. Try to increase prescaler or use a higher baud rate.
#endif

#if ( (WAIT_ONE) < (100/PRESCALER))
#error A too high baud rate is used. Please check the PRESCALER and WAIT_ONE setting.
#endif


/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xRxQueue, SERIAL_SW_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTxQueue, SERIAL_SW_TXBUFSIZE);

volatile uint8_t ucStatus; //< Byte holding status flags.
static volatile uint8_t ucCounter;     //< Holds the counter used in the timer interrupt.

/* Communication parameters. */
static volatile uint8_t   ucTxData;     //< Byte holding data being transmitted.
static volatile uint8_t   ucRxData;     //< Byte holding data being received.
// static volatile uint8_t   ucTxBuffer;   //< Transmission buffer.
static volatile uint8_t   ucRxBuffer;   //< Reception buffer.


/* private functions ======================================================== */

// -----------------------------------------------------------------------------
/*  External interrupt service routine
 *
 *  The falling edge in the beginning of the
 *  start bit will trigger this interrupt.
 *  Timer interrupt is enabled and counter is
 *  is set to RECEIVE_FIRST_DATA to signal that the next bit is
 *  the first data bit to be received.
 */
ISR (SERIAL_SW_INT_vect) {

  // Make sure bit is low.
  if (! (SERIAL_SW_RX_PIN & _BV (SERIAL_SW_RX))) {

    ucCounter = RECEIVE_FIRST_DATA; // 1. data bit is the next to be received
    ucRxData = 0x00;
    vSerialSwTimerStop();    // Stop timer to make sure prescaler is reset.
    vSerialSwTimerClear();
    vSerialSwTimerSet (START_RECEIVE);
    vSerialSwIntDisable();
    vSerialSwTimerEnable();
    vSerialSwTimerStart (TIMER_CS);
  }
}


// -----------------------------------------------------------------------------
/*  Timer compare interrupt service routine
 *
 *  This interrupt is triggered when the compare
 *  register equals the timer. It increments the
 *  counter and handles the transmission and
 *  reception of each bit.
 *
 *  \note   The WAIT_ONE define has to be less than
 *          the maximum time spent in this routine
 *          so an interrupt is not missed.
 */
ISR (SERIAL_SW_TIMER_vect) {
  uint8_t bit_in = 0x00;

  //Set timer compare value to trigger the ISR once every bit period.
  vSerialSwTimerSet (WAIT_ONE);

  ////////////////////////////////
  // Receive if counter is odd. //
  ////////////////////////////////
  if (ucCounter & 0x01) {

    // Sample bit by checking the value on the UART pin:
    if (SERIAL_SW_RX_PIN & _BV (SERIAL_SW_RX)) {

      bit_in = 0x01;
    }

    // If to receive data bit -> Copy received bit into Rx_data.
    if (ucCounter <= RECEIVE_LAST_DATA) {

      // Right shift RX_data so the new bit can be masked into the Rx_data byte.
      ucRxData = (ucRxData >> 1);
      if (bit_in) {

        ucRxData |= 0x80; // Set MSB of RX data if received bit == 1.
      }
    }
    else {
      // If to receive stop bit -> Copy Rx data to Rx buffer.
      if (ucCounter == RECEIVE_STOP_1) {

        // Set frame error flag if a low stop bit is received.
        if (!bit_in) {

          ucStatus |= _BV (SERIAL_SW_FRAME_ERROR);
          // Disable timer and stop reception?
        }

        // Set overflow error if RX_buffer is not received before new data is ready.
        if (xQueueIsFull (&xRxQueue)) {

          ucStatus |= _BV (SERIAL_SW_RX_BUFFER_OVERFLOW);
        }
        else {

          vQueuePush (&xRxQueue, ucRxData);
        }
        vSerialSwIntEnable();
      }
      else {
        // If reception finished and no new incoming data has been detected.
        if (ucCounter == RECEIVE_FINISH) {

          if (xQueueIsEmpty (&xTxQueue)) {
            // If no data is waiting to be transmitted in the Tx bufferSet to IDLE
            // and disable interrupt if no TX data is waiting.

            vSerialSwTimerStop();
            vSerialSwTimerDisable();
            ucCounter = UART_STATE_IDLE;
            return; // Exit ISR so the counter is not updated.
          }
          else {
            // Initiate transmission if there is data in TX_buffer. This is done in the
            // same way as in the UART_transmit routine.

            // ucTxData = ucTxBuffer;
            // ucStatus &= ~_BV (SERIAL_SW_TX_BUFFER_FULL);
            ucTxData = ucQueuePull (&xTxQueue);
            vSerialSwIntDisable();
            SERIAL_SW_TX_PORT &= ~_BV (SERIAL_SW_TX); // Write start bit.
            // Update counter so the 1. data bit is the next bit to be transmitted.
            ucCounter = TRANSMIT_FIRST_DATA;
            vSerialSwTimerStop(); // Stop timer to reset prescaler.
            vSerialSwTimerClear();
            vSerialSwTimerSet (START_TRANSMIT);
            // TODO: pas de set irq à l'origine
            vSerialSwTimerEnable();
            vSerialSwTimerStart (TIMER_CS);
            return; // Exit ISR so the counter is not updated.
          }
        }
      }
    }
  }
  else {
    //////////////////////////////
    //Transmit if counter is even.
    //////////////////////////////
    uint8_t bit_out = 0x00;

    // Sample bit by checking the value on the UART pin:
    if (SERIAL_SW_TX_PIN & _BV (SERIAL_SW_TX)) {

      bit_in = 0x01;
    }

    // Check if bit equals last bit sent. Set frame error flag if not.
    // Also set ucTxData to the next value.
    if (ucCounter != TRANSMIT_FIRST_DATA)  {
      uint8_t last_bit_sent = 0x01;

      if (ucCounter <= TRANSMIT_STOP_1) {

        last_bit_sent = ucTxData & 0x01;
        // Right shift data bits. Does not have any effect for stop bits.
        ucTxData = ucTxData >> 1;
      }

      // if a high bit was sent, but a low received, set frame error flag.
      if (last_bit_sent != bit_in) {

        ucStatus |= _BV (SERIAL_SW_FRAME_ERROR);
        // Stop transmission ?
      }
    }

    // If to transmit data bit, set bit_out according to ucTxData.
    if (ucCounter <= TRANSMIT_LAST_DATA) {

      if (ucTxData & 0x01) {

        bit_out = 0x01;
      }
    }

    // If to transmit stop bit, set the bit to be transmitted high.
    else {
      if (ucCounter == TRANSMIT_STOP_1) {

        bit_out = 0x01;
      }
      else {
        if (ucCounter == TRANSMIT_STOP_2) {

          vSerialSwIntEnable();
          bit_out = 0x01;
        }
        else  {
          // If transmission finished. Start a new transmission if data is
          // present in the Tx buffer. Set the UART idle if not.

          // Check if new data is ready to be sent. If not, set UART state to idle,
          // disable the timer interrupt and enable the external interrupt to make
          // the UART wait for new incoming data.

          // READ_FLAG (ucStatus, SERIAL_SW_TX_BUFFER_FULL)
          if (xQueueIsEmpty (&xTxQueue)) {

            vSerialSwTimerStop();
            vSerialSwTimerDisable();
            ucCounter = UART_STATE_IDLE;
            return; // Exit ISR so the counter is not updated.
          }
          else {
            // Initiate transmission if there is data in TX_buffer.

            // ucTxData = ucTxBuffer;
            ucTxData = ucQueuePull (&xTxQueue);
            // ucStatus &= ~_BV (SERIAL_SW_TX_BUFFER_FULL);
            // Need to substract 2 because counter is updated at the end of the ISR.
            ucCounter = TRANSMIT_FIRST_DATA - 2;
            vSerialSwIntDisable();
            // bit_out already set to 0x00.
          }
        }
      }
    }
    // Transmit bit.
    if (bit_out) {

      SERIAL_SW_TX_PORT |=  _BV (SERIAL_SW_TX);
    }
    else {

      SERIAL_SW_TX_PORT &= ~_BV (SERIAL_SW_TX);
    }
  }
  ucCounter += 2; // Update counter.
}

/* internal public functions ================================================ */


// -----------------------------------------------------------------------------
/*
 *  Enable the UART.
 *  This function initialize the timer and buffers,
 *  and enables the external interrupt to sense
 *  for incoming data. It is important to run this
 *  function before calling any of the other UART
 *  functions. Received data will automatically be
 *  put in the RX buffer.
 *
 *  \note   The UART can be stopped by disabling the
 *          timer and external interrupt.
 */
void
vSerialSwEnable (void) {

  SERIAL_SW_RX_PORT |= _BV (SERIAL_SW_RX);
  SERIAL_SW_TX_DDR  |=  _BV (SERIAL_SW_TX);
  SERIAL_SW_TX_PORT |=  _BV (SERIAL_SW_TX);

  ucStatus = 0x00;
  ucCounter = UART_STATE_IDLE;

  vSerialSwTimerInit();
  vSerialSwIntInit();
}


// -----------------------------------------------------------------------------
/*
 *  Disable the UART.
 *  This function disables the UART by disabling the timer
 *  interrupt and the external interrupt.
 */
void
vSerialSwDisable (void) {

  ucStatus = 0x00;
  vSerialSwTimerDisable();
  vSerialSwIntDisable();
  vSerialSwTimerStop();
}

// -----------------------------------------------------------------------------
void
vSerialSwPutChar (char c) {

#if defined(CONFIG_EOL_CRLF)
  if (c == '\n') {
    (void) iSerialPutChar ('\r');
  }
#elif defined(CONFIG_EOL_CR)
  if (c == '\n') {
    c = '\r';
  }
#elif defined(CONFIG_EOL_LF)
  if (c == '\r') {
    c = '\n';
  }
#endif

  while (xQueueIsFull (&xTxQueue))
    ;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vQueuePush (&xTxQueue, c);
  }

  if (ucCounter == UART_STATE_IDLE) {
    // Start transmission if no ongoing communication.

    // Copy byte from buffer and clear buffer full flag.
    ucTxData = ucQueuePull (&xTxQueue);

    vSerialSwIntDisable();
    SERIAL_SW_TX_PORT &= ~_BV (SERIAL_SW_TX); // Write start bit.
    // Update counter so the 1. data bit is the next bit to be transmitted.
    ucCounter = TRANSMIT_FIRST_DATA;
    vSerialSwTimerClear();
    vSerialSwTimerSet (START_TRANSMIT);
    vSerialSwTimerEnable();
    vSerialSwTimerStart (TIMER_CS);
  }
}

// -----------------------------------------------------------------------------
void
vSerialSwPutString (const char *pcString) {

  while (*pcString) {

    vSerialSwPutChar (*pcString++);
  }
}

// -----------------------------------------------------------------------------
int
iSerialSwGetChar (void) {
  int c = _FDEV_EOF;

#ifdef AVRIO_SERIAL_SW_NOBLOCK
  // Version non-bloquante, renvoie EOF si rien reçu
  if (!xQueueIsEmpty (&xRxQueue)) {

    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      c = ucQueuePull (&xRxQueue);
    }
  }
#else
  // Version bloquante, on attends tant que rien n'est reçue
  while (xQueueIsEmpty (&xRxQueue))
    ;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    c = ucQueuePull (&xRxQueue);
  }
#endif

  return (unsigned int) c;
}

/* avr-libc stdio interface ================================================= */
static int prvPutChar (char c, FILE * pxStream);
static int prvGetChar (FILE * pxStream);

FILE xSerialSwPort = FDEV_SETUP_STREAM (prvPutChar, prvGetChar, _FDEV_SETUP_RW);

// -----------------------------------------------------------------------------
static int
prvPutChar (char c, FILE * pxStream) {

  vSerialSwPutChar (c);
  return 0;
}

// -----------------------------------------------------------------------------
static int
prvGetChar (FILE * pxStream) {

  clearerr (&xSerialSwPort);
  return iSerialSwGetChar ();
}


#endif /* AVRIO_SERIAL_SW_ENABLE defined */
/* ========================================================================== */
