#include <string.h>
#include "B_TaskLabyrinth.h"

#define C_THIS_TASK "TaskLabyrinth 2015-10-08"

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
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  ptr_io->iosCurrentTaskIsFinished = 1; 

} // taskLabyrinth

// ============================================================================================

