AvrIO/src/arduino README file
$Id$
--------------------------------------------------------------------------------
Vous pouvez intégrer la librairie Arduino dans AvrIO sans modifier une ligne
de code à Arduino.

 Vousdevez au préalable récupérer les sources du projet Arduino ce qui peut se
faire à l'aide de la commande:
$ git clone git://git.epsilonrt.com/Arduino

Après cela vous pouvez construire le projet d'exemple dans avrio/examples/arduino.

Toutes les fonctions standards Arduino sont supportées (cores).

Seules les librairies suivantes sont supportées:
  * EEPROM - reading and writing to "permanent" storage
  * Firmata - for communicating with applications on the computer using a standard serial protocol.
  * LiquidCrystal - for controlling liquid crystal displays (LCDs)
  * SD - for reading and writing SD cards
  * Servo - for controlling servo motors
  * SPI - for communicating with devices using the Serial Peripheral Interface (SPI) Bus
  * SoftwareSerial - for serial communication on any digital pins. Version 1.0 and later of Arduino incorporate Mikal Hart's NewSoftSerial library as SoftwareSerial.
  * Stepper - for controlling stepper motors
  * Wire - Two Wire Interface (TWI/I2C) for sending and receiving data over a net of devices or sensors.
