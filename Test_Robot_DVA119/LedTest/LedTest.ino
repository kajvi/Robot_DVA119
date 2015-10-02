/* Blink without Delay

 Turns on and off a light emitting diode (LED) connected to a digital
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.

 The circuit:
 * LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 that's attached to pin 13, so no hardware is needed for this example.
 
 created 2005
 by David A. Mellis
 modified 8 Feb 2010
 by Paul Stoffregen
 modified 11 Nov 2013
 by Scott Fitzgerald
 
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

myMotors Motors; // Create motors object

// constants won't change. Used here to set a pin number :
const int ledRedPin =  0;      // the number of the LED pin
const int ledGreenPin =  1;

// Variables will change :
int ledRedState = LOW;             // ledState used to set the LED
int ledGreenState = LOW;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  
  //Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println(C_THIS_VERSION);
  
  //Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
} // setup


void loop()
{
    // if the LED is off turn it on and vice-versa:
    if (ledRedState == LOW)
      ledRedState = HIGH;
    else
      ledRedState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledRedPin, ledRedState);
    delay(1500);
    
    // if the LED is off turn it on and vice-versa:
    if (ledGreenState == LOW)
      ledGreenState = HIGH;
    else
      ledGreenState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledGreenPin, ledGreenState);
    delay(1500);
}

