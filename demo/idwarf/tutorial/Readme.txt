--- Tutorial ---


This tutorial demonstrates the creation of a small project from the ground up.

It should help you getting started with the iDwaRF-Net firmware.


This project should do one very simple task:

- push the button on one sensor -> toggle the LED on the HUB
- push the button on the hub -> toggle the LED on the Sensor(s)


-> Take the >Empty< firmware as template

copy the project & change the project name etc.


-> LED Code

For this tutorial/example we are using the SensorBox PCB. 
The LED ist connected to PORT7 of the iDwaRF Module. It is wired
low active, so setting the port to 0 powers the LED.

PORT7 is connected to two of the Mega168 pins.
- CLK0 / ICP / PB0
- OC1A / PB1

For this example we will use PB0 as output pin and tristate PB1. 
Always remember to initialize all PINs connected to one Port,
to prevent mishaps.

To initialize the two pins connected to the LED we will use
the following code:


void initLEDPort (void)
{
	PORTB &= ~(1<<PIN1);	// no pullup for PB1
	DDRB &= ~(1<<PIN1);		// set PB1 as input / tristate
	PORTB |= (1<<PIN0);		// set PB1 to active. (Low active LED is now OFF)
	DDRB |= (1<<PIN0);		// set PB0 as output
}

This code is placed in tutorial.c

The next two functions are needed to access the LED. Switch it off or on.


void setLED (void) {
	PORTB &= ~(1<<PIN0);	// set to inactive / LED is ON
}

void clearLED (void) {
	PORTB |= (1<<PIN0);		// set to active / LED isoff
}


Place them in tutorial.c. Do not forget to add their definitions to the header file.


-> button code

The button on the SensorBox is connected to PORT6 of the iDwaRF module.
When closed it switches PORT6 to ground. 

PORT6 is connected to two Mega168 pins.
- INT1 / OC2B / PD3
- XCK / T0 / PD4

We will use PD3 as our input for this example. Both Portpins are defined
as input. The pullup of PD3 is activated to have high level when the button 
is open.


void initButtonPort (void)
{
	PORTD |=  (1<<PIN3);	// use pullup for PD3
	DDRD  &= ~(1<<PIN3);	// set PD3 as input / tristate
	PORTD &= ~(1<<PIN4);	// no pullup for PD4
	DDRD  &= ~(1<<PIN4);	// set PD4 as input / tristate
}


U8 getButton (void) 
{
	return (PIND & (1<<PIN3))?0:1;	// if set return 0 else 1
}


If you want to test both, the LED & the button code, you can place
if (getButton()) setLED(); else clearLED(); inside the main loop.

Place their definitions in the header.



-> Sensor code

The sensor should test its button and transmit a special data message
to the HUB if it has been pressed. 

The sensor spends most of his time sleeping to preserve the battery
power. To react on the button it has to wake up.

This code configures INT1 to trigger an external interrupt.
Press/Release Button -> trigger interrupt


	// every change on INT1 should trigger an interrupt
	EICRA |= (0<<ISC11) | (1<<ISC10);

	// enable the external interrupt on INT1
	EIMSK |= (1<<INT1);


To test this code, you can use. The empty interrupt function with the
presented testcode from before.


SIGNAL (SIG_INTERRUPT1)
{
	// button testcode
	if (getButton()) setLED(); else clearLED();
}


----
[...Unfortunately I never came around to finish this document. The tutorial project
 itself is working and living well. So please look inside the source to see the rest
 of the code. Christian Meinhardt Feb/07]

[Update 12/07 - on the nodeboard the button is connected to another port on the iDwaRF
module. So the interrupt is no longer an option. The code changed for the new version,
this tutorial is still the old one. :(]



