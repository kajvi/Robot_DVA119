#include <string.h>
#include <math.h>
#include "IO.h"

#include "A_TaskLineFollow.h"

#define C_THIS_TASK "LineFollow 2015-10-08"

// Local speed triming for engine stream.
#define C_SPEED_HIGH 120
#define C_SPEED_MEDIUM 70
#define C_SPEED_LOW 50

#define C_STATE_COUNT_LIMIT 200
#define C_STATE_COUNT_DIVIDER 4

// Sets state of robot: what is State is the robot in.
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsForward,
  rsLeft,
  rsRight,
  rsBackward,
  rsFinnished,
};

/*
// Keeps track what state the robot comes from
enum lastStateEnum {
  lsUnknown,
  lsInitial,
  lsStraight,
  lsLeft,
  lsRight,
  lsBackward,
  lsFinnished
};
*/

// Variable that count the number of times a state has been visited:
long stateCount = 0;
// Variable that sets an entry condition to switch states.
int stateEntry = 0;


// Variable Speed depending on state count.
int stateSpeedRight = C_SPEED_MEDIUM;
int stateSpeedLeft = C_SPEED_MEDIUM;

static enum robotStateEnum stat_RobotState = rsInitial;
static enum robotStateEnum stat_LastState = rsInitial;
static enum actionEnum stat_Action = aeInitial;

// ============================================================================================


void taskLineFollow(struct ioStruct* ptr_io)
{
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  
  ptr_io->iosCurrentTaskIsFinished = 1;  // STUB test
  
  // ptr_io->iosDelayMS = 2000;

  // Evaluate sensor data
  // ====================

// black is found on "Middle/Left" or only "Left" - Turn left!
  if (( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
      || 
      ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )) 
  {
    stat_RobotState = rsLeft;
  } // if


  // black is found only on "Middle/Right" or only "right" - turn right!
  if (( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
      ||
      ( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )) 
  {
    stat_RobotState = rsRight;
  } // if


  // Black is found in "middle" or "left/right but not middle" - go forward!
  if (( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0)) 
      ||
      ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )) 
  {
    stat_RobotState = rsForward;
  } // if


  // No black is found - Back up alittle!
  if ( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    stat_RobotState = rsBackward;
  } // if


  // Black is found on all: Different actions depending on state and direction.
  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    stat_RobotState = rsFinnished;
  }

// End of Sensor Data evaluation.
// ================================================================================================================================================================================================================== //

  // Limit State Counter.
  if (stateCount > C_STATE_COUNT_LIMIT)
  {
    stateCount = C_STATE_COUNT_LIMIT;
  }

  switch (stat_RobotState)
  {
    // Initial Value : Drive Forward
    case rsInitial:
    {
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = 0;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = 0;
      
      stat_RobotState = rsForward;
      stat_LastState = rsInitial;
      
      break;
    }

    // Action suggestion forward
    case rsForward:
    {
      if (stat_LastState != rsForward)
      {
        stateCount = lround(stateCount/3);
      }

      // Direction and speed of Right Engine
      stateSpeedRight = (int) (stateSpeedRight + stateCount)/C_STATE_COUNT_DIVIDER;
      stateSpeedLeft = stateSpeedRight;
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = stateSpeedRight;
      ptr_io->iosLeftEngine.direction = deForward;
      ptr_io->iosLeftEngine.speed = stateSpeedLeft;

      stat_LastState = rsForward;
      stateCount++;
      
      break;
    }

    // Algorithm for left turn
    case rsLeft:
    {
      // Check last state: if(forward) save half of stateCount : else reset stateCount
      if (stat_LastState != rsLeft)
      { 
        stateCount = 0;
      }

      // Set stateSpeed in increments of C_STATE_COUNT_DIVIDER entrys to THIS STATE 
      stateSpeedRight = stateSpeedRight + (stateCount/C_STATE_COUNT_DIVIDER);

      // 
      ptr_io->iosRightEngine.direction = deForward;
      ptr_io->iosRightEngine.speed = stateSpeedRight;

      // Set stateSpeed in increments of C_STATE_COUNT_DIVIDER entrys to THIS STATE
      stateSpeedLeft = stateSpeedLeft - 5 * (stateCount/C_STATE_COUNT_DIVIDER);

      //Check sign of StateSpeedLeft and 
      if (stateSpeedLeft < 30)
      {
        stateSpeedLeft = 30;
      }
      else
      {
        if ( stateSpeedLeft < 0)
        {
          stateSpeedLeft = 0;
        } // if
        else
        {
          if (stateSpeedLeft < -30)
          {
            stateSpeedLeft = fabs(stateSpeedLeft);
            ptr_io->iosLeftEngine.direction = deBackward;            
          } // if
        } // else
      } // else

      // Check that the backward speed does not exceed C_SPEED_HIGH
      if (stateSpeedLeft > C_SPEED_HIGH)
      {
        stateSpeedLeft = C_SPEED_HIGH;
      }  // if 
            
      ptr_io->iosLeftEngine.speed = stateSpeedLeft;
                  
      stat_LastState = rsLeft;
      stateCount++;
      
      break;
    }


    // Action suggestion Right
    case rsRight:
    {
      

      stat_LastState = rsRight;
      stateCount++;
      
      break;
    }
  
    // Action suggestion Backward
    case rsBackward:
    {
      
      stat_LastState = rsBackward;
      stateCount++;
      
      break;
    }
    
   // Action suggestion Finnished
    case rsFinnished:
    {
      
      stat_LastState = rsFinnished;
      stateCount++;
      
      break;
    }
    
    default:
    break;
  }
  

  

  
} // taskLineFollow

// ============================================================================================

