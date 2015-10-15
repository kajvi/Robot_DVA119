

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 
#include <SimpleTimer.h>

#define C_TaskLinefollow = 0
#define C_TaskLabyrinth = 1
#define C_TaskBalls = 2
#define C_TaskSlope = 3

#define C_BLACK_1 1
#define C_LIGHT_0 0

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

#define C_ActionStill 0
#define C_ActionTurnLeft 1
#define C_ActionTurnRight 2
#define C_ActionGoForward 3
#define C_ActionSearch 4

#define C_SPEED_HIGH   90
#define C_SPEED_MIDDLE 70
#define C_SPEED_LOW    50
#define C_DELAY 30

// Sets state of robot: what is State is the robot in.
enum robotStateEnum {
  rsUnknown,
  rsStarting,
  rsLineFollow,
  rsLineSearch,
  rsBallSearch,
  rsBallReturn,
  rsFinnished
};

// Sets action of the robot depending on the state of the robot.
enum actionEnum {
  aeActionUnknown,
  aeActionStill,
  aeActionTurnHalfLeft,
  aeActionTurnFullLeft,
  aeActionTurnHalfRight,
  aeActionTurnFullRight,
  aeActionGoForward,
  aeActionSearch
};

// Sets engine directions.
enum directionEnum {
  deUnknown,
  deForward,
  deBackward
};

// Engine struct which contains direction and speed of the engine.
struct engineStruct {
  enum directionEnum direction;
  int speed;
};

// Keeps track of the robot state and which assignment it is dooing.
struct robotStateStruct {
  enum robotStateEnum robotState;
  int assignment;
};

// Initiate variables.
enum actionEnum robotAction;
struct robotStateStruct robotState;
struct engineStruct leftEngine;
struct engineStruct rightEngine;

// Initiate sensors.
int R0Val_Left = C_LIGHT_0;
int R1Val_Center = C_LIGHT_0;
int R2Val_Right = C_LIGHT_0;  // = 0 om ljust, =1 om svart

/* int ActionMode; // 0 = stand still, 1 = turn left, 2 = turn right, 3 = go forward, 4 = search
int LastAction = 0; // 0 = stand still, 1 = turn left, 2 = turn right, 3 = go forward, 4 = search

int searchCounter = 0;
float factor = 2.0;
*/

mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

void setup() 
{ 
  // Initiate robot state and actions.
  robotState.robotState = rsStarting;
  robotState.assignment = C_TaskLinefollow;
  robotAction = aeActionStill;
  
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors

  // w8 b4 starting.
  delay(4000);
} // setup

// ============================================================================================


void loop() 
{
  // Read all sensors
  R0Val_Left  = Sensors.readReflect0(); // Read digital value of reflect sensor 0
  R1Val_Center  = Sensors.readReflect1(); // Read digital value of reflect sensor 1
  R2Val_Right  = Sensors.readReflect2(); // Read digital value of reflect sensor 2


  // Track which state the robot is in.
  if(
  switch(robotState.robotStateEnum)
  {
    case rsStarting:
    if (!((R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0)) && !((R0Val_Left == C_LIGHT_1) &&  (R1Val_Center == C_LIGHT_1) && (R2Val_Right == C_LIGHT_1)))
    {
      robotState = rsLineFollow;
    }
    else
    if ((R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0))
    {
      robotState = rsSearching;
    }
    else
    {
      robotState = nextState;
    }
    break;

    case rsFollowing:
    if ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0) )
    {
      // black is found only on Left - turn left!
      leftEngine.direction = dBackward;
      leftEngine.speed = C_SPEED_LOW;
      rightEngine.direction = dForward;
      rightEngine.speed = C_SPEED_MIDDLE;
      
      
    }
      
    if( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_BLACK_1) )
    {
      // black is found only on Right - turn right!
      rightEngine.direction = dBackward;
      rightEngine.speed = C_SPEED_LOW;
      leftEngine.direction = dForward;
      leftEngine.speed = C_SPEED_MIDDLE;
    }


    if ( 
        ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_LIGHT_0) )
        ||
        ( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_LIGHT_0) )
        ||
        ( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_BLACK_1) )
        ||
        ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_BLACK_1) )
      )
    {
      leftEngine.direction = dForward;
      leftEngine.speed = C_SPEED_MIDDLE;
      rightEngine.direction = dForward;
      rightEngine.speed = C_SPEED_MIDDLE;
    }    
    break;

    case rsSearching:
    
    break;

    case rsFinnished:
    break;

    defalut:
    // To do: Error handling
    break;
  } // Switch

  // Activate Output
  if(leftEngine.direction == dBackward)
  {
    Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, leftEngine.speed);
  }
  if(leftEngine.direction == dForward)
  {
    Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, leftEngine.speed);
  }
  if(rightEngine.direction == dBackward)
  {
    Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, rightEngine.speed);
  }
  if(rightEngine.direction == dForward)
  {
    Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, rightEngine.speed);
  }

  /*
  // Evaluate sensor data
  ActionMode = C_ActionStill;
  if ( 
       ( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0) )
       ||
       ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_BLACK_1) )
     )
  {
    // No black is found - search for it!
    // OR
    // Black is found on Left + Rigth but not on Middle - search fot it!
    ActionMode = C_ActionSearch;
  }

  if ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0) )
  {
    // black is found only on Left - turn left!
    ActionMode = C_ActionTurnLeft;
    searchCounter = 0;
  }

  if( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_BLACK_1) )
  {
    // black is found only on Right - turn right!
    ActionMode = C_ActionTurnRight;
    searchCounter = 0;
  }


  if ( 
      ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_LIGHT_0) )
       ||
      ( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_LIGHT_0) )
        ||
      ( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_BLACK_1) )
       ||
      ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_BLACK_1) && (R2Val_Right == C_BLACK_1) )
     )
  {
    // Black is found... - go forward!
    ActionMode = C_ActionGoForward;
    searchCounter = 0;
  }


  if(ActionMode == C_ActionStill)
  {
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, 50);
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, 50);
      for (int i = 0; i<6; i++)
      {
        delay(1000);   
      }
  }

  
  // Activate outputs...
  LastAction = ActionMode;
  switch (ActionMode)
  {
    case C_ActionStill:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 0);
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 0);   
    break;
    
    case C_ActionTurnLeft:
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, C_SPEED_MIDDLE); 
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_HIGH);
    break;

    case C_ActionTurnRight:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_HIGH);
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_MIDDLE);    
    break;

    case C_ActionGoForward:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_HIGH); // Run motor 1 & 2 forward
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_HIGH);
    break;

    case C_ActionSearch:
      searchCounter++;
      if (searchCounter < 3)
      {
        break;
      }
  
      if(LastAction == C_ActionTurnLeft)
      {
        for (int i = 0; i<=5; i++)
        {
          // Turn right for 50 ms
          Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
          Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);
          delay(10)
        }
      }
      else
      {
        if(LastAction == C_ActionTurnRight)
        {
          // Turn left for 50 ms
          Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
          Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);
          delay(40)
        }
        else
        {
          if(LastAction == C_ActionGoForward)
          {
            for (int i = 0; i<=5; i++)
            {
              // Go backwards for 50 ms
              Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, C_SPEED_MIDDLE);
              Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);
              delay(10)
            }
          }
        }
        else
        {
          searchCounter = searchCounter + 1;
          factor = factor*0.95;
          int SearchSpeed = C_SPEED_MIDDLE;
          SearchSpeed = SearchSpeed*factor;

          // Begin Spiral search
          for (int i = 0; i<=5; i++)
            {
              Motors.runMotor(C_MOTOR_LEFT_1, FOREWARD, C_SPEED_MIDDLE);
              Motors.runMotor(C_MOTOR_RIGHT_2, FOREWARD, C_SPEED_LOW);
              delay(10)
            }
        }
    break;  
  } // switch 
  */        
} // main

// ============================================================================================

