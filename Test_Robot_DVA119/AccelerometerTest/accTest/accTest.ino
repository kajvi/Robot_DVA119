//Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

myMotors Motors; // Create motors object

#define C_MOTOR_LEFT_2 2
#define C_MOTOR_RIGHT_1 1

int valAxisX = 0;
int valAxisY = 0;

mySensors Sensors; // create sensors object

void setup() 
{ 
  //Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println("button and IR sensor test.");
  
  Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
  delay(4000);
}

void loop() 
{
  valAxisX  = Sensors.readAccX();
  valAxisY = Sensors.readAccY();

  //Serial.print("");Serial.print(valAxisX);
  //Serial.print("\t");Serial.print(valAxisY);
  //Serial.print("\n");

   Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); // Motor 1 should run forward with speed x
    Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 60);
/*
  if(valAxisX <=345)
  {
    // Going up and right
    Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 0); // Motor 1 should run forward with speed x
    Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 60);
    
  } // if
  else
  {
    if(valAxisX <= 349)
    {
      // Going straight
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 70); // Motor 1 should run forward with speed x
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 90);
    } // if
    else
    {
      // going down and left
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 60); // Motor 1 should run forward with speed x
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 0);
    } // else
  } // else
*/
}
