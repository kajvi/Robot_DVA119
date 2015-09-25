#include <Adafruit_MotorShield.h>
#include <lenlib.h>
#include <Wire.h>
#include <Servo.h>

myMotors Motors; // Create motors object

void setup() 
{
  Motors.beginMotors(); // Start up motors 
  delay(4000);
}

void loop() 
{
    Motors.runMotor(1, FORWARD, 70); // Motor 1 should run forward with speed x
    Motors.runMotor(2, FORWARD, 70);
  /*
  for(int x = 80; x <= 180; x++)// Increase angle of servo slowly
  {
    Motors.angleServo(x); // Set servo angle
    delay(10);// Delay for 10ms
  }
  for(int x = 180; x >= 80; x--)// Decrease angle of servo slowly
  {
    Motors.angleServo(x); // Set servo angle
    delay(10);  
  }
  
  for(int x = 1; x <= 255; x++)// Increase backward speed of motors slowly
  {
    Motors.runMotor(1, BACKWARD, x);
    Motors.runMotor(2, BACKWARD, x);
    Motors.runMotor(3, BACKWARD, x);
    Motors.runMotor(4, BACKWARD, x);
    delay(5);  
  }
  for(int x = 255; x >= 1; x--)// Decrease backward speed of motors slowly
  {
    Motors.runMotor(1, BACKWARD, x);
    Motors.runMotor(2, BACKWARD, x);
    Motors.runMotor(3, BACKWARD, x);
    Motors.runMotor(4, BACKWARD, x);
    delay(5);  
  }
*/
}
