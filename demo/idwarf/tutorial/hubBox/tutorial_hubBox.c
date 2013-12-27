/**
 * @file itutorial_hub.c
 * @brief iDwaRF Tutorial - Hub style
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net _ 2005-2007 All rights reserved.
 * @version $Id$
 * @anchor idwarf_tutorial_hub
 * Revision History ---
 *    20120227 - initial version from iDwaRF version 2.5 
 */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/serial.h>
#include <avrio/delay.h>

#define AVRIO_DEBUG_STREAM &xSerialPort
#include <avrio/debug.h>

#include <idwarf/hub.h>

/* macros =================================================================== */
/* constants ================================================================ */
#define LED_RX LED_LED1
#define LED_TX LED_LED2
#define BUTTON BUTTON_BUTTON1
#define DEBUG_BAUDRATE 115200
 
/* public variables ========================================================= */
// array of sensors. Set to 1 if there's a ucValid sensor
uint8_t ucValid[10];
uint8_t ucValue = 0;  // current sensor value that has its LED set

/* private functions ======================================================== */
#if defined(AVRIO_DEBUG_STREAM)
void
prvvDebugInit (void) {

  vSerialInit (DEBUG_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_WR);
}
#else
# define prvvDebugInit()
#endif

/* internal public functions ================================================ */
void cbSensorPacketReceived (PACKET_TYPES, uint16_t, uint8_t, volatile uint8_t *);

// -----------------------------------------------------------------------------
// the main entry for the user Software
int 
main (void) {
  bool xOldButton = false;

  vLedInit();
  vButInit();
  prvvDebugInit();
  vDbgPuts_P (PSTR("Testcode for the tutorial - hub style\r"));

  // initialize the RF - Firmware
  rfInit();

  // register callback: function called, each time a sensor data packet is received
  rfRegisterCBSensorDataReceived (cbSensorPacketReceived);
  
  // the main loop
  for (;;) {
    
    if (xButGet(BUTTON) == BUTTON) {
    
      if (xOldButton == false) {
      
        // debug output
        for (uint8_t n = 0; n < 10; n ++) {
        
          vDbgPutDec (ucValid[n]); 
          vDbgPutc ('.');
        }
        vDbgPutc (' ');

        // search for the next value sensor id
        for (ucValue++; ucValue < 10; ucValue++) {
        
          if (ucValid[ucValue]) 
            break;
        }
        
        if (ucValue == 10) {  // if overflow | no sensor ucValid, search again
        
          for (ucValue = 0; ucValue < 10; ucValue++) {
            if (ucValid[ucValue]) 
              break;
          }
        }
        if (ucValue == 10) 
          ucValue = 0;  // no sensor ucValid - fallback to id 0

        // debug output
        vDbgPutDec (ucValue); 
        vDbgPutc (' ');
        
        // mark all sensors as not ucValid for the next iteration
        for (uint8_t n = 0; n < 10; n++) {
        
          ucValid[n] = 0;
        }
        xOldButton = true;
      }
    } 
    else {
    
      if (xOldButton == true) 
        xOldButton = false;
    }
    
    rfProcessAll();
  }
}

// -----------------------------------------------------------------------------
// cbSensorPacketReceived (callback)
//  xPacketType     - the type of packet received: BIND_REQUEST or fixed/variable data packets
//  DeviceID        - id of the device the data was send from
//  ucUserDataCount - amount of bytes / user data payload stored in pucBuffer
//  pucBuffer       - pointer to the transmitted data bytes. Only ucUserDataCount bytes are ucValid
//
void cbSensorPacketReceived (PACKET_TYPES xPacketType, uint16_t usDeviceId, 
                uint8_t ucUserDataCount, volatile uint8_t * pucBuffer) {
  uint8_t lastPacket;

  // a data paket from the sensor was received. Place your own code

  if (ucUserDataCount > 1) {
  
    if (*pucBuffer == 1) {  // this packet triggers the LED toggle

      vLedToggle (LED_RX);
      lastPacket = pucBuffer[1];  // remember the packet number
    }
    else {

      vLedToggle (LED_TX);
    }
  }
  
  // set the backchannel data if there's storage free
  if (rfIsBackchannelFree (usDeviceId)) {
  
    // set the LED of the >ucValue< usDeviceId. All others are unset
    //  The beacon time is set to 2 fo faster reaction time.
    // The packet number is retransmitted as an example.
    rfSetBackchannelData (usDeviceId, usDeviceId == ucValue, 2, &lastPacket, 1);
    ucValid[usDeviceId] = 1;  // this Sensor is active & valid
  }
}
