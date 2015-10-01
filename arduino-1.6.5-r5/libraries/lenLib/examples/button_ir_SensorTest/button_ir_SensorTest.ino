#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

int valB0 = 0;
int valB1 = 0;
int valIR = 0;

mySensors Sensors; // create sensors object

void setup() 
{ 
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("button and IR sensor test.");
  
  Sensors.beginSensors(); // start sensors
}

void loop() 
{
  valB0  = Sensors.readButton0();
  valB1 = Sensors.readButton1();
  valIR = Sensors.readIR0();

  Serial.print("Button 0: ");Serial.print(valB0);
  Serial.print("\tButton 1: ");Serial.print(valB1);
  Serial.print("\tIR sensor: ");Serial.print(valIR);
  Serial.print("\n");
}
