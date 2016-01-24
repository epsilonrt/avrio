/*
 * iDwaRF Terminal - Hub style
 *
 * @author Christian Meinhardt, cmeinhardt@cwmsys.de for www.chip45.com
 * @copyright iDwaRF-Net © 2005-2007 All rights reserved.
 *
 * This file is part of the Terminal - program example - hub version.
 * The devices (Sensors) connect to the hub and can be controlled by
 * the serial interface. The code that implements the serial protocol,
 * as well as the rf code is given here.
 *  
 * This is a simulation of the users main.c File. It uses the 
 * iDwaRF Firmware to have access to the rf hardware. Take this code
 * as an example and explore the possibilities.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avrio/serial.h>
#include <avrio/term.h>
#include <avrio/util.h>
#include <avrio/led.h>

#include <idwarf/hub.h>

/* constants ================================================================ */
#define DEBUG_BAUDRATE 115200
#define ADC_SCALE 3.3
#define CMD_SIZE 21
#define BEACON_TIME IDWARF_SLEEP_TIME(5) // 5 sec.

/* public variables ========================================================= */
// buffered data, received from the sensor
uint8_t ucData[IDWARF_PAYLOAD_SIZE];
uint8_t ucDataLen = 0;
uint16_t usBeaconTime = BEACON_TIME;

// global flag for hex mode
enum {
  HEX_FLAG = 0x01,
  PRINT_FLAG = 0x02
};

volatile uint8_t ucFlags = HEX_FLAG | PRINT_FLAG;

/* internal public functions ================================================ */
void vSensorPacketReceived (PACKET_TYPES, uint16_t, uint8_t, volatile uint8_t *);
void vProcessRxData (void);
void vPrintStatus (void);
void vPrintOk (void);
void vPrintBeaconTime (void);
void vPrintHelp (void);

// -----------------------------------------------------------------------------
// Le programme principal
int 
main (void) {

  vLedInit();
  vSerialInit (DEBUG_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;

  printf_P (PSTR("\r\riDwaRF - Firmware v" __IDWARF_VERSION_STRING__ " Hub version\n\n"));

  // initialise le firmware iDwaRF
  printf_P(PSTR("Init... "));
  rfInit(); // les interruptions sont validées...
  vPrintOk();

  // Enregistre les fonctions de traitement de l'utilisateur 
  printf_P(PSTR("Register Call Back functions... "));
  rfRegisterCBSensorDataReceived (vSensorPacketReceived);
  vPrintOk();

  vPrintStatus();
  vPrintBeaconTime();
  vPrintHelp();
  puts_P(PSTR("\nProcess All ..."));

  for (;;) {
    
    // la boucle principale gère les événements du réseau...
    rfProcessAll();

    // if the serial callback has set this flag, there is new data to be processed.
    //  done here to prevent overload in the callback function
    if (usSerialHit()) {
    
      vProcessRxData();
    }
    
    // show the saved data received with the last sensor packet 
    //  OutStr takes some time - that's the reason why it is done here!
    //  ucData holds the data and ucDataLen is the amount of data stored in ucData.
    if (ucDataLen > 0) {
      uint8_t * pucData = ucData;

      if ((ucFlags & HEX_FLAG) == 0) {  // show data user friendly

        if (ucDataLen >= 1) {
 
          // Push Button state - 1 byte
          printf_P (PSTR("Button %s"), (*pucData++ != 0 ? "ON " : "off"));
        }
        
        if (ucDataLen >= 2) {
          uint8_t ucBatt;
          
          // Battery voltage - 1 byte
          ucBatt = *pucData++;
          printf_P (PSTR(" Batt (%d) %.2fV"), ucBatt, ADC_SCALE * (float) ucBatt / 256.0);
        
        }

        if (ucDataLen >= 4) {
          int16_t iTemp;
          
          // Temperature value - 2 bytes
          iTemp   = (*pucData++) << 8;
          iTemp  +=  *pucData++;
          printf_P (PSTR(" Temp (%d) %.1foC"), iTemp, (float)iTemp / 10.0);
       
        }
        
        if (ucDataLen >= 5) {
        
          // Light Dependent Resistor value - 1 byte
          printf_P (PSTR(" Ldr %d"), *pucData++);
        }
      }
      
 
      while (pucData < (ucData + ucDataLen)) {
      
        printf_P (PSTR("%02X "), *pucData++);
      }

      // show the payload size
      printf_P (PSTR("> len=%d\n"), ucDataLen);
      ucDataLen = 0;  // reset. Otherwise this data is displayed non stop.
    }    
  }
}

/* internal public functions ================================================ */

void vPrintOk (void);
void vPrintError (void);
void vEnumerateAllSensors (void);
void vCleanupSensorList (void);
void vDoTimCommand (char * cCmd, uint8_t ucLen);
void vDoCnfCommand (char * cCmd, uint8_t ucLen);
void vDoSndCommand (char * cCmd, uint8_t ucLen);
void vDoDelCommand (char * cCmd, uint8_t ucLen);
void vSendBackData (uint16_t usDeviceId, uint8_t * pucPayload, uint8_t ucPayloadLen);

// -----------------------------------------------------------------------------
// vSensorPacketReceived (callback)
//  xPacketType     - Type de paquet reçu: BIND_REQUEST or fixed/variable data packets
//  usDeviceId      - Identifiant du capteur qui a envoyé le paquet
//  ucUserDataCount - nombre d'octets de payload stockés dans pucBuffer
//  pucData         - Pointeur sur les données transmises. Seuls les ucUserDataCount octets sont valides.
//
void vSensorPacketReceived (PACKET_TYPES xPacketType, uint16_t usDeviceId, 
                uint8_t ucUserDataCount, volatile uint8_t * pucData) {
                
  // Un paquet a été reçu venant d'un capteur: Placez votre code ici.
  static uint8_t ucPacketCount;
  
  vLedToggle (LED_LED1);
  
  if (ucUserDataCount == 5) {
  
    if (pucData[4] != 0) {  // this packet triggers the LED toggle

      vLedToggle (LED_LED2);
    }
  }

  ucPacketCount++;

  if (ucFlags & PRINT_FLAG) {
  
    printf_P (PSTR("%03d> S%d-"), ucPacketCount, xPacketType);
    switch (xPacketType) {
    
      case BIND_REQUEST_PACKET: // 0x00
        puts_P (PSTR("BIND_REQUEST\n"));
        break;

      case FIXED_DATA_PACKET: // 0x04: Data packet of fixed length as defined by SIZE_OF_FIXED_DATA_PACKET
        // no break

      case VARIABLE_DATA_PACKET: // 0x05: Data packet of variable length
        printf_P (PSTR("ID%d <"), usDeviceId);
        // copy payload to the buffer for later, time intensive processing
        for (uint8_t n = 0; n < ucUserDataCount; n ++) {
          
          ucData[n] = *pucData++;
        }
        ucDataLen = ucUserDataCount;
        break;

      default:
        break;
    }
  }
    
  // set the backchannel data if there's storage free
  if (rfIsBackchannelFree (usDeviceId)) {
  
    rfSetBackchannelData (usDeviceId, 1, usBeaconTime, &ucPacketCount, 1);
  }
}

// -----------------------------------------------------------------------------
// implements the serial protocol of the hub module
// this function is called from the mainloop when data from the usart has
// been received.
void 
vProcessRxData (void) {
  static char cCmd[CMD_SIZE];
  static uint8_t ucPos;
  static char cLastChar;
  
  while (usSerialHit() && (cLastChar != '\r')) {
  
    cLastChar = cSerialGetChar();
    ucFlags &= ~PRINT_FLAG;
    cCmd[ucPos] = cLastChar;
    if (ucPos < CMD_SIZE) {
    
      ucPos++;
    }
    else {
    
      ucPos = 0;
    }
  }
              
  // check for the end char - \r triggers the command matching
  if (cLastChar == '\r') {

    if (strncmp_P (cCmd, PSTR("rst\r"), 4) == 0) {
 
      // RESET
      vPrintOk();     // acknowledge the rs command
      rfReset();
    } 
    else if (strncmp_P (cCmd, PSTR("gps\r"), 4) == 0) {

      // GETPROTOCOLSTATUS
      vPrintOk();
      vPrintStatus();
    } 
    else if (strncmp_P (cCmd, PSTR("bon\r"), 4) == 0) {
    
      // start bind / bind on
      vPrintOk();
      rfStartBind();
      vPrintStatus();
    } 
    else if (strncmp_P (cCmd, PSTR("bof\r"), 4) == 0) {
    
      // stop bind / bind off
      vPrintOk();
      rfStopBind();  
      vPrintStatus();
    } 
    else if (strncmp_P (cCmd, PSTR("enu\r"), 4) == 0) {
      
      // enumerate all sensors
      vPrintOk();
      vEnumerateAllSensors();
    } 
    else if (strncmp_P (cCmd, PSTR("cln\r"), 4) == 0) {
    
      // clear up the sensorlist
      vPrintOk();
      vCleanupSensorList();
    } 
    else if (strncmp_P (cCmd, PSTR("hex\r"), 4) == 0) {
      
      // switch output to hex numbers
      vPrintOk();
      ucFlags ^= HEX_FLAG;
      printf_P (PSTR("hex %d\r"), ucFlags & HEX_FLAG);
    } 
    else if (strncmp_P (cCmd, PSTR("hlp\r"), 4) == 0) {

      // display help message
      vPrintOk();
      vPrintHelp();
    } 
    else if (strncmp_P (cCmd, PSTR("cnf"), 3) == 0) {
      
      // configure network parameters
      vDoCnfCommand (cCmd, ucPos);
    } 
    else if (strncmp_P (cCmd, PSTR("tim"), 3) == 0) {
      
      // configure beacon time
      vDoTimCommand (cCmd, ucPos);
    } 
    else if (strncmp_P (cCmd, PSTR("snd"), 3) == 0) {
      
      // send backchannel data
      vDoSndCommand (cCmd, ucPos);
    } 
    else if (strncmp_P (cCmd, PSTR("del"), 3) == 0) {
      
      // remove sensor(s) from the hub
      vDoDelCommand (cCmd, ucPos);
    } 
    else {  // unknown command - return error code
      
      vPrintError();  // signal a problem
    }
    ucPos = 0;  // reset the buffer to sync to the input
    cLastChar = 0;
    ucFlags |= PRINT_FLAG;
  }
}

// -----------------------------------------------------------------------------
// serial protocol - everything is right
void
vPrintHelp (void) {

  puts_P(PSTR("\n\nHub commands list:\n"
              "rst\t\t\t\t - Reset firmware\n"
              "gps\t\t\t\t - Get protocol status\n"
              "bon\t\t\t\t - Set Bind On\n"
              "bof\t\t\t\t - Set Bind Off\n"
              "enu\t\t\t\t - Enumarate all sensors\n"
              "cln\t\t\t\t - Cleanup sensor list\n"
              "hex\t\t\t\t - Switch output to hex numbers\n"
              "hlp\t\t\t\t - Display this help\n"
              "cnf PNCODE CHANNEL\t\t - Configure Network parameters\n"
              "tim BEACONTIME\t\t\t - Configure beacon time (unit=125ms)\n"
              "snd DEVICEID BYTE1 [BYTE2]...\t - Send backchannel data\n"
                  "\t\t\t\t   DEVICEID=x playload send to all devices\n"
              "del [DEVICEID]\t\t\t - Remove sensor(s) from the hub\n\n"
  ));
}

// -----------------------------------------------------------------------------
// serial protocol - everything is right
void 
vPrintOk (void) {

  puts_P (PSTR("Ok"));
}


// -----------------------------------------------------------------------------
// serial protocol - something was wrong. Command not understood
//  or error while performing command.
void 
vPrintError (void) {

  puts_P (PSTR("Error"));
}

// -----------------------------------------------------------------------------
void 
vPrintStatus (void) {
  uint8_t ucStatus;

  // GETPROTOCOLSTATUS
  ucStatus = rfGetProtocolStatus();
  printf_P (PSTR("Status 0x%02X\n"), ucStatus);
}

// -----------------------------------------------------------------------------
void 
vPrintBeaconTime (void) {

  printf_P (PSTR("Beacon Time %d s.\n"), usBeaconTime >> 3);
}


// -----------------------------------------------------------------------------
// vEnumerateAllSensors
//  show all Devices/Sensors connected to the hub.
//  go through all possible DeviceIds and query for a valid entry
//  and its MID value. This data is than transfered to the host
//
void 
vEnumerateAllSensors (void) {
  uint16_t usDeviceId;
  uint16_t usNumOfDevicesFound = 0;
  uint16_t usMaximumDeviceId = rfGetMaximumDeviceId();
  
  printf_P (PSTR("Maximum Devices = %d\n"), usMaximumDeviceId);
  
  for (usDeviceId = 0; usDeviceId <= usMaximumDeviceId; usDeviceId++) {
    MID xMid;
    
    if (rfGetSensorMID (usDeviceId, &xMid)) {
    
      // valid Device found
      vSwapBytes ((uint8_t *) &xMid.u32Mid, 4);
      printf_P (PSTR("deviceID = %d %+08lX\n"), usDeviceId, xMid.u32Mid);
      usNumOfDevicesFound++;
    }
  }
  printf_P (PSTR("nofDevices = %d\n"), usNumOfDevicesFound);
}


// -----------------------------------------------------------------------------
// remove duplicated entries out of the bound devices list. If a device/sensor
//  tries to bind multiple time, it gets multiple deviceIds. There is not enough
//  time in the firmware to check for duplicates while bind. So this functions
//  removes them, when called. This can be quite time consuming.
//
void 
vCleanupSensorList (void) {
  uint16_t usDeviceId1;
  uint16_t usMaximumDeviceId = rfGetMaximumDeviceId();
  
  // go through all bound and active sensor nodes
  for (usDeviceId1 = 0; usDeviceId1 <= usMaximumDeviceId; usDeviceId1++) {
    MID xMid1;
    uint16_t usDeviceId2;
    
    if (rfGetSensorMID (usDeviceId1, &xMid1)) {
    
      for (usDeviceId2 = usDeviceId1 + 1; usDeviceId2 <= usMaximumDeviceId; usDeviceId2++) {
        MID xMid2;
        
        if (rfGetSensorMID (usDeviceId2, &xMid2)) {
        
          // check for a duplicate MID entry
          if (memcmp (&xMid1, &xMid2, 4) == 0) {
          
            printf_P (PSTR ("Same entry found! remove the first one %d\n"), usDeviceId1);
            rfDeleteSensor (usDeviceId1);
            break;
          }
        }
      }
    }
  }
}

// -----------------------------------------------------------------------------
// vSendBackData()
//  deviceID - the id of the sensor, the target of the message
//  pucPayload - pointer to the data that should be send to the sensor
//  ucPayloadLen - amount of data that payload points to. 
//
void 
vSendBackData (uint16_t usDeviceId, uint8_t * pucPayload, uint8_t ucPayloadLen) {
  static uint8_t ucPktCount = 0;  // ucPktCount is incremented each time, just for fun + to see it is working
  uint8_t ucStatus;
  uint8_t * pucData = pucPayload;
  
  printf_P (PSTR ("SND-ID%d <%04X %02X"), usDeviceId, usBeaconTime, ucPktCount);
  while (pucData < (pucPayload + ucPayloadLen)) {
  
    printf_P (PSTR("%02X "), *pucData++);
  }
  // show the payload size
  printf_P (PSTR("> %d\n"), ucPayloadLen + 3);
  
  // queue packet for transfer to the device/sensor
  ucStatus = rfSetBackchannelData (usDeviceId, ucPktCount++, usBeaconTime, pucPayload, ucPayloadLen);
  if (ucStatus == CMD_STATUS_MSG_QUEUED) {
  
    vPrintOk();
  }
  else {
  
    vPrintError();
  }
}

// -----------------------------------------------------------------------------
// process the <snd> command and parse three parameters. 
//  syntax: snd [usDeviceId] [payload]
//    when usDeviceId=x the playload is send
//    to all devices currently bound to the hub
//
void 
vDoSndCommand (char * cCmd, uint8_t ucLen) {
  uint16_t usDeviceId = 0;
  bool  sndToAll = false;
  uint8_t  * pucPayload = NULL;
  uint8_t  ucPayloadLen = 0;
  uint8_t ucIndex;
  
  if (ucLen > 4) {
    // query for first number - cCmd+4 points to first character of the number
    if (cCmd[4] == 'x') {
    
      sndToAll = true;
    } 
    else {
    
      usDeviceId = atoi (&cCmd[4]);
    }
    
    // look for additional data, and set the pointer if found
    for (ucIndex = 4; ucIndex < ucLen; ucIndex++) {
    
      if ((cCmd[ucIndex] == ' ') && (ucLen > ucIndex + 1)) {
      
        pucPayload = (uint8_t *) &cCmd[ucIndex + 1];  // cut the leading space
        ucPayloadLen = ucLen - (ucIndex + 2);  // cut the trailing \r
        break;
      }
    }
  }
  
  if (sndToAll == false) {  // send data to one sensor
  
    vSendBackData (usDeviceId, pucPayload, ucPayloadLen);
  } 
  else {        // send data to all sensors
    uint16_t usDeviceId;
    uint16_t usMaximumDeviceId = rfGetMaximumDeviceId();
    
    for (usDeviceId = 0; usDeviceId <= usMaximumDeviceId; usDeviceId++) {
      uint8_t ucStatus;
      
      ucStatus = rfGetSensorMID (usDeviceId, NULL);
      
      if (ucStatus) {
      
        vSendBackData (usDeviceId, pucPayload, ucPayloadLen);
      }
    }
  }
}


// -----------------------------------------------------------------------------
// process the <cnf> command and parse two parameters. 
//  syntax: cnf [ucPnCode] [ucChannel]
//    calls rfConfigureNetwork
//
void 
vDoCnfCommand (char * cCmd, uint8_t ucLen) {
  uint8_t ucPnCode = 1;  // set the default values
  uint8_t ucChannel = 1;
  uint8_t ucStatus;

  if (ucLen > 4) {
    uint8_t ucIndex;
    
    // query for first number - cCmd+4 points to first character of the number
    ucPnCode = atoi (&cCmd[4]);
    
    for (ucIndex = 4; ucIndex < ucLen; ucIndex++) {
    
      if (cCmd[ucIndex] == ' ') {
      
        ucChannel = atoi (&cCmd[ucIndex]);
        break;
      }
    }
  }
  
  // this code changes the mid value of the hub
  //U32 nMid = 0x6035f934;//34 f9 35 60
  //uint8_t ucStatus = rfConfigureNetwork (ucPnCode, ucChannel, &nMid);
   
  // mid value should stay as it is given in the iDwaRF module
  ucStatus = rfConfigureNetwork (ucPnCode, ucChannel, NULL);
  if (ucStatus == 0) {
  
    vPrintOk();
    printf_P (PSTR ("Pn=%d - Ch=%d\n"), ucPnCode, ucChannel);
  }
  else {
  
    vPrintError();
  }
}
// -----------------------------------------------------------------------------
// process the <tim> command and parse one parameter. 
//  syntax: tim [beacontime]
//
void 
vDoTimCommand (char * cCmd, uint8_t ucLen) {

  if (ucLen > 4) {
    
    // query for first number - cCmd+4 points to first character of the number
    usBeaconTime = atoi (&cCmd[4]);
  }
  
  vPrintBeaconTime();
}


// -----------------------------------------------------------------------------
// process the <del> command and parse one parameter. 
//  syntax: del [usDeviceId]
//    removes the sensor <devideId> from the list of known sensors.
//
void 
vDoDelCommand (char * cCmd, uint8_t ucLen) {
  uint16_t usDeviceId = 0;  // set the default values
  uint8_t ucStatus;

  if (ucLen > 4) {
  
    // query for first number - cCmd+4 points to first character of the number
    usDeviceId = atoi (&cCmd[4]);
  }

  ucStatus = rfDeleteSensor (usDeviceId);
  vPrintOk();
  printf_P (PSTR("0x%02X\n"), ucStatus);
}
/* ========================================================================== */
