#include <string.h>
#include "B_TaskLabyrinth.h"

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

void taskLabyrinth(struct ioStruct* ptr_io)
{

} // taskLabyrinth

// ============================================================================================

