static int #include <string.h>

#include "D_TaskSlope.h"
#include "IO.h"

#define C_THIS_TASK "Slope 2015-10-06"

#define C_AVERAGING_COUNT 10

// Sets state of robot: what is State is the robot in.
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

enum robotDirectionEnum {
  rdUnknown,
  rdInitial,
  rdStopped,
  rdStraight,
  rdLeft,
  rdSharpLeft,
  rdRight,
  rdSharpRight,
  rdBackwards,
};

static enum robotStateEnum stat_RobotState = rsInitial;
static enum robotDirectionEnum stat_RobotDirection = rdInitial;

static int averagingCount = 0;
static float averageX;
static float averageY;

// ============================================================================================

void taskSlope(struct ioStruct* ptr_io)
{
  switch(stat_RobotState)
  {
    case rsInitial:
      // Stop motors...
      ptr_io->iosLeftEngine.direction = FORWARD;
      ptr_io->iosLeftEngine.speed = 0;
      ptr_io->iosRightEngine.direction = FORWARD;
      ptr_io->iosRightEngine.speed = 0;
      robotDirectionEnum = rdStopped;
      robotStateEnum = rsPrepAveraging;
      ptr_io->iosDelayMS = 50;
    break;

    case rsPrepAveraging:
      // Prepare averaging
      averagingCount = C_AVERAGING_COUNT;
      averageX = ptr_io->iosAccelerometerX; 
      averageY = ptr_io->iosAccelerometerY;
      robotStateEnum = rsAveragingAcc;
      ptr_io->iosDelayMS = 10;
    break;

    case rsAveragingAcc:
      // Do averaging of accel.values
      averagingCount--;
      if (averagingCount > 0)
      {
        averageX = (averageX << 2 + ptr_io->iosAccelerometerX << 2) / 4; // ToDo! *2 to get space for averaging?
        averageY = (averageY << 2 + ptr_io->iosAccelerometerY << 2) / 4;
        ptr_io->iosDelayMS = 10;
      } // if
      else
      {
         robotStateEnum = rsFinishedAveragingAcc
         ptr_io->iosDelayMS = 0; // onödig...
      } // else
    break;

    case rsFinishedAveragingAcc:
    
    break;


    case rsRunning:
    
    break;

    case rsStopped:
    
    break;

    case rsFinnished:
    
    break;
    
    default:
    // No Change - what to do?
    break;
  } // switch


} // taskSlope

// ============================================================================================


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

