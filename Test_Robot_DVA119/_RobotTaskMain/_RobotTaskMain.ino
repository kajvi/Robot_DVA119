#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h>
#include "IO.h" 
#include "A_TaskLineFollow.h"
#include "B_TaskLabyrinth.h"
#include "C_TaskBalls.h"
#include "D_TaskSlope.h"

#define C_THIS_VERSION "Main 2015-10-08"

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

//#define C_DEBUG_PRINT_ON

// Sets the Task which the robot is solving.
enum robotTaskEnum {
  rtUnknown,
  rtLinefollow,
  rtLabyrinth,
  rtBalls,
  rtSlope,
  rtEndSentinel
};

mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

// Declaring Variables.
static struct ioStruct stat_IO;
static enum robotTaskEnum stat_currentTask;

// constants won't change. Used here to set a pin number :
const int LedRedPin =  12;      // the number of the LED pin
const int LedGreenPin =  13;

// Variables for timekeeping.
static unsigned long targetTime = 0;
static int checkPoint = 0;
    
// ============================================================================================

void setup() 
{ 
  // Initiate some IO-struct to start values
  stat_IO.iosMessageChArr[0] = 0;
  stat_IO.iosDelayMS = 0;
  
  // Sets initial Led output to off : LOW = off : HIGH = on
  stat_IO.iosLedRed = LOW;             
  stat_IO.iosLedGreen = LOW;

  // set the digital pin as output:
  pinMode(LedRedPin, OUTPUT);
  pinMode(LedGreenPin, OUTPUT);

  // Initiate robot Task to the first task: linefollow
  stat_currentTask = rtLinefollow;
  
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors

  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println(C_THIS_VERSION);

  // w8 b4 starting.
  delay(2500);
} // setup

// ============================================================================================

void loop() 
{
  static bool isBreaked = 0;
#ifdef C_DEBUG_PRINT_ON

  Serial.print(millis());
  Serial.print(" - ");
#endif

/*  
  // Only run limited time
  if (millis() > 10*1000)
  {
    if (!isBreaked)
    {
       Serial.print(millis());
       Serial.print(" - ");
       Serial.println("BREAK");
    }
    isBreaked = true;
  } // if

  else
*/
  {
  

  
    // Read all sensors
    // ================

  
    stat_IO.iosReflFrontLeft_0   = Sensors.readReflect0(); // Read digital value of reflect sensor 0
    stat_IO.iosReflFrontCenter_1 = Sensors.readReflect1(); // Read digital value of reflect sensor 1
    stat_IO.iosReflFrontRight_2  = Sensors.readReflect2(); // Read digital value of reflect sensor 2

    stat_IO.iosAccelerometerX  = Sensors.readAccX();
    stat_IO.iosAccelerometerY  = Sensors.readAccY();

    stat_IO.iosCurrentTaskIsFinished = 0;

    // Execute current task.
    // ====================
  
    // Determine which task the robot is preforming
    switch(stat_currentTask)
    {
      case rtLinefollow:
      {
        taskLineFollow(&stat_IO);
        if (stat_IO.iosCurrentTaskIsFinished != 0)
        {
          // Switch to next task if currenttask is finished
          stat_currentTask = rtLabyrinth;
        }
        break;
      }
    
      case rtLabyrinth:
      {
        taskLabyrinth(&stat_IO);
        if (stat_IO.iosCurrentTaskIsFinished != 0)
        {
          // Switch to next task if currenttask is finished
          stat_currentTask = rtBalls;
        }      
        break;
      }
    
      case rtBalls:
      {
        taskBalls(&stat_IO);
         if (stat_IO.iosCurrentTaskIsFinished != 0)
        {
          // Switch to next task if currenttask is finished
          stat_currentTask = rtSlope;
        }    
        break;
      }
    
      case rtSlope:
      {
        taskSlope(&stat_IO);
        if (stat_IO.iosCurrentTaskIsFinished != 0)
        {
          // Switch to next task if currenttask is finished
          // All tasks finished - ToDo ???terminate??
          stat_currentTask = rtEndSentinel;
        }     
        break;
      }
    
      default:
      break;
    } // switch


    // Do activations.
    // ==============

    if (stat_IO.iosLeftEngine.direction == deBackward)
    {
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, stat_IO.iosLeftEngine.speed);
    } // if
    if (stat_IO.iosLeftEngine.direction == deForward)
    {
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, stat_IO.iosLeftEngine.speed);
    } // if
    if (stat_IO.iosRightEngine.direction == deBackward)
    {
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, stat_IO.iosRightEngine.speed);
    } // if
    if (stat_IO.iosRightEngine.direction == deForward)
    {
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, stat_IO.iosRightEngine.speed);
    } // if

    
    digitalWrite(LedGreenPin, stat_IO.iosLedGreen);
    digitalWrite(LedRedPin, stat_IO.iosLedRed);
    stat_IO.iosLedRed = LOW;             
    stat_IO.iosLedGreen = LOW;

#ifdef C_DEBUG_PRINT_ON
    if (stat_IO.iosMessageChArr[0] != 0)
    {
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(stat_IO.iosMessageChArr);
      stat_IO.iosMessageChArr[0] = 0;
    } // if

    if (stat_IO.iosMessageInteger != -1)
    {
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(stat_IO.iosMessageInteger);
      stat_IO.iosMessageInteger = -1;
    } // if
#endif

    if (stat_IO.iosDelayMS != 0)
    {
      delay(stat_IO.iosDelayMS);
      stat_IO.iosDelayMS = 0;
    } // if

  } // else
} // loop

// ============================================================================================
