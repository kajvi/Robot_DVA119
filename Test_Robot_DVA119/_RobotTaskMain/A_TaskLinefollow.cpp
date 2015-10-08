#include <string.h>
#include "IO.h"

#include "A_TaskLineFollow.h"

#define C_THIS_TASK "LineFollow 2015-10-08"



// Sets state of robot: what is State is the robot in.
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsStraight1,
  rsStraight2,
  rsStraight3,
  rsLeft1,
  rsLeft2,
  rsLeft3,
  rsRight1,
  rsRight2,
  rsRight3,
  rsBackward1,
  rsBackward2,
  rsFinnished1,
  rsFinnished2,
  rsFinnished3
};

enum lastestDirectionEnum {
  ldUnknown,
  ldInitial,
  ldStraight,
  ldLeft,
  ldRight
};

/* finns i IO.h
// Sets action of the robot depending on the state of the robot.
enum actionEnum {
  aeUnknown,
  aeInitial,
  aeStill,
  aeLeft,
  aeRight,
  aeForward,
  aeBackward,
  aeFinnished
};

jmf:
enum actionEnum {
  aeActionUnknown,
  aeActionStill,
  aeActionTurnHalfLeft,
  aeActionTurnFullLeft,
  aeActionTurnHalfRight,
  aeActionTurnFullRight,
  aeActionGoForward,
  aeActionGoBackward,
  aeActionSearch,
  aeFinnished
};
*/

static enum robotStateEnum stat_RobotState = rsInitial;
static enum lastestDirectionEnum stat_LastestDirection = ldInitial;
static enum actionEnum stat_Action = aeActionUnknown;

// ============================================================================================


void taskLineFollow(struct ioStruct* ptr_io)
{
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  // ptr_io->iosDelayMS = 2000;

  // Evaluate sensor data
  // ====================

// black is found on middle and Left - turn a little left!
  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    stat_Action = aeActionTurnHalfLeft;
  } // if


  // black is found only on Left - turn sharp left!
  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    stat_Action = aeActionTurnFullLeft;
  } // if


  // black is found only on Middle and Right - turn half right!
  if( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    stat_Action = aeActionTurnHalfRight;
  } // if


  // black is found only on Right - turn right!
  if( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    stat_Action = aeActionTurnFullRight;
  } // if


  // Black is found in middle - go forward!
  if ( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    stat_Action = aeActionGoForward;
  } // if


  // No black is found - Back up alittle!
  if ( (ptr_io->iosReflFrontLeft_0 == C_LIGHT_0) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_LIGHT_0) )
  {
    stat_Action = aeActionGoBackward;
  } // if


  // Black is found on all: Different actions depending on state and direction.
  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_DARK_1) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    stat_Action = aeFinnished;
  }


  if ( (ptr_io->iosReflFrontLeft_0 == C_DARK_1) &&  (ptr_io->iosReflFrontCenter_1 == C_LIGHT_0) && (ptr_io->iosReflFrontRight_2 == C_DARK_1) )
  {
    // Black is found on Left + Rigth but not on Middle - what to do?!
    stat_Action = aeActionGoForward;
  } // if

  switch (stat_RobotState)
  {
    // Initial Value : Drive Forward
    case rsInitial:
    {
      // TO DO!
      break;
    }

    // Action suggestion straight "1 time"
    case rsStraight1:
    {
      // TO DO!
      break;
    }

    // Action suggestion straight "2 time"
    case rsStraight2:
    {
      // TO DO!
      break;
    }

    // Action suggestion straight "3 time"
    case rsStraight3:
    {
      // TO DO!
      break;
    }

    // Action suggestion Left "1 time"
    case rsLeft1:
    {
      // TO DO!
      break;
    }

    // Action suggestion Left "2 time"
    case rsLeft2:
    {
      // TO DO!
      break;
    }
    
    // Action suggestion Left "3 time"
    case rsLeft3:
    {
      // TO DO!
      break;
    }

    // Action suggestion Right "1 time"
    case rsRight1:
    {
      // TO DO!
      break;
    }
    
    // Action suggestion Right "2 time"
    case rsRight2:
    {
      // TO DO!
      break;
    }
    
    // Action suggestion Right "3 time"
    case rsRight3:
    {
      // TO DO!
      break;
    }

    // Action suggestion Backward1 "1 time"
    case rsBackward1:
    {
      // TO DO!
      break;
    }
    
    // Action suggestion Backward2 "2 time"
    case rsBackward2:
    {
      // TO DO!
      break;
    }

   // Action suggestion Finnished "1 time"
    case rsFinnished1:
    {
      // TO DO!
      break;
    }

    // Action suggestion Finnished "2 time"
    case rsFinnished2:
    {
      // TO DO!
      break;
    }
    
    // Action suggestion Finnished "3 time"
    case rsFinnished3:
    {
      // TO DO!
      break;
    }
    
    default:
    break;
  }
  

  

  
} // taskLineFollow

// ============================================================================================

