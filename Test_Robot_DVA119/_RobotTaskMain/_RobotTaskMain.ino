// _RobotTaskMain.cpp
//
// Main program that reads sensors, handles subtask, writes to actuators.
// 
// DVA119 -  Introduktion till robotik, ht'15, MdH
//
// Authors:
// Daniel Stenekap, Göran Forsström
//
// History
// 2015-10-16   Introduced
// 2015-10-23   Added second LED (Left, old now = Right)
//
// ============================================================================

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h>
#include "IO.h" 
#include "Utilities.h"
#include "A_TaskLineFollow.h"
#include "B_TaskLabyrinth.h"
#include "C_TaskBalls.h"
#include "D_TaskSlope.h"

#define C_THIS_VERSION "Main 2015-10-26"

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

// If the line below not inside a comment, then debug printouts are made on the serial port.
#define C_DEBUG_PRINT_ON

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
const int LedRightRedPin =  12;      // the number of the LED pin
const int LedRightGreenPin =  13;
const int LedLeftRedPin =  2;   // 10 fails??? är input för brytare!
const int LedLeftGreenPin =  11;

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
  stat_IO.iosRightLedRed = LOW;             
  stat_IO.iosRightLedGreen = LOW;
  stat_IO.iosLeftLedRed = LOW;             
  stat_IO.iosLeftLedGreen = LOW;

  // set the digital pin as output:
  pinMode(LedLeftRedPin, OUTPUT);
  pinMode(LedLeftGreenPin, OUTPUT);
  pinMode(LedRightRedPin, OUTPUT);
  pinMode(LedRightGreenPin, OUTPUT);
  
  // Initiate robot Task to the first task: linefollow
  stat_currentTask = rtLinefollow;
  stat_currentTask = rtSlope;
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
  int tmp;
  
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
    stat_IO.iosReflAnalog_3  = Sensors.readReflect3(); // Read analog value of reflect sensor 3

    stat_IO.iosIRAnalog = Sensors.readIR0();  //Analog value of IR sensor
    
    // Test:Left LD ind. new analog refl sensor
    setUpLedFromValueLimits(stat_IO.iosIRAnalog, 350, 450, 550, 1023, &stat_IO.iosLeftLedGreen, &stat_IO.iosLeftLedRed);

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
          stat_IO.iosLeftEngine.speed = 0;
          stat_IO.iosRightEngine.speed = 0;
          // Switch to next task if currenttask is finished
          stat_currentTask = rtLabyrinth;
          stat_currentTask = rtEndSentinel; // TEST!
          delay(100); // Try to move robot in position for labyrinth
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

    
    digitalWrite(LedLeftGreenPin, stat_IO.iosLeftLedGreen);
    digitalWrite(LedLeftRedPin, stat_IO.iosLeftLedRed);
    digitalWrite(LedRightGreenPin, stat_IO.iosRightLedGreen);
    digitalWrite(LedRightRedPin, stat_IO.iosRightLedRed);
    stat_IO.iosRightLedRed = LOW;             
    stat_IO.iosRightLedGreen = LOW;
    stat_IO.iosLeftLedRed = LOW;             
    stat_IO.iosLeftLedGreen = LOW;
    
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
