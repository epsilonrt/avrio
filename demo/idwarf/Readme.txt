Welcome to the iDwaRF-Net Firmware!

In this file you will find some information that will help you getting
started with the iDwaRF-Net firmware. At the end of this file you will
find a log with the changes that were made.


[AVRSTUDIO]
In the each directories you will find some '.aps' projectfiles for
the use with AVRStudio. This is done for the users that like
to use winAVR out of the AVRStudio environment.


[Folder Structure]
.template - A placeholder for a new project. Everything is set up so that
you can directly start to enter your code without a hassle.

.terminal - the terminal demo application. This demo implements the sensor
functions (temperature, button, LED, poti) and the serial protocol used
for the communication between hub and host. This is the biggest demo with
the most code and functionality. 

.tutorial - the tutorial project does some simple LED toggling on the 
sensor and the hub side. It is documented to show step for step how
to implement some simple project using the iDwaRF firmware.

The firmware is exists in three versions, one for the hub, one for hubBox and 
one for the sensornode. The example projects exist in three versions, too.

UPDATE 12/07: A new configuration has been added to the projects. The iDwaRF-Box
setting. There are project files ending in "hubBox" and a new library file.


Change History:

Version 2.2
- Christmas edition 2007 ;)
- reduced the supply current! The iDwaRF module itself now only takes
  11.5ÁA while sleeping. If the internal RC with 8MHz is used the sleep
  current is reduced to 8ÁA! The complete nodeboard now takes 125ÁA while
  sleeping.
- added support for the new iDwaRF-Box hub module
- added iDwaRF-Box setting to terminal, QuadADC makefile
- added iDwaRF-Box setting to chat, empty makefile
- removed some compile warnings
- fixed an error deep inside the original cypress code. (packet sequence handling)
- fixed the tutorial example - works now with the nodeboard PCB.


Version 2.1
- AVR Studio projects files (re-)added

Version 2.0
- examples were modified to work with the new NodeBoard and HubBoard
- the library is identical for 1.2 and 2.0! So users of the older
  SensorBox can use the new optimized library

Version 1.2
- the power saving mode was optimized (3mA -> 0.3mA standby)
- added the >quad_adc< project
 
Version 1.1
- Added the >Tutorial< project. A small demonstration.
- This file is new for this version. This should help you getting started.
- The AVRStudio projects were added. For each example project two. One
  for the hub and one for the sensor version.
- The >empty< Project was added. It is a starting point for your own 
  projects without overhead.
- The makefiles were cleaned up.
- removed setLED & clearLED from the libraries.
- changed from STK500 to STK500v2 in the makefiles.


Version 1.0
- first public version. Terminal and Chat are the two example projects.



(c) by Christian Meinhardt 2006/2007
	cmeinhardt@neulandmm.de
iDwaRF-Net firmware is done for www.chip45.com
