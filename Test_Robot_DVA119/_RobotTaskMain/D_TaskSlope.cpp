// D_TaskSlope.cpp
//
// Handle task going on a slope
// 
// DVA119 -  Introduktion till robotik, ht'15, MdH
//
// Authors:
// Daniel Stenekap, Göran Forsström
//
// History
// 2015-10-16   Introduced
//
//
// ============================================================================

#include <arduino.h>  // To get millis() to be defined...
#include "Utilities.h"

#include "D_TaskSlope.h"
#include "IO.h"

#define C_THIS_TASK "Slope 2015-10-19"

#define C_RUN_LENGTH_IN_MS 3000

#define C_AVERAGING_COUNT 10

#define C_ACC_FLAT_X 347
// #define C_TILT_X TO DO!
#define C_ACC_FLAT_Y 342
// #define C_TILT_Y TO DO!

#define C_ACC_WINDOW_WIDTH_X 1
#define C_ACC_WINDOW_WIDTH_Y 2

#define C_SPEED_HIGH   90
#define C_SPEED_MIDDLE 70
#define C_SPEED_LOW    50

// Possible main states for the robot at this task
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsStartCalculateAccelerometeReferenceValues,
  rsCalculatingAccelerometeReferenceValues,
  rsFollowingFirstTape,
  rsRunningWithoutTape,
  rsStartCalcAccelerometerAveraging,
  rsCalcAccelerometerAveraging,
  rsStopped,
  rsFinished
};

enum accelerometerAverageCalcEnum {
  aaUnknown,
  aaInitial,
  aaPrepAveraging,
  aaAveragingAcc,
  aaFinishedAveragingAcc
} ;

// Possible sub states = directions for the robot at this task
/* not used???
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
}; */

static enum robotStateEnum     stat_RobotState     = rsInitial;


static int stat_AverageX;
static int stat_AverageY;
static int stat_ReferenceX;   // Indicates data for current slope
static int stat_ReferenceY;
static unsigned long stat_TargetTime;

// ============================================================================

void setupMotorControlFromDirectionCommand(directionCommandEnum i_directionCommandEnum, struct ioStruct* ptr_io)
{
  // Evaluate turning command to motor parameters.
  // =============================================
  
  switch (i_directionCommandEnum)
  {
    case dceTurnLeft:
      // Turn left!
      ptr_io->iosLeftEngine.direction = deBackward;
      ptr_io->iosLeftEngine.speed = C_SPEED_LOW;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MIDDLE;    
      break;
    
   case dceGoStraight:
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = C_SPEED_MIDDLE;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MIDDLE;
      break;
      
   case dceTurnRight:
      // Turn right!
      ptr_io->iosRightEngine.direction = deBackward;
      ptr_io->iosRightEngine.speed = C_SPEED_LOW;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = C_SPEED_MIDDLE;   
      break;

   case dcsStandStill:
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;   
      break;
      
    default:
      // ToDo:  No Change - what to do?
    break;   
  } // switch
} // setupMotorControlFromCommand

// ============================================================================

int followTapeAndReturnIsFinished(struct ioStruct* ptr_io)
{
  frontLCRsensorsEnum currLCRval;
  directionCommandEnum currDirCmd;
  int isFinished;

  // Evaluate tape sensors into direction command.
  // ============================================
  
  currLCRval = decodeFrontLCRsensors(ptr_io);
  currDirCmd = dceUnknown;
  isFinished = 0;
  switch (currLCRval) 
  {
    case dfs_LCR_DarkDarkDark:
      // Go forward  
      currDirCmd = dceGoStraight;
      break;

    case dfs_LCR_DarkDarkLight:
      // Go forward  
      currDirCmd = dceGoStraight;
      break;

    case dfs_LCR_DarkLightDark:
      // Go forward  ??? strange case ???
      currDirCmd = dceGoStraight;
      break;
            
    case dfs_LCR_DarkLightLight:
      // Black is found only on Left - turn left!
      currDirCmd = dceTurnLeft;
      break;

// -----

    case dfs_LCR_LightDarkDark:
      // Go forward  
      currDirCmd = dceGoStraight;
      break;

    case dfs_LCR_LightDarkLight:
      // Go forward  
       currDirCmd = dceGoStraight;
      break;
      
    case dfs_LCR_LightLightDark:
      // Black is found only on Right - turn right!
      currDirCmd = dceTurnRight;
      break;

   case dfs_LCR_LightLightLight:
      // Stop - all light means end of tape (?) - set up to continue straight on in next super-state...
      isFinished = 1;
      currDirCmd = dcsStandStill;
      break;
          
    default:
    // ToDo:  No Change - what to do?
    break;
  } // switch

  setupMotorControlFromDirectionCommand(currDirCmd, ptr_io);

  return isFinished;
  
} // followTapeAndReturnIsFinished

// ============================================================================

int followAccelerometerStraightAndReturnIsFinished(int i_AvgAccX, int i_AvgAccY, struct ioStruct* ptr_io)
{
  frontLCRsensorsEnum currLCRval;
  directionCommandEnum currDirCmd;
  int isFinished;

  // Evaluate Accelerometer averages into direction command.
  // ======================================================
  

  currDirCmd = dceUnknown;
  isFinished = 0;
  
  // Sets different speeds on each engine depending on right or left tilt.
  if (i_AvgAccX < (C_ACC_FLAT_Y - C_ACC_WINDOW_WIDTH_Y))
  {
    // Right Tilt
    ptr_io->iosRightEngine.speed = C_SPEED_LOW;
    ptr_io->iosLeftEngine.speed = C_SPEED_HIGH;
  } // if
  else
  {
    // Control according to Y-axis
    if (i_AvgAccY <= (C_ACC_FLAT_Y + C_ACC_WINDOW_WIDTH_Y))
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
  if (i_AvgAccX < (C_ACC_FLAT_X - C_ACC_WINDOW_WIDTH_X))
  {
      // Going Right -> Turn left.
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0; 
  } // if
  else
  {
    if(i_AvgAccX <= (C_ACC_FLAT_X + C_ACC_WINDOW_WIDTH_X))
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
  

  // Ccheck if we have reached the black tape again.
  currLCRval = decodeFrontLCRsensors(ptr_io);
  if (dfs_LCR_LightLightLight != currLCRval)
  {
    isFinished = 1;
  } // if

  return isFinished;
  
} // followAccelerometerStraightAndReturnIsFinished

// ============================================================================

// Input: i_DoNewCalc = 0: normal, = 1 means start new round
// Returns 0 = doing calc, returns 1 means finished average calc.

int calcAverageAccelerometerValuesXYIsFinished(int i_DoNewCalc, struct ioStruct* ptr_io)
{
  static enum accelerometerAverageCalcEnum stat_AccelerometerAverageCalcState = aaInitial;
  static int stat_AveragingCount;
  int retValue;

  if (i_DoNewCalc == 1)
  {
    stat_AccelerometerAverageCalcState = aaInitial;
  } // if
  
  retValue = 0;
  switch(stat_AccelerometerAverageCalcState)
  {
    case aaInitial:
      stat_AccelerometerAverageCalcState = aaPrepAveraging;
    break;

    case aaPrepAveraging:
      // Prepare averaging - motors stopped when you come here
      stat_AveragingCount = C_AVERAGING_COUNT;
      stat_AverageX = ptr_io->iosAccelerometerX; 
      stat_AverageY = ptr_io->iosAccelerometerY;
      stat_AccelerometerAverageCalcState = aaAveragingAcc;
      ptr_io->iosDelayMS = 10;    
    break;

    case aaAveragingAcc:
      // Do averaging of accel.values by counting down 10ms delays
      stat_AveragingCount--;
      if (stat_AveragingCount > 0)
      {
        stat_AverageX = (stat_AverageX + ptr_io->iosAccelerometerX) / 2;
        stat_AverageY = (stat_AverageY + ptr_io->iosAccelerometerY) / 2;
        ptr_io->iosDelayMS = 10;
      } // if
      else
      {
         stat_AccelerometerAverageCalcState = aaFinishedAveragingAcc;
         retValue = 1;
         ptr_io->iosDelayMS = 0;  // ToDo: onödig...
      } // else
    break;

    case aaFinishedAveragingAcc:
      retValue = 1;  // ToDo: onödigt?
    break;
        
    default:
      // ERROR ToDo
    break;    
  } // switch

  return retValue;
  
} // calcAverageAccelerometerValuesXYIsFinished

// ============================================================================

void taskSlope(struct ioStruct* ptr_io)
{
  switch(stat_RobotState)
  {
    case rsInitial:
      strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
      // Make sure motors stopped...
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;
      stat_RobotState = rsStartCalculateAccelerometeReferenceValues;
    break;

    case rsStartCalculateAccelerometeReferenceValues:
      strcpy (ptr_io->iosMessageChArr, "rsStartCalculateAccelerometeReferenceValues");
      calcAverageAccelerometerValuesXYIsFinished(1, ptr_io); // Start a new average round with 1st param = 1
      stat_RobotState = rsCalculatingAccelerometeReferenceValues;
    break;
    
    case rsCalculatingAccelerometeReferenceValues:
      strcpy (ptr_io->iosMessageChArr, "rsCalculateAccelerometeReferenceValues");
      if (calcAverageAccelerometerValuesXYIsFinished(0, ptr_io) != 0)
      {
        stat_RobotState = rsFollowingFirstTape;
        stat_TargetTime = millis() + C_RUN_LENGTH_IN_MS;  // Prepare time for next averaging
        ptr_io->iosDelayMS = 10;
      } // if    

      // Always update ref.values from calc average.
      stat_ReferenceX = stat_AverageX;
      stat_ReferenceY = stat_AverageY;
    break;
    
    case rsFollowingFirstTape:
      strcpy (ptr_io->iosMessageChArr, "rsFollowingFirstTape");
      if (followTapeAndReturnIsFinished(ptr_io) != 0)
      {
        // No more tape - stop motors and wait until start calc of average acc.
        stat_RobotState = rsStartCalcAccelerometerAveraging;
      } // if
    break;

    case rsStartCalcAccelerometerAveraging:
      strcpy (ptr_io->iosMessageChArr, "rsStartCalcAccelerometerAveraging");
      ptr_io->iosLeftEngine.speed = 0;
      ptr_io->iosRightEngine.speed = 0;
      ptr_io->iosDelayMS = 500; // ToDo stop-tid 
        
      calcAverageAccelerometerValuesXYIsFinished(1, ptr_io); // Start a new average round with 1st param = 1
      stat_RobotState = rsCalcAccelerometerAveraging;
    break;

    case rsCalcAccelerometerAveraging:
      strcpy (ptr_io->iosMessageChArr, "rsCalcAccelerometerAveraging");
      if (calcAverageAccelerometerValuesXYIsFinished(0, ptr_io) != 0)
      {
        stat_RobotState = rsRunningWithoutTape;
        stat_TargetTime = millis() + C_RUN_LENGTH_IN_MS;  // Prepare time for next averaging
        ptr_io->iosDelayMS = 10;
      } // if    
    break;
        
    case rsRunningWithoutTape:
      strcpy (ptr_io->iosMessageChArr, "rsRunningWithoutTape");
      if (followAccelerometerStraightAndReturnIsFinished(stat_AverageX, stat_AverageY, ptr_io) != 0)
      {
        // Reached target - stopp motors and report that task finisked
        ptr_io->iosLeftEngine.speed = 0;
        ptr_io->iosRightEngine.speed = 0;
        ptr_io->iosDelayMS = 10; 
        stat_RobotState = rsFinished;
        break;
      } // if
    
      // Check if time to do a new measurement of acc.
      if (millis() > stat_TargetTime)
      {
        // Stop motors.
        ptr_io->iosLeftEngine.speed = 0;
        ptr_io->iosRightEngine.speed = 0;  
        ptr_io->iosDelayMS = 500; // ToDo stop-tid ?
        stat_RobotState = rsCalcAccelerometerAveraging;
      } // if
      else
      {
        ptr_io->iosDelayMS = 10;
      } // else
    break;

    case rsStopped:
    
    break;

    case rsFinished:
       strcpy (ptr_io->iosMessageChArr, "rsFinished");
       ptr_io->iosCurrentTaskIsFinished = 1; 
    break;
    
    default:
    // ToDo:  No Change - what to do?
    break;
  } // switch


} // taskSlope


// ============================================================================

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



