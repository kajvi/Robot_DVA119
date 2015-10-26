// Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

#define C_THIS_VERSION "DVA119 Robotics Intro: Slope Test v. 2015-10-01/GF"

myMotors Motors; // Create motors object

#define C_MOTOR_RIGHT_1 1
#define C_MOTOR_LEFT_2 2

#define C_K_R 4
#define C_K_L 2
#define C_SPEED_HIGH 70
#define C_SPEED_MEDIUM 60
#define C_SPEED_LOW 50

#define C_TILT_X 338 - 15
// #define C_TILT_X TO DO!
#define C_TILT_Y 361
// #define C_TILT_Y TO DO!

#define C_WINDOW_X 0
#define C_ACC_WINDOW_WIDTH_Y 2

#define C_AVERAGE_COUNT 10

#define C_FLOAT_WIDTH 6

// constants won't change. Used here to set a pin number :
const int LedRedPin =  12;      // the number of the LED pin
const int LedGreenPin =  13;
const int LedLeftRedPin =  2;   // 10 fails??? är input för brytare!
const int LedLeftGreenPin =  11;


mySensors Sensors; // create sensors object

int valAxisX = C_TILT_X;
int valAxisY = C_TILT_Y;

// ============================================================================

void setup() 
{ 
  // set the digital pin as output:
  pinMode(LedRedPin, OUTPUT);
  pinMode(LedGreenPin, OUTPUT);
  pinMode(LedLeftRedPin, OUTPUT);
  pinMode(LedLeftGreenPin, OUTPUT);
  
  //Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println(C_THIS_VERSION);
  
  Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 

  delay(4000);
  digitalWrite(LedGreenPin, HIGH);
  digitalWrite(LedRedPin, HIGH);
  
  /*
  Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, C_SPEED_MEDIUM); 
  Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_SPEED_HIGH*1.08);
  delay(1000);
  Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, C_SPEED_LOW); 
  Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_SPEED_MEDIUM*1.08);
  */
  delay(4000);
} // setup

// ============================================================================

static int isRunning = 1;

void loop() 
{
  if (isRunning == 1)
  {
    valAxisX = (valAxisX + Sensors.readAccX()) / 2;
    valAxisY = (valAxisY + Sensors.readAccY()) / 2;
  
    int diffX = valAxisX - C_TILT_X; 
    
    if (diffX < -C_WINDOW_X)
    {
      digitalWrite(LedGreenPin, HIGH);
      digitalWrite(LedRedPin, LOW);
      // Om detta sker så har vi Uppåtlut och Höger motor behöver Kräm
      int leftSpeed = C_SPEED_MEDIUM + diffX*C_K_L;
      
      
      if (leftSpeed < 40)
      {
        digitalWrite(LedGreenPin, HIGH);
        digitalWrite(LedRedPin, HIGH);
        leftSpeed = C_SPEED_LOW;
        Motors.runMotor(C_MOTOR_LEFT_2, BACKWARD, leftSpeed);
      }
      else
      {
        Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, leftSpeed);
      }
    
      //Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, leftSpeed);  
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_SPEED_MEDIUM*1.08 - diffX*C_K_R);
    }
    else
    if (diffX < C_WINDOW_X)
    {
      digitalWrite(LedGreenPin, LOW);
      digitalWrite(LedRedPin, LOW);    
    }
    else
    {
      digitalWrite(LedGreenPin, LOW);
      digitalWrite(LedRedPin, HIGH);
      
      // Vi har Nedåtlut och Vänster Motor behöver kräm!
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, C_SPEED_MEDIUM + diffX*C_K_L);
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_SPEED_MEDIUM*1.08 - diffX*C_K_R);
    }
  
    delay(10);
  
    // Check if tape reached...
    if ( (Sensors.readReflect0() == 1) && (Sensors.readReflect1() == 1) && (Sensors.readReflect2() == 1) )
    {
      digitalWrite(LedLeftRedPin, HIGH);
      Motors.runMotor(C_MOTOR_LEFT_2,  FORWARD, 0);
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
      isRunning = 0;
    }
  }
  else
  {
    // nothing...
  }


} // loop

// ============================================================================

