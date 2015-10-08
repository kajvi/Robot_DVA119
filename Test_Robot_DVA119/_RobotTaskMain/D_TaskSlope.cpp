#include <string.h>
#include <arduino.h>  // To get millis() to be defined...

#include "D_TaskSlope.h"
#include "IO.h"

#define C_THIS_TASK "Slope 2015-10-08"

#define C_AVERAGING_COUNT 10

#define C_ACC_FLAT_X 347
// #define C_TILT_X TO DO!
#define C_ACC_FLAT_Y 342
// #define C_TILT_Y TO DO!

#define C_ACC_WINDOW_WIDTH_X 1
#define C_ACC_WINDOW_WIDTH_Y 2

#define C_SPEED_HIGH 100
#define C_SPEED_MEDIUM 80
#define C_SPEED_LOW 60

 
// Possible main states for the robot at this task
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsRunning,
  rsPrepAveraging,
  rsAveragingAcc,
  rsFinishedAveragingAcc,
  rsStopped,
  rsFinnished
};

// Possible sub states = directions for the robot at this task
enum robotDirectionEnum {
  rdUnknown,
  rdInitial,
  rdStopped,
  rdStraight,
  rdLeft,
  rdSharpLeft,
  rdRight,
  rdSharpRight,
  rdBackwards
};

static enum robotStateEnum     stat_RobotState     = rsInitial;
static enum robotDirectionEnum stat_RobotDirection = rdInitial;

static int stat_AveragingCount = 0;
static int stat_AverageX;
static int stat_AverageY;
static unsigned long stat_TargetTime;

// ============================================================================================

void taskSlope(struct ioStruct* ptr_io)
{
  switch(stat_RobotState)
  {
    case rsInitial:
      // Stop motors...
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;
      stat_RobotDirection = rdStopped;
      stat_RobotState = rsPrepAveraging;
      ptr_io->iosDelayMS = 50;
    break;

    case rsPrepAveraging:
      // Prepare averaging
      stat_AveragingCount = C_AVERAGING_COUNT;
      stat_AverageX = ptr_io->iosAccelerometerX; 
      stat_AverageY = ptr_io->iosAccelerometerY;
      stat_RobotState = rsAveragingAcc;
      ptr_io->iosDelayMS = 10;
    break;

    case rsAveragingAcc:
      // Do averaging of accel.values
      stat_AveragingCount--;
      if (stat_AveragingCount > 0)
      {
        stat_AverageX = (stat_AverageX << 2 + ptr_io->iosAccelerometerX << 2) / 4; // ToDo! *2 to get space for averaging?
        stat_AverageY = (stat_AverageY << 2 + ptr_io->iosAccelerometerY << 2) / 4;
        ptr_io->iosDelayMS = 10;
      } // if
      else
      {
         stat_RobotState = rsFinishedAveragingAcc;
         ptr_io->iosDelayMS = 0; // onödig...
      } // else
    break;

    case rsFinishedAveragingAcc:
      stat_RobotState = rsRunning;
      stat_TargetTime = millis();
    break;


    case rsRunning:
      // Sets different speeds on each engine depending on right or left tilt.
      if (stat_AverageY < C_ACC_FLAT_Y - C_ACC_WINDOW_WIDTH_Y)
      {
        // Right Tilt
        ptr_io->iosRightEngine.speed = C_SPEED_LOW;
        ptr_io->iosLeftEngine.speed = C_SPEED_HIGH;
      } // if
      else
      {
        if (stat_AverageY <= (C_ACC_FLAT_Y + C_ACC_WINDOW_WIDTH_Y))
        {
          // No Tilt
          ptr_io->iosRightEngine.speed = C_SPEED_HIGH;
          ptr_io->iosLeftEngine.speed = C_SPEED_HIGH;      
        } // if
        else
        {
          // Left Tilt
          ptr_io->iosLeftEngine.speed = C_SPEED_LOW;
          ptr_io->iosRightEngine.speed = C_SPEED_HIGH;
        } // else
      } // else


  // Control according to X-value
  if (stat_AverageX < (C_ACC_FLAT_X - C_ACC_WINDOW_WIDTH_X))
  {
      // Going Right -> Turn left.
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0; 
  } // if
  else
  {
    if(stat_AverageX <= (C_ACC_FLAT_X + C_ACC_WINDOW_WIDTH_X))
    {
      // Going Straight
      ptr_io->iosRightEngine.direction = deForward; 
      ptr_io->iosLeftEngine.direction = deForward;
    } // if
    else
    {
      // Going Left -> Turn Right
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;
      ptr_io->iosLeftEngine.direction = deForward; 
    } // else
  } // else
  
    break;

    case rsStopped:
    
    break;

    case rsFinnished:
    
    break;
    
    default:
    // ToDo:  No Change - what to do?
    break;
  } // switch


} // taskSlope

// ============================================================================================

/*

  // Experiment: stop while reading accelerometer sensor
  Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
  Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); 
  delay(100);
  
  valAxisX  = Sensors.readAccX();
  valAxisY = Sensors.readAccY();
  Serial.print("Raw:      X: " + adjustStr(String(valAxisX, 2), C_FLOAT_WIDTH) + "\t Y: " + adjustStr(String(valAxisX, 2), C_FLOAT_WIDTH) + "\n");
    
  stat_AverageX = valAxisX;
  stat_AverageY = valAxisY;
  for (int i = 1; i <= C_AVERAGE_COUNT; i++)
  {
    delay(10);
    stat_AverageX = (stat_AverageX + Sensors.readAccX()) / 2.0;
    stat_AverageY = (stat_AverageY + Sensors.readAccY()) / 2.0;
  } // for i
  Serial.print("Average Count" + String(C_AVERAGE_COUNT) + "\n");
  Serial.print("Average:  X: " + adjustStr(String(stat_AverageX, 2), C_FLOAT_WIDTH) + "\t Y: " + adjustStr(String(stat_AverageY, 2), C_FLOAT_WIDTH) + "\n");


  // Sets different speeds on each engine depending on right or left tilt.
  if (stat_AverageY < C_ACC_FLAT_Y - C_ACC_WINDOW_WIDTH_Y)
  {
    // Right Tilt
    rightMotorSpeed = C_SPEED_LOW;
    leftMotorSpeed = C_SPEED_HIGH;
  } // if
  else
  {
    if (stat_AverageY <= (C_ACC_FLAT_Y + C_ACC_WINDOW_WIDTH_Y))
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
  if (stat_AverageX < (C_ACC_FLAT_X - C_ACC_WINDOW_WIDTH_X))
  {
      // Going Right -> Turn left.
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, rightMotorSpeed);
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); 
  } // if
  else
  {
    if(stat_AverageX <= (C_ACC_FLAT_X + C_ACC_WINDOW_WIDTH_X))
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
*/
// ============================================================================

