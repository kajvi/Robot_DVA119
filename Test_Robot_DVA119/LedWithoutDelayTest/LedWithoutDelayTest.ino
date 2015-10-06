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
const int ledRedPin =  12;      // the number of the LED pin
const int ledGreenPin =  13;

// Variables will change :
int ledRedState = LOW;             // ledState used to set the LED
int ledGreenState = LOW;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 1000;           // interval at which to blink (milliseconds)
unsigned long time;
unsigned long targetTime = 0;
int checkPoint = 0;


void setup() {
  // set the digital pin as output:
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  
  Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println(C_THIS_VERSION);
  
  //Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
} // setup


void loop()
{
  //Serial.print("Time: ");
  time = millis();
  if (targetTime == 0)
  {
    targetTime = time + 1500;
  }
    
  //prints time since program started
  //Serial.println(time);
  
  // if the redLED is off turn it on and vice-versa:
  if (ledRedState == LOW && checkPoint == 0 && time >= targetTime)
  {
    ledRedState = HIGH;
    targetTime = time + 1500;
    digitalWrite(ledRedPin, ledRedState);
  }

  if (ledRedState == HIGH && time >= targetTime && checkPoint == 1)
  {
    ledRedState = LOW;
    targetTime = time + 1500;
    digitalWrite(ledRedPin, ledRedState);
  }
  // set the LED with the ledState of the variable:
  
  //delay(1500);
    
  // if the greenLED is off turn it on and vice-versa:
  if (ledGreenState == LOW && time >= targetTime)
  {
    ledGreenState = HIGH;
    checkPoint = 1;
    targetTime = time + 1500;
    digitalWrite(ledGreenPin, ledGreenState);
  }
  else
  {
    if (ledGreenState == HIGH && time >= targetTime && checkPoint == 1)
    {
      ledGreenState = LOW;
      checkPoint = 0;
      digitalWrite(ledGreenPin, ledGreenState);
      targetTime = time + 1500;
    }
  }

  // set the LED with the ledState of the variable:
  
  //delay(1500);
}


