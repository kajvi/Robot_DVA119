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

mySensors Sensors; // create Sensors object
myMotors Motors; // Create motors object

// constants won't change. Used here to set a pin number :
const int ledRedPin =  12;      // the number of the LED pin
const int ledGreenPin =  13;
const int led2RedPin =  2;   // 10 fails???   // the number of the LED pin
const int led2GreenPin =  11;

// Variables will change :
int ledRedState = LOW;             // ledState used to set the LED
int ledGreenState = LOW;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

void setup() {
  // set the digital pin as output:
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
   pinMode(led2RedPin, OUTPUT);
  pinMode(led2GreenPin, OUTPUT);
   
  //Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println(ledGreenState);
  
  Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
} // setup


void loop()
{
  int onTimeInMs;
  
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledRedPin, LOW);

    delay(1000);

/* Notes        Grön      Röd
 *  100 ms      Tydlig    Tydligt
 *  10 ms       Tydligt   Lite svagare
 *  1 ms        märkbar   märkbar
 *  0 ms        -         -
 */
    onTimeInMs = 500;
    
    digitalWrite(ledGreenPin, HIGH);
    delay(onTimeInMs); 
    digitalWrite(ledGreenPin, LOW);    

    delay(500);

    digitalWrite(ledRedPin, HIGH);
    delay(onTimeInMs); 
    digitalWrite(ledRedPin, LOW);
 

    delay(500);

    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledGreenPin, HIGH);
    delay(onTimeInMs); 
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledGreenPin, LOW);    

   delay(500);
 
    digitalWrite(led2GreenPin, HIGH);
    delay(onTimeInMs); 
    digitalWrite(led2GreenPin, LOW);

    delay(500);

    digitalWrite(led2RedPin, HIGH);
    delay(onTimeInMs); 
    digitalWrite(led2RedPin, LOW);


    delay(500);

    digitalWrite(led2RedPin, HIGH);
    digitalWrite(led2GreenPin, HIGH);
    delay(onTimeInMs); 
    digitalWrite(led2RedPin, LOW);
    digitalWrite(led2GreenPin, LOW);
           
}

