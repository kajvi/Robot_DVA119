

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

#define C_BLACK_1 1
#define C_LIGHT_0 0

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

#define C_ActionStill 0
#define C_ActionTurnLeft 1
#define C_ActionTurnRight 2
#define C_ActionGoForward 3
#define C_ActionSearch 4
#define C_ActionUnknown 5

#define C_SPEED_HIGH   120
#define C_SPEED_MIDDLE 120
#define C_SPEED_LOW    100
#define C_DELAY 30

int R0Val_Left = 0;
int R1Val_Center = 0;
int R2Val_Right = 0;  // = 0 om ljust, =1 om svart

int ActionMode; // 0 = stand still, 1 = turn left, 2 = turn right, 3 = go forward


mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

void setup() 
{ 
  delay(4000);
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors
}

void loop() 
{
  
  // Read all sensors
  R0Val_Left  = Sensors.readReflect0(); // Read digital value of reflect sensor 0
  R1Val_Center  = Sensors.readReflect1(); // Read digital value of reflect sensor 1
  R2Val_Right  = Sensors.readReflect2(); // Read digital value of reflect sensor 2

  // Evaluate sensor data

  ActionMode = C_ActionStill;
  if ( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0) )
  {
    // No black is found - search for it!
    ActionMode = C_ActionSearch;
  }

  if ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_LIGHT_0) )
  {
    // black is found only on Left - turn left!
    ActionMode = C_ActionTurnLeft;
  }

  if( (R0Val_Left == C_LIGHT_0) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_BLACK_1) )
  {
    // black is found only on Right - turn right!
    ActionMode = C_ActionTurnRight;
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
  }


  if ( (R0Val_Left == C_BLACK_1) &&  (R1Val_Center == C_LIGHT_0) && (R2Val_Right == C_BLACK_1) )
  {
    // Black is found on Left + Rigth but not on Middle - what to do?!
    ActionMode = C_ActionUnknown;
  }


  // Activate outputs...
  switch (ActionMode)
  {
    case C_ActionStill:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 0);
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 0);   
    break;
    
    case C_ActionTurnLeft:
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, C_SPEED_LOW); 
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_MIDDLE);
    break;

    case C_ActionTurnRight:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);  
      delay(C_DELAY);   
    break;

    case C_ActionGoForward:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE); // Run motor 1 & 2 forward
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_MIDDLE);
      delay(C_DELAY);
    break;

    case C_ActionSearch:
      // ToDo!
    break;

    case C_ActionUnknown:
       // ToDo
    break;

    default:
      // ERROR
      break;
  } // switch


}
