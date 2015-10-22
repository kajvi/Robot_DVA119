#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

int valAxisX = 0;
int valAxisY = 0;

mySensors Sensors; // create sensors object

void setup() 
{ 
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("button and IR sensor test.");
  
  Sensors.beginSensors(); // start sensors
}

void loop() 
{
  valAxisX  = Sensors.readAccX();
  valAxisY = Sensors.readAccY();

  Serial.print("Axis X: ");Serial.print(valAxisX);
  Serial.print("\tAxis Y: ");Serial.print(valAxisY);
  Serial.print("\n");
}
