// 75-50 ger ca 1m diameter
// 100-50 ger ca 0,5m diameter


#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

#define C_SPEED_LOW     80
#define C_SPEED_HIGH    80

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

#define C_SENSORDATA 1

mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

void setup() {
  // put your setup code here, to run once:
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors
}

void loop() {
  int left_speed = C_SPEED_HIGH;
  float factor = 2.0; 
  
  Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 0); 
  Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 0); 
  delay(4000);
  
  while (C_SENSORDATA == 1)
  {
    left_speed = left_speed * factor;
    Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, left_speed); 
    Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_HIGH); 
    delay(700);
    
    factor = factor * 0.95;
    
    if (factor <= 1.0)
    {
      delay(700);
      break;  
    } // while
  }
}
