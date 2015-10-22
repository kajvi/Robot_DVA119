//Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 
#include <EEPROM.h>

myMotors Motors; // Create motors object

#define C_MOTOR_LEFT_2 2
#define C_MOTOR_RIGHT_1 1

#define C_ENGINE_SPEED 0

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

void setup() 
{ 
  //Serial.begin(9600); // set up Serial library at 9600 bps
  //Serial.println("button and IR sensor test.");
  
  Sensors.beginSensors(); // start sensors

  Motors.beginMotors(); // Start up motors 
  delay(6000);
  stopMeassureTime = millis() + 5000;
  startMeassureTime = millis() + 1000;

  Motors.runMotor(C_MOTOR_LEFT_2, FORWARD, C_ENGINE_SPEED); // Motor 1 should run forward with speed x
  Motors.runMotor(C_MOTOR_RIGHT_1, FORWARD, C_ENGINE_SPEED*1.08);
}

void loop() 
{
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
