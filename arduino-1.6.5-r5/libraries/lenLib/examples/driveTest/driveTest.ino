#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

int R0Val = 0;

mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

void setup() 
{ 
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors
}

void loop() 
{
  
  R0Val  = Sensors.readReflect0(); // Read digital value of reflect sensor 0

  if(R0Val == 1)
  {
      Motors.runMotor(1, FORWARD, 0); // Run motor 1 & 2 forward at 0 velocity(Stop)
      Motors.runMotor(2, FORWARD, 0);
  }
  else
  {
      Motors.runMotor(1, FORWARD, 100); // Run motor 1 & 2 forward at 100 velocity
      Motors.runMotor(2, FORWARD, 100);
  }
}
