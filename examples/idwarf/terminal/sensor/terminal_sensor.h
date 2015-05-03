/**
 * @file template_sensor.h
 * @brief iDwaRF Template - Sensor style
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net _ 2005-2007 All rights reserved.

 * Revision History ------------------------------------------------------------
 *    20120227 - initial version from iDwaRF version 2.5 
 *
 * This file is part of the Terminal - program example - sensor version.
 * The device will connect to a present hub and transmit its sensor data
 * via rf. The sampling of the analog voltages as well as the access to
 * the one-wire device is done here. 
 */
 
#ifndef _IDWARF_TERMINAL_SENSOR_H_
#define _IDWARF_TERMINAL_SENSOR_H_

#include <avr/io.h>

/* constants ================================================================ */
#if defined(AVRIO_BOARD_IDWARF_NODE)
// -----------------------------------------------------------------------------
// Code for the iDwaRF-Sensor Node PCB
// just to get things rolling and moving ;o)
// start code
// iDwaRF Port pins
// (1)  PORT0 (OC0B T1 PD5)   (AIN1 PD7)   (ADC3 PC3)
// (2)  PORT1 (OC0A AIN0 PD6) (ADC2 PC2)
// (3)  PORT2 (SCL ADC5 PC5)
// (4)  PORT3 (SDA ADC4 PC4)
// (5)  PORT4 (TXD PD1)
// (6)  PORT5 (RXD PD0)
// (7)  PORT6 (INT1 OC2B PD3) (XCK T0 PD4) 
// (8)  PORT7 (CLK0 ICP PB0)  (OC1A PB1)
// (9)  MISO  (MISO PB4)
// (10) VCC   (VCC)
// (11) SCK   (SCK PB5)
// (12) MOSI  (MOSI OC2 PB3)
// (13) RESET (RESET PC6)
// (14) GND   (GND)

// NodeBoard is the latest version of the sensor module
// IIC LM75 temperature sensor, light sensor, push button, LED
// and optional dataflash

// PORT0   Vbatt
// PORT1   LDR analog signal
// PORT2   SCL to LM75
// PORT3   SDA to LM75
// PORT4   LED (low -> LED on)
// PORT5   push button
// PORT6   over temperature output of LM75 (low active -> generate an INT)
// PORT7   1) power supply for LDR (high -> LDR on, J2 must be closed, J1 must be open)
//         2) chip select for data flash, J1 must be closed, J2 must be open)

// Battery monitor pin and registers
#define BATT_DDR  DDRC
#define BATT_PORT PORTC
#define BATT_PIN  PINC
#define BATT      PINC3

// LDR analog signal pin and registers
#define LDR_DDR  DDRC
#define LDR_PORT PORTC
#define LDR_PIN  PINC
#define LDR      PINC2

// LDR power pin and registers
#define LDR_POWER_DDR  DDRB
#define LDR_POWER_PORT PORTB
#define LDR_POWER_PIN  PINB
#define LDR_POWER      PINB0

// ADC
#define ADC_REF _BV(REFS0)  // AREF = AVCC
//#define ADC_REF (_BV(REFS1) | _BV(REFS0))  // AREF = Internal Volatge Ref. 1.1V

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
// initialize battery monitor, pin is input, no pullup
static inline void 
vBatteryMonitorInit (void) {
  // ADC2
  BATT_DDR  &= ~_BV(BATT);
  BATT_PORT &= ~_BV(BATT);
}

// -----------------------------------------------------------------------------
// initialize LDR, signal pin is input, no pullup, power pin is output, first low
static inline void 
vLdrInit (void) {
  // ADC3
  LDR_DDR &= ~_BV(LDR);
  LDR_PORT &= ~_BV(LDR);
  LDR_POWER_DDR |= _BV(LDR_POWER);
  LDR_POWER_PORT &= ~_BV(LDR_POWER);
}

// -----------------------------------------------------------------------------
// turn off power to LDR
static inline void 
vLdrDisable (void) {

  LDR_POWER_PORT &= ~_BV(LDR_POWER);
}
// -----------------------------------------------------------------------------
// turn on power to LDR
static inline void 
vLdrEnable (void) {

  LDR_POWER_PORT |= _BV(LDR_POWER);
}

// -----------------------------------------------------------------------------
// enabling the power reduction ADC bit
static inline void 
vAdcEnable (void) {

  PRR &= ~_BV(PRADC);
  ADMUX = ADC_REF | (BATT<<MUX0);
}

// -----------------------------------------------------------------------------
// disabling the power reduction ADC bit
static inline void 
vAdcDisable (void) {

  ADCSRA = 0;
  PRR |= _BV(PRADC);
}

// ------------------------------------------------------------------------------
// read ADC value
//
static uint8_t 
ucReadAdc (uint8_t ucMux) {

  ADMUX = ADC_REF | _BV(ADLAR) | (ucMux<<MUX0);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | (3<<ADPS0);
  
  // wait for end of conversion
  while (ADCSRA & _BV(ADSC))
    ;

  return ADCH;;
}

#elif defined(AVRIO_BOARD_DMNET_HUB)
// -----------------------------------------------------------------------------
// Code for the epsilonRT Domonet HUB

// Battery monitor pin and registers
#define BATT_DDR  DDRC
#define BATT_PORT PORTC
#define BATT_PIN  PINC
#define BATT      PINC3

// LDR analog signal pin and registers
#define LDR_DDR  DDRC
#define LDR_PORT PORTC
#define LDR_PIN  PINC
#define LDR      PINC2

// LDR power pin and registers
#define LDR_POWER_DDR  DDRD
#define LDR_POWER_PORT PORTD
#define LDR_POWER_PIN  PIND
#define LDR_POWER      PIND5

// ADC
#define ADC_REF _BV(REFS0)  // AREF = AVCC
//#define ADC_REF (_BV(REFS1) | _BV(REFS0))  // AREF = Internal Volatge Ref. 1.1V

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
// initialize battery monitor, pin is input, no pullup
static inline void 
vBatteryMonitorInit (void) {
  // ADC2
  BATT_DDR  &= ~_BV(BATT);
  BATT_PORT &= ~_BV(BATT);
}

// -----------------------------------------------------------------------------
// initialize LDR, signal pin is input, no pullup, power pin is output, first low
static inline void 
vLdrInit (void) {
  // ADC3
  LDR_DDR &= ~_BV(LDR);
  LDR_PORT &= ~_BV(LDR);
  LDR_POWER_DDR |= _BV(LDR_POWER);
  LDR_POWER_PORT &= ~_BV(LDR_POWER);
}

// -----------------------------------------------------------------------------
// turn off power to LDR
static inline void 
vLdrDisable (void) {

  LDR_POWER_PORT &= ~_BV(LDR_POWER);
}
// -----------------------------------------------------------------------------
// turn on power to LDR
static inline void 
vLdrEnable (void) {

  LDR_POWER_PORT |= _BV(LDR_POWER);
}

// -----------------------------------------------------------------------------
// enabling the power reduction ADC bit
static inline void 
vAdcEnable (void) {

  PRR &= ~_BV(PRADC);
  ADMUX = ADC_REF | (BATT<<MUX0);
}

// -----------------------------------------------------------------------------
// disabling the power reduction ADC bit
static inline void 
vAdcDisable (void) {

  ADCSRA = 0;
  PRR |= _BV(PRADC);
}

// ------------------------------------------------------------------------------
// read ADC value
//
static uint8_t 
ucReadAdc (uint8_t ucMux) {

  ADMUX = ADC_REF | _BV(ADLAR) | ucMux;
  ADCSRA = _BV(ADEN) | _BV(ADSC) | (3<<ADPS0);
  
  // wait for end of conversion
  while (ADCSRA & _BV(ADSC))
    ;

  return ADCH;;
}

#else
// ------------------------------------------------------------------------------
/*
  Supported Boards List:
  - IDWARF_NODE
  - DMNET_HUB
 */
#error "Board not supported ! See terminal_sensor.h for suppoted boards list."

#endif  /* IDWARF_NODE defined */

/* ========================================================================== */
#endif  /* _IDWARF_TERMINAL_SENSOR_H_ not defined */

