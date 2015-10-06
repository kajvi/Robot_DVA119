#include <string.h>
#include "D_TaskSlope.h"

#define C_THIS_TASK "Slope 2015-10-06"


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

void taskSlope(struct ioStruct* ptr_io)
{

} // taskSlope

// ============================================================================================
