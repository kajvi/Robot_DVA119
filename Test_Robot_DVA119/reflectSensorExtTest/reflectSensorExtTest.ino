#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h> 

int R0Val = 0;
int R1Val = 0;
int R2Val = 0;

int R0ValMem = 0;
int R1ValMem = 0;
int R2ValMem = 0;


mySensors Sensors; // create sensors object

void setup() 
{ 
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("Reflect sensor test.");
  
  Sensors.beginSensors(); // start sensors
}

void loop() 
{
  R0Val  = Sensors.readReflect0();
  R1Val = Sensors.readReflect1();
  R2Val = Sensors.readReflect2();
  

  Serial.print("Reflect 0: ");Serial.print(R0Val);
  Serial.print("\tReflect 1: ");Serial.print(R1Val);
  Serial.print("\tReflect 2: ");Serial.print(R2Val);
  Serial.print("\n");
}
