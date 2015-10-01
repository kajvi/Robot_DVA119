// Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

#define C_THIS_VERSION "DVA119 Robotics Intro: Slope Test v. 2015-10-01/GF"

myMotors Motors; // Create motors object

#define C_MOTOR_RIGHT_1 1
#define C_MOTOR_LEFT_2 2


#define C_SPEED_HIGH 100
#define C_SPEED_MEDIUM 80
#define C_SPEED_LOW 60

#define C_ACC_FLAT_X 347
// #define C_TILT_X TO DO!
#define C_ACC_FLAT_Y 342
// #define C_TILT_Y TO DO!

#define C_ACC_WINDOW_WIDTH_X 1
#define C_ACC_WINDOW_WIDTH_Y 2

#define C_AVERAGE_COUNT 10

#define C_FLOAT_WIDTH 6



mySensors Sensors; // create sensors object

// ============================================================================

String adjustStr(String i_Str, int i_Width)
{
  // i_Width < 0 - add spaces after i_Str - Left Justify
  // i_Width > 0 - add spaces before i_Str - Right Justify
  // No if i_Str longer that abs(i_Width) - no change!
  
  String resultStr = i_Str;
  
  if (i_Width > 0)
  {
    // Add spaces before
    while (resultStr.length() < abs(i_Width))
    {
      resultStr = ' ' + resultStr;
    } // while
  } // if
  else
  {
    // Add spaces after
    while (resultStr.length() < abs(i_Width))
    {
      resultStr = resultStr + ' ';
    } // while    
  } // else
  
  return resultStr;
   
} // adjustStr

// ============================================================================

void setup() 
{ 
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println(C_THIS_VERSION);
  
  Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
  delay(4000);
} // setup

// ============================================================================

void loop() 
{
  long valAxisX = 0;
  long valAxisY = 0;

  int rightMotorSpeed = 50;
  int leftMotorSpeed = 50;

  float averageX = 0.0;
  float averageY = 0.0;

  // Experiment: stop while reading accelerometer sensor
  Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
  Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); 
  delay(100);
  
  valAxisX  = Sensors.readAccX();
  valAxisY = Sensors.readAccY();
  Serial.print("Raw:      X: " + adjustStr(String(valAxisX, 2), C_FLOAT_WIDTH) + "\t Y: " + adjustStr(String(valAxisX, 2), C_FLOAT_WIDTH) + "\n");
    
  averageX = valAxisX;
  averageY = valAxisY;
  for (int i = 1; i <= C_AVERAGE_COUNT; i++)
  {
    delay(10);
    averageX = (averageX + Sensors.readAccX()) / 2.0;
    averageY = (averageY + Sensors.readAccY()) / 2.0;
  } // for i
  Serial.print("Average Count" + String(C_AVERAGE_COUNT) + "\n");
  Serial.print("Average:  X: " + adjustStr(String(averageX, 2), C_FLOAT_WIDTH) + "\t Y: " + adjustStr(String(averageY, 2), C_FLOAT_WIDTH) + "\n");


  // Sets different speeds on each engine depending on right or left tilt.
  if (averageY < C_ACC_FLAT_Y - C_ACC_WINDOW_WIDTH_Y)
  {
    // Right Tilt
    rightMotorSpeed = C_SPEED_LOW;
    leftMotorSpeed = C_SPEED_HIGH;
  } // if
  else
  {
    if (averageY <= (C_ACC_FLAT_Y + C_ACC_WINDOW_WIDTH_Y))
    {
      // No Tilt
      rightMotorSpeed = C_SPEED_HIGH;
      leftMotorSpeed = C_SPEED_HIGH;      
    } // if
    else
    {
      // Left Tilt
      leftMotorSpeed = C_SPEED_LOW;
      rightMotorSpeed = C_SPEED_HIGH;
    } // else
  } // else
  

  // Control according to X-value
  if (averageX < (C_ACC_FLAT_X - C_ACC_WINDOW_WIDTH_X))
  {
      // Going Right -> Turn left.
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, rightMotorSpeed);
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); 
  } // if
  else
  {
    if(averageX <= (C_ACC_FLAT_X + C_ACC_WINDOW_WIDTH_X))
    {
      // Going Straight
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, rightMotorSpeed);
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, leftMotorSpeed); 
    } // if
    else
    {
      // Going Left -> Turn Right
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, leftMotorSpeed);
    } // else
  } // else

  delay(100);  

} // loop

// ============================================================================

