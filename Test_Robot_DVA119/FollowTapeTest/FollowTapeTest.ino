
#define C_THIS_VERSION "DVA119 Robotics Intro: Follow Tape Test v. 2015-10-06/GF"



#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

#define C_BLACK_1 1
#define C_LIGHT_0 0

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

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







mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

// ============================================================================

void setup() 
{ 
  delay(4000);
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors
} // setup

// ============================================================================

void loop() 
{
  int reflectorLeft_0 = 0;
  int reflectorCenter_1 = 0;
  int reflectorRight_2 = 0;  // = 0 om ljust, = 1 om svart

  actionEnum actionMode; // See aeAction-defines!



  // Read all sensors
  // ================
  
  reflectorLeft_0  = Sensors.readReflect0(); // Read digital value of reflect sensor 0
  reflectorCenter_1  = Sensors.readReflect1(); // Read digital value of reflect sensor 1
  reflectorRight_2  = Sensors.readReflect2(); // Read digital value of reflect sensor 2



  // Evaluate sensor data
  // ====================
  
  actionMode = aeActionStill;
  if ( (reflectorLeft_0 == C_LIGHT_0) &&  (reflectorCenter_1 == C_LIGHT_0) && (reflectorRight_2 == C_LIGHT_0) )
  {
    // No black is found - search for it!
    actionMode = aeActionSearch;
  } // if


  if ( (reflectorLeft_0 == C_BLACK_1) &&  (reflectorCenter_1 == C_BLACK_1) && (reflectorRight_2 == C_LIGHT_0) )
  {
    // black is found on middle and Left - turn half left!
    actionMode = aeActionTurnHalfLeft;
  } // if
  
  if ( (reflectorLeft_0 == C_BLACK_1) &&  (reflectorCenter_1 == C_LIGHT_0) && (reflectorRight_2 == C_LIGHT_0) )
  {
    // black is found only on Left - turn full left!
    actionMode = aeActionTurnFullLeft;
  } // if


  if( (reflectorLeft_0 == C_LIGHT_0) &&  (reflectorCenter_1 == C_BLACK_1) && (reflectorRight_2 == C_BLACK_1) )
  {
    // black is found only on Middle and Right - turn half right!
    actionMode = aeActionTurnHalfRight;
  } // if
  
  if( (reflectorLeft_0 == C_LIGHT_0) &&  (reflectorCenter_1 == C_LIGHT_0) && (reflectorRight_2 == C_BLACK_1) )
  {
    // black is found only on Right - turn right!
    actionMode = aeActionTurnFullRight;
  } // if


  if ( (reflectorLeft_0 == C_LIGHT_0) &&  (reflectorCenter_1 == C_BLACK_1) && (reflectorRight_2 == C_LIGHT_0) )
  {
    // Black is found in middle - go forward!
    actionMode = aeActionGoForward;
  } // if

  
  if ( (reflectorLeft_0 == C_BLACK_1) &&  (reflectorCenter_1 == C_BLACK_1) && (reflectorRight_2 == C_BLACK_1) )
  {
    // Black is found on all - strange but go forward!
    actionMode = aeActionGoForward;
  } // i
  

  if ( (reflectorLeft_0 == C_BLACK_1) &&  (reflectorCenter_1 == C_LIGHT_0) && (reflectorRight_2 == C_BLACK_1) )
  {
    // Black is found on Left + Rigth but not on Middle - what to do?!
    actionMode = aeActionUnknown;
  } // if



  // Do Actions
  // ==========
  
  switch (actionMode)
  {
    case aeActionStill:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 0);
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 0);   
    break;
    
    case aeActionTurnHalfLeft:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_LOW); 
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_MIDDLE);
    break;    
    
    case aeActionTurnFullLeft:
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, C_SPEED_LOW); 
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_MIDDLE);
    break;

     case aeActionTurnHalfRight:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_LOW);  
    break;
    
    case aeActionTurnFullRight:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);  
    break;

    case aeActionGoForward:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_MIDDLE);
    break;

    case aeActionSearch:
      // ToDo!
    break;

    case aeActionUnknown:
       // ToDo
    break;

    default:
      // ERROR
      break;
  } // switch

  delay(C_DELAY);

} // loop

// ============================================================================

