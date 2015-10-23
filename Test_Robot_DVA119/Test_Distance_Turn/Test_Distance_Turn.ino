//Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 
#include <EEPROM.h>

#define C_MOTOR_LEFT_2 2
#define C_MOTOR_RIGHT_1 1

#define C_ENGINE_SPEED 0
#define C_ARR_LENGHT 13

struct timeSpeedStruct {
  int tsLeftEngine;
  int tsRightEngine;
  int tsMilliSec;
};

mySensors Sensors; // create sensors object

myMotors Motors; // Create motors object

static int targetTime;
static int ix;


// constants won't change. Used here to set a pin number :
const int LedRedPin =  12;      // the number of the LED pin
const int LedGreenPin =  13;

int valIR = 0;

struct timeSpeedStruct timeSpeedArr[C_ARR_LENGHT] = {
  { 0,  0, 1900}, // 1 rakt
  { 0,  0, 1400}, // 2 sväng vänster
  { 0,  0,  700}, // 3 rakt
  { 0,  0, 1350}, // 4 sväng vänster
  { 0,  0,  700}, // 5 rakt
  { 0,  0, 2000}, // 6 sväng höger
  { 0,  0, 2000}, // 7 rakt
  { 0,  0, 2000}, // 8 sväng höger
  { 0,  0, 2000}, // 9 rakt
  { 0,  0, 2000}, // 10 sväng höger
  { 0,  0, 2000}, // 11 rakt
  { 0,  0, 2000}, // 12 sväng vänster
  { 0,  0, 2000} // 13 rakt
};


void setup() 
{ 
  // set the digital pin as output:
  pinMode(LedRedPin, OUTPUT);
  pinMode(LedGreenPin, OUTPUT);
  Sensors.beginSensors(); // start sensors
  Motors.beginMotors(); // Start up motors 
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("button and IR sensor test.");
  delay(6000);
  ix = 0;
  targetTime = millis() + timeSpeedArr[ix].tsMilliSec;
  
} // setup

void loop() 
{
  valIR = Sensors.readIR0();

  if (valIR < 350)
  {
    digitalWrite(LedGreenPin, LOW);
    digitalWrite(LedRedPin, LOW);
  }
  else if (valIR < 450)
  {
    digitalWrite(LedGreenPin, HIGH);
    digitalWrite(LedRedPin, HIGH);
  }
  else if (valIR < 550)
  {
    digitalWrite(LedGreenPin, HIGH);
    digitalWrite(LedRedPin, LOW);
  }
  else if (valIR < 650)
  {
    digitalWrite(LedGreenPin, LOW);
    digitalWrite(LedRedPin, HIGH);
  }

  Serial.print("\tIR sensor: ");Serial.print(valIR);
  Serial.print("\n");
  
  if (ix >= C_ARR_LENGHT)
  {
    Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); 
    Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
  } // if
  else
  {
    if (millis() <= targetTime)
    {
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, timeSpeedArr[ix].tsLeftEngine);
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, timeSpeedArr[ix].tsRightEngine*1.08);
    }
    else
    {
      ix++;      
      if (ix < C_ARR_LENGHT)
      {
        targetTime = millis() + timeSpeedArr[ix].tsMilliSec;
      } // if
    } // if
  } // else
} // loop
    







/*

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32
#define C_ARR_SIZE 100

int AxisX = 0;
int AxisY = 0;

int XArr[C_ARR_SIZE];
int YArr[C_ARR_SIZE];
int ArrIndex = 0;

mySensors Sensors; // create sensors object

// Example settings structure
struct StoreStruct {
  int x, y, ax, ay, AI;
};
struct StoreStruct storage;

int stopMeassureTime = 0;
int startMeassureTime = 0;
int count = 0;

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
     for (unsigned int t=0; t<sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(storage); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}


  //Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println("button and IR sensor test.");
  
  Sensors.beginSensors(); // start sensors

  

 
}


  delay(50);
  if((millis() >= startMeassureTime) && (count == 0))
  {
    
    AxisX = Sensors.readAccX();
    XArr[ArrIndex] = AxisX; 
    AxisY = Sensors.readAccY();
    YArr[ArrIndex] = AxisY;
    ArrIndex++;
    count = 1;
  }
  
  if (count == 1)
  {
    if (millis() <= stopMeassureTime)
    {
      if (ArrIndex < C_ARR_SIZE)
      {
        XArr[ArrIndex] = Sensors.readAccX();
        YArr[ArrIndex] = Sensors.readAccY();
        ArrIndex++;
      }
      AxisX = (AxisX + Sensors.readAccX())/2;
      AxisY = (AxisY + Sensors.readAccY())/2;
    }
    else
    {
      long xTemp = 0;
      long yTemp = 0;
      Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, 0); // Motor 1 should run forward with speed x
      Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, 0);
      storage.x = AxisX;
      storage.y = AxisY;
      for (int i = 0; i < ArrIndex; i++)
      {
        xTemp = xTemp + XArr[i];
        yTemp = yTemp + YArr[i];
      }
      storage.ax = xTemp/ArrIndex;
      storage.ay = yTemp/ArrIndex;
      storage.AI = ArrIndex;
      saveConfig();
    }
  }
}

*/
