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

#define C_THIS_TASK "Slope 2015-10-26"

#define C_RUN_LENGTH_IN_MS 3000

#define C_AVERAGING_COUNT 10

#define C_TILT_X 338
#define C_TILT_Y 361
#define C_TILT_X_ASKEW C_TILT_X - 15


#define C_WINDOW_X 0

// #define C_ACC_WINDOW_WIDTH_X 1
// #define C_ACC_WINDOW_WIDTH_Y 2
#define C_K_R 4
#define C_K_L 2

#define C_SPEED_HIGH   90
#define C_SPEED_MIDDLE 70
#define C_SPEED_MEDIUM 60
#define C_SPEED_LOW    40

// Possible main states for the robot at this task
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsInitial2,
  rsInitial3,
  rsGoOverTheEdgeAskew1,
  rsGoOverTheEdgeAskew2,
  rsFollowingFirstTape,
  rsRunningWithoutTape,
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
static int stat_Use3xWhiteAsForward = 0;
static int stat_RecommendedSpeed = 0;

static int stat_ValAxisX = C_TILT_X;
static int stat_ValAxisY = C_TILT_Y;

static int stat_TargetTimeAskew = 0;

// ============================================================================

void setupMotorControlFromDirectionCommand(directionCommandEnum i_directionCommandEnum, struct ioStruct* ptr_io)
{
  // Evaluate turning command to motor parameters.
  // =============================================

  // LEFT RIGHT CONFUSED!
  switch (i_directionCommandEnum)
  {
    case dceTurnLeft:
      // Turn left!
      ptr_io->iosLeftEngine.direction = deBackward;
      ptr_io->iosLeftEngine.speed = C_SPEED_HIGH + 20;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_LOW;    
      break;
    
   case dceGoStraight:
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = stat_RecommendedSpeed;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = stat_RecommendedSpeed;
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

   case dceGoBackwards:
      ptr_io->iosLeftEngine.direction = deBackward;
      ptr_io->iosLeftEngine.speed = C_SPEED_LOW;
      ptr_io->iosRightEngine.direction = deBackward;
      ptr_io->iosRightEngine.speed = C_SPEED_LOW;
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

  // Treat while-white-white as right turn if before target time
   if (dfs_LCR_LightLightLight == currLCRval)
   {
      if ( (millis() > stat_TargetTime) && (stat_Use3xWhiteAsForward == 1) )
      { 
        // Treat as right turn...
        currLCRval = dfs_LCR_LightLightDark;
      } // if
   } // if


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

int followAccelerometerStraightAndReturnIsFinished(struct ioStruct* ptr_io)
{
  frontLCRsensorsEnum currLCRval;
  directionCommandEnum currDirCmd;
  int isFinished;
  int diffX; 

  // Evaluate Accelerometer averages into direction command.
  // ======================================================
  

  currDirCmd = dceUnknown;
  isFinished = 0;
  
  stat_ValAxisX = (stat_ValAxisX + ptr_io->iosAccelerometerX) / 2;
  stat_ValAxisY = (stat_ValAxisY + ptr_io->iosAccelerometerY) / 2;
  
  diffX = stat_ValAxisX - C_TILT_X; 


   if (diffX < -C_WINDOW_X)
   {
     // ptr_io->iosRightLedGreen = HIGH;
     // ptr_io->iosRightLedRed = LOW;
      
      // Om detta sker så har vi Uppåtlut och Höger motor behöver Kräm
      int leftSpeed = C_SPEED_MEDIUM + diffX*C_K_L;
      
      
      if (leftSpeed < 40)
      {
      //  ptr_io->iosRightLedGreen = HIGH;
      //  ptr_io->iosRightLedRed =  HIGH;
        leftSpeed = C_SPEED_LOW;
        ptr_io->iosLeftEngine.direction = deBackward;
        ptr_io->iosLeftEngine.speed = leftSpeed;
      } // if
      else
      {
        ptr_io->iosLeftEngine.direction = deForward;
        ptr_io->iosLeftEngine.speed = leftSpeed;
      } // else
    
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MEDIUM*1.08 - diffX*C_K_R;
   
     } // if
    else
    if (diffX < C_WINDOW_X)
    {
    //  ptr_io->iosRightLedGreen = LOW;
    //  ptr_io->iosRightLedRed =  LOW;
    } // if
    else
    {
    //  ptr_io->iosRightLedGreen = LOW;
    //  ptr_io->iosRightLedRed =  HIGH;

      
      // Vi har Nedåtlut och Vänster Motor behöver kräm!
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = C_SPEED_MEDIUM + diffX*C_K_L;      
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MEDIUM*1.08 - diffX*C_K_R;
    } // else

    ptr_io->iosDelayMS = 10;
 
  // Check if we have reached the black tape again.
  // Black is found on all: Different actions depending on state and direction.
  if ( (ptr_io->iosReflFrontRight_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontLeft_2 == C_DARK_1) )
  {
    if ( (ptr_io->iosReflAnalog_3 >= C_ANALOG_DARK))
    {
     isFinished = 1;
    } // if
  } // if


  return isFinished;
  
} // followAccelerometerStraightAndReturnIsFinished

/* *********************************************

   valAxisX = (valAxisX + Sensors.readAccX()) / 2;
    valAxisY = (valAxisY + Sensors.readAccY()) / 2;
  
    int diffX = valAxisX - C_TILT_X; 
        
    if (diffX < -C_WINDOW_X)
    {
      digitalWrite(LedGreenPin, HIGH);
      digitalWrite(LedRedPin, LOW);
      // Om detta sker så har vi Uppåtlut och Höger motor behöver Kräm
      int leftSpeed = C_SPEED_MEDIUM + diffX*C_K_L;
      
      
      if (leftSpeed < 40)
      {
        digitalWrite(LedGreenPin, HIGH);
        digitalWrite(LedRedPin, HIGH);
        leftSpeed = C_SPEED_LOW;
        Motors.runMotor(C_MOTOR_LEFT_2, BACKWARD, leftSpeed);
      }
      else
      {
        Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, leftSpeed);
      }
    
      //Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, leftSpeed);  
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_SPEED_MEDIUM*1.08 - diffX*C_K_R);
    }
    else
    if (diffX < C_WINDOW_X)
    {
      digitalWrite(LedGreenPin, LOW);
      digitalWrite(LedRedPin, LOW);    
    }
    else
    {
      digitalWrite(LedGreenPin, LOW);
      digitalWrite(LedRedPin, HIGH);
      
      // Vi har Nedåtlut och Vänster Motor behöver kräm!
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, C_SPEED_MEDIUM + diffX*C_K_L);
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_SPEED_MEDIUM*1.08 - diffX*C_K_R);
    }
  
    delay(10);


// *********************************************/



// ============================================================================

int followAccelerometerAskewAndReturnIsFinished(struct ioStruct* ptr_io)
{
  frontLCRsensorsEnum currLCRval;
  directionCommandEnum currDirCmd;
  int isFinished;
  int diffX; 

  // Evaluate Accelerometer averages into direction command.
  // ======================================================
  

  currDirCmd = dceUnknown;
  isFinished = 0;
  
  stat_ValAxisX = (stat_ValAxisX + ptr_io->iosAccelerometerX) / 2;
  stat_ValAxisY = (stat_ValAxisY + ptr_io->iosAccelerometerY) / 2;
  
  diffX = stat_ValAxisX - C_TILT_X_ASKEW; 


   if (diffX < -C_WINDOW_X)
   {
     // ptr_io->iosRightLedGreen = HIGH;
     // ptr_io->iosRightLedRed = LOW;
      
      // Om detta sker så har vi Uppåtlut och Höger motor behöver Kräm
      int leftSpeed = C_SPEED_MEDIUM + diffX*C_K_L;
      
      
      if (leftSpeed < 40)
      {
      //  ptr_io->iosRightLedGreen = HIGH;
      //  ptr_io->iosRightLedRed =  HIGH;
        leftSpeed = C_SPEED_LOW;
        ptr_io->iosLeftEngine.direction = deBackward;
        ptr_io->iosLeftEngine.speed = leftSpeed;
      } // if
      else
      {
        ptr_io->iosLeftEngine.direction = deForward;
        ptr_io->iosLeftEngine.speed = leftSpeed;
      } // else
    
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MEDIUM*1.08 - diffX*C_K_R;
   
     } // if
    else
    if (diffX < C_WINDOW_X)
    {
    //  ptr_io->iosRightLedGreen = LOW;
    //  ptr_io->iosRightLedRed =  LOW;
    } // if
    else
    {
    //  ptr_io->iosRightLedGreen = LOW;
    //  ptr_io->iosRightLedRed =  HIGH;

      
      // Vi har Nedåtlut och Vänster Motor behöver kräm!
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = C_SPEED_MEDIUM + diffX*C_K_L;      
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MEDIUM*1.08 - diffX*C_K_R;
    } // else

    ptr_io->iosDelayMS = 10;

    // Block detect of tape for some time as sesors in midair is confused with tape...
    if (millis() > stat_TargetTimeAskew)
    {
      

 
      // Check if we have reached the black tape again.
      // Black is found on all: Different actions depending on state and direction.
      if ( (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontLeft_2 == C_DARK_1) )
      {
        if ( (ptr_io->iosReflAnalog_3 < C_ANALOG_DARK))
        {
          isFinished = 1;
        } // if
      } // if

    }
    
  return isFinished;
  
} // followAccelerometerAskewAndReturnIsFinished


// ============================================================================

void taskSlope(struct ioStruct* ptr_io)
{
  ptr_io->iosMessageInteger = ptr_io->iosAccelerometerY;
 // ptr_io->iosRightLedRed = ptr_io->iosReflFrontCenter_1;
  switch(stat_RobotState)
  {
    case rsInitial:
      strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
      // Make start zag to left
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = C_SPEED_MEDIUM;
      stat_RobotState = rsInitial2;
      
      ptr_io->iosDelayMS = 400;
     break;

    case rsInitial2:
      strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
      // Make start zag to left
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0;
      stat_RobotState = rsInitial3;
      
      ptr_io->iosDelayMS = 4000;
     break;

case rsInitial3:
      strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
      // Make start zag to left
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = C_SPEED_MIDDLE;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = C_SPEED_MIDDLE;
      stat_RobotState = rsGoOverTheEdgeAskew1;
      stat_TargetTimeAskew = millis() + 700;
      
      ptr_io->iosDelayMS = 200;
     break;


    case rsGoOverTheEdgeAskew1:
ptr_io->iosLeftLedRed = HIGH;    
ptr_io->iosLeftLedGreen = LOW;      
      strcpy (ptr_io->iosMessageChArr, "rsGoOverTheEdgeAskew");
      stat_RobotState = rsGoOverTheEdgeAskew2;
      if (followAccelerometerAskewAndReturnIsFinished(ptr_io) != 0)
      {
        // Reached tape - follow it
        stat_RobotState = rsFollowingFirstTape;
      } // if
      else
      {
        ptr_io->iosDelayMS = 100;
      }
     break;


    case rsGoOverTheEdgeAskew2:
ptr_io->iosLeftLedRed = HIGH;    
ptr_io->iosLeftLedGreen = LOW;      
      strcpy (ptr_io->iosMessageChArr, "rsGoOverTheEdgeAskew");
      stat_RobotState = rsGoOverTheEdgeAskew1;
      ptr_io->iosRightEngine.speed = 0;
      ptr_io->iosLeftEngine.speed = 0;
      
      ptr_io->iosDelayMS = 50;
     
      break;
      
    case rsFollowingFirstTape:
      ptr_io->iosRightLedRed = HIGH;
      ptr_io->iosRightLedGreen = LOW;
      strcpy (ptr_io->iosMessageChArr, "rsFollowingFirstTape");
      stat_RecommendedSpeed = C_SPEED_MIDDLE;
      if (followTapeAndReturnIsFinished(ptr_io) != 0)
      {
        // No more tape - just go on
        stat_RobotState = rsRunningWithoutTape;
    
      } // if
    break;
    

    case rsRunningWithoutTape:
      strcpy (ptr_io->iosMessageChArr, "rsRunningWithoutTape");
      ptr_io->iosRightLedGreen = HIGH;
      ptr_io->iosRightLedRed = LOW;
      if (followAccelerometerStraightAndReturnIsFinished(ptr_io) != 0)
      {
        // Reached target - stop motors and report that task finisked
        ptr_io->iosLeftEngine.speed = 0;
        ptr_io->iosRightEngine.speed = 0;
        ptr_io->iosDelayMS = 10; 
        stat_RobotState = rsFinished;
        break;
      } // if
    
    break;

    case rsStopped:
     strcpy (ptr_io->iosMessageChArr, "rsStopped");
    break;

    case rsFinished:
       strcpy (ptr_io->iosMessageChArr, "rsFinished");
       ptr_io->iosLeftEngine.speed = 0;
       ptr_io->iosRightEngine.speed = 0;       
       ptr_io->iosCurrentTaskIsFinished = 1; 
    break;
    
    default:
    // ToDo:  No Change - what to do?
    break;
  } // switch


} // taskSlope

// ============================================================================

