#include <string.h>
#include "A_TaskLineFollow.h"

#define C_THIS_TASK "LineFollow 2015-10-06"



// Sets state of robot: what is State is the robot in.
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsStraight,
  rsLeft,
  rsSharpLeft,
  rsRight,
  rsSharpRight,
  rsBackward,
  rsFinnished
};

enum lastestDirectionTurn {
  ldUnknown,
  ldInitial,
  ldStraight,
  ldLeft,
  ldRight
};

static enum robotStateEnum stat_RobotState = rsInitial;
enum lastestDirectionTurn stat_LastestDirection = ldInitial;

// ============================================================================================


void taskLineFollow(struct ioStruct* ptr_io)
{
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  // ptr_io->iosDelayMS = 2000;

  // Evaluate sensor data
  // ====================
  
  if ( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    // No black is found - search for it!
    stat_RobotState = rsBackward;
  } // if


  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    // black is found on middle and Left - turn half left!
    stat_RobotState = rsLeft;
  } // if
  
  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    // black is found only on Left - turn full left!
    stat_RobotState = rsSharpLeft;
  } // if


  if( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    // black is found only on Middle and Right - turn half right!
    stat_RobotState = rsRight;
  } // if
  
  if( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    // black is found only on Right - turn right!
    stat_RobotState = rsSharpRight;
  } // if


  if ( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    // Black is found in middle - go forward!
    stat_RobotState = rsStraight;
  } // if

  
  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    // Black is found on all and previous state was forward - Task compleeted!
    if (stat_RobotState == rsStraight)
    {
      stat_RobotState = rsFinnished;
    }

    // Else depending on current state and the previous known direction:
    else
    {
      // Keep going left but not sharply
      if (stat_RobotState == rsSharpLeft)
      {
        stat_RobotState = rsLeft;
      }

      // Keep going right but not sharply
      if (stat_RobotState == rsSharpRight)
      {
        stat_RobotState == rsRight;    
      }

      // State backward means we lost track of the line during a turn - depending on latest known direction of turn:
      if (stat_RobotState == rsBackward)
      {
         // if we lost the line during a left turn, we now returned to the line and we make a sharp left (override with iosDelayMs).
        if(stat_LastestDirection == ldLeft)
        {
          stat_RobotState == rsSharpLeft;
        }

        // if we lost the line during a Right turn, we now returned to the line and we make a sharp Right (override with iosDelayMs).
        if(stat_LastestDirection == ldRight)
        {
          stat_RobotState == rsSharpRight;
        }
      }
    }
  } // i
  

  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    // Black is found on Left + Rigth but not on Middle - what to do?!
    //actionMode = aeActionUnknown;
  } // if
  

  
} // taskLineFollow

// ============================================================================================

