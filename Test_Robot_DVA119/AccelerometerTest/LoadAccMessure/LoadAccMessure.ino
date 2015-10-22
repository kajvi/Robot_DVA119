//Direction: Right to left On: leftward down sloap

#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 
#include <EEPROM.h>


// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

int AxisX = 0;
int AxisY = 0;

mySensors Sensors; // create sensors object


// Example settings structure
struct StoreStruct {
  int x, y, ax, ay, AI;
};
struct StoreStruct storage;

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
  Sensors.beginSensors(); // start sensors
  Serial.begin(9600); // set up Serial library at 9600 bps
  loadConfig();
  Serial.print("X: " + String(storage.x) + "\t Y: " + String(storage.y)  + "\t AX: " + String(storage.ax) + "\t AY: " + String(storage.ay) + "\t AI: " + String(storage.AI) + "\n");
}

void loop() 
{
  delay(1000);
  AxisX = Sensors.readAccX();
  AxisY = Sensors.readAccY();
  
  Serial.print("X: " + String(AxisX) + "\t Y: " + String(AxisY) + "\n");
}
