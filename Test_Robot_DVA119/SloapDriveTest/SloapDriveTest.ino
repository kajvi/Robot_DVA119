//Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

myMotors Motors; // Create motors object

#define C_MOTOR_LEFT_2 2
#define C_MOTOR_RIGHT_1 1

#define C_Speed_High 100
#define C_Speed_Low 80

#define C_FlatX 347
// #define C_TiltX TO DO!
#define C_FlatY 342
// #define C_TiltY TO DO!

long valAxisX = 0;
long valAxisY = 0;

int RSpeed = 50;
int LSpeed = 50;

float Xmean = 0.0;
float Ymean = 0.0;

mySensors Sensors; // create sensors object

void setup() 
{ 
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("button and IR sensor test.");
  
  Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
  delay(4000);
}

void loop() 
{
  valAxisX  = Sensors.readAccX();
  valAxisY = Sensors.readAccY();
  Serial.print(" ");Serial.print(valAxisX);
  Serial.print("\t");Serial.print(valAxisY);
  
  for (int i = 0; i <= 198; i++)
  {
    valAxisX = valAxisX + Sensors.readAccX();
    valAxisY = valAxisY + Sensors.readAccY();
    // Serial.print(" ");Serial.print(valAxisX);
    // Serial.print("\t");Serial.print(valAxisY);

  }
  Serial.print("\n");
  
  Xmean = valAxisX / 200;
  Ymean = valAxisY / 200;

  // Sets different speeds on each engine depending on right or left tilt.
  if (Ymean < C_FlatY)
  {
    // Right Tilt
    RSpeed = C_Speed_Low;
    LSpeed = C_Speed_High;
  } // if
  else
  {
    if (Ymean == C_FlatY)
    {
      // No Tilt
      RSpeed = C_Speed_High;
      LSpeed = C_Speed_High;      
    } // if
    else
    {
      // Left Tilt
      LSpeed = C_Speed_Low;
      RSpeed = C_Speed_High;
    } // else
  } // else
  
  Serial.print("");Serial.print(Xmean);
  Serial.print("\t");Serial.print(Ymean);
  Serial.print("\n");

  if(Xmean < (C_FlatX-1))
  {
    // Going Right -> Turn left.
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); 
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, RSpeed);
      delay(10);
  } // if
  else
  {
    if(Xmean <= (C_FlatX+1))
    {
      // Going Straight
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, LSpeed); 
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, RSpeed);
    } // if
    else
    {
      // Going Left -> Turn Right
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, LSpeed);
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
      delay(10);  
    } // else
  } // else
}
