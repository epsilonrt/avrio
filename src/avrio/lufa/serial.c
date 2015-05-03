/**
 * @file serial.c
 * @brief CDC Virtual Serial
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20121226 - Initial version by epsilonRT
 */
//#include <string.h>
#include <stdio.h>
#include <avrio/led.h>
#include "serial.h"
#include "serial_descriptors.h"
#include <LUFA/Drivers/USB/USB.h>

/* constants ================================================================ */
/* LED mask for the library LED driver, to indicate that the USB interface is not ready. */
#define LEDMASK_USB_NOTREADY      LED_LED1

/* LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
#define LEDMASK_USB_ENUMERATING  (LED_LED2 | LED_LED3)

/* LED mask for the library LED driver, to indicate that the USB interface is ready. */
#define LEDMASK_USB_READY        (LED_LED2 | LED_LED4)

/* LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
#define LEDMASK_USB_ERROR        (LED_LED1 | LED_LED3)

/* public variables ========================================================= */
/*  Standard file stream for the CDC interface when set up, so that the virtual 
 *  CDC COM port can be used like any regular character stream in the C APIs
 */
FILE xUsbSerial;

/* private variables ======================================================== */
/*  LUFA CDC Class driver interface configuration and state information. 
 *  This structure is passed to all CDC Class driver functions, so that multiple 
 *  instances of the same class within a device can be differentiated from 
 *  one another.
 */
static USB_ClassInfo_CDC_Device_t xVirtualSerialInterface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vSerialInit (void) {
  
	USB_Init();

	/* Create a regular character stream for the interface so that it can be used 
     with the stdio.h functions */
	CDC_Device_CreateStream (&xVirtualSerialInterface, &xUsbSerial);
  sei();
  while (USB_DeviceState != DEVICE_STATE_Configured)
    vSerialUsbTask();
}

// -----------------------------------------------------------------------------
void
vSerialUsbTask (void) {

  CDC_Device_USBTask(&xVirtualSerialInterface);
  USB_USBTask();
}

// -----------------------------------------------------------------------------
uint16_t 
usSerialHit (void) {
  uint16_t usChars;
  
  /* Must throw away unused bytes from the host, or it will lock up while 
     waiting for the device */
  usChars = CDC_Device_BytesReceived(&xVirtualSerialInterface);

  return usChars;
}

// -----------------------------------------------------------------------------
/* Event handler for the library USB Connection event. */
void 
EVENT_USB_Device_Connect(void) {

  vLedClear (LED_ALL_LEDS);
	vLedSet (LEDMASK_USB_ENUMERATING);
}

// -----------------------------------------------------------------------------
/* Event handler for the library USB Disconnection event. */
void 
EVENT_USB_Device_Disconnect(void) {

  vLedClear (LED_ALL_LEDS);
	vLedSet(LEDMASK_USB_NOTREADY);
}

// -----------------------------------------------------------------------------
/* Event handler for the library USB Configuration Changed event. */
void 
EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&xVirtualSerialInterface);

  vLedClear (LED_ALL_LEDS);
	vLedSet(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

// -----------------------------------------------------------------------------
/* Event handler for the library USB Control Request reception event. */
void 
EVENT_USB_Device_ControlRequest(void) {

	CDC_Device_ProcessControlRequest(&xVirtualSerialInterface);
}
/* ========================================================================== */
