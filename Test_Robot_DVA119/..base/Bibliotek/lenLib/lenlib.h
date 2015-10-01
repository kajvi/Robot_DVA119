#ifndef lenlib_H
#define lenlib_H

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h> 

class mySensors
{
private:
	uint8_t _R0;
	uint8_t _R1;
	uint8_t _R2;
	uint8_t _R3;
	uint8_t _IR;
	uint8_t _B0;
	uint8_t _B1;
	uint8_t _xAxis;
	uint8_t _yAxis;
public:
	void beginSensors();
	void setAllSensors(uint8_t Reflect0, uint8_t Reflect1, uint8_t Reflect2, uint8_t Reflect3, uint8_t IR0, uint8_t Button0, uint8_t Button1, uint8_t xAxis, uint8_t yAxis);
	int readReflect0();
	int readReflect1();
	int readReflect2();
	int readReflect3();
	int readIR0();
	int readButton0();
	int readButton1();
	int readAccX();
	int readAccY();
};

class myMotors
{
private:
	Adafruit_MotorShield AFMS = Adafruit_MotorShield();
	Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
	Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
	Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);
	Adafruit_DCMotor *myMotor4 = AFMS.getMotor(4);
	Servo servo;
public:
	void beginMotors();
	void angleServo(uint8_t angle);
	void runMotor(uint8_t motor, uint8_t dir, uint8_t vel);
};

#endif