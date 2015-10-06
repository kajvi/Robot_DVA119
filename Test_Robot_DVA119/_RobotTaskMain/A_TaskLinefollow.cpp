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
  rsBackwards,
  rsFinnished
};

static enum robotStateEnum stat_RobotState = rsInitial;

// ============================================================================================


void taskLineFollow(struct ioStruct* ptr_io)
{
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  // ptr_io->iosDelayMS = 2000;
  

  
} // taskLineFollow

// ============================================================================================

