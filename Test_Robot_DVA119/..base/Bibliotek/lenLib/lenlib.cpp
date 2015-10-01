#include "lenlib.h"
#include "Arduino.h"
#include <Adafruit_MotorShield.h>
#include <Servo.h> 

void mySensors::beginSensors()
{
	_R0 = 7;
	_R1 = 6;
	_R2 = 5;
	_R3 = A2;
	_IR = A3;
	_B0 = 9;
	pinMode(_B0, INPUT_PULLUP);
	_B1 = 8;
	pinMode(_B1, INPUT_PULLUP);
	_xAxis = A0;
	_yAxis = A1;
}
void mySensors::setAllSensors(uint8_t Reflect0, uint8_t Reflect1, uint8_t Reflect2, uint8_t Reflect3, uint8_t IR0, uint8_t Button0, uint8_t Button1, uint8_t xAxis, uint8_t yAxis)
{
	pinMode(_B0, INPUT);
	pinMode(_B1, INPUT);
	_R0 = Reflect0;
	_R1 = Reflect1;
	_R2 = Reflect2;
	_R3 = Reflect3;
	_IR = IR0;
	_B0 = Button0;
	pinMode(_B0, INPUT_PULLUP);
	_B1 = Button1;
	pinMode(_B1, INPUT_PULLUP);
	_xAxis = xAxis;
	_yAxis = yAxis;
}
int mySensors::readReflect0()
{
	return digitalRead(_R0);
}
int mySensors::readReflect1()
{
	return digitalRead(_R1);
}
int mySensors::readReflect2()
{
	return digitalRead(_R2);
}
int mySensors::readReflect3()
{
	return analogRead(_R3);
}
int mySensors::readIR0()
{
	return analogRead(_IR);
}
int mySensors::readButton0()
{
	return digitalRead(_B0);
}
int mySensors::readButton1()
{
	return digitalRead(_B1);
}
int mySensors::readAccX()
{
	return analogRead(_xAxis);
}
int mySensors::readAccY()
{
	return analogRead(_yAxis);
}


void myMotors::beginMotors()
{
	AFMS.begin();
	servo.attach(10);
}
void myMotors::angleServo(uint8_t angle)
{
	servo.write(map(angle, 0, 255, 0, 180));
}
void myMotors::runMotor(uint8_t motor, uint8_t dir, uint8_t vel)
{
	switch (motor)
	{
	case 1:
	{
			  myMotor1->run(dir);
			  myMotor1->setSpeed(vel);
	}
		break;
	case 2:
	{
			  myMotor2->run(dir);
			  myMotor2->setSpeed(vel);
	}
		break;
	case 3:
	{
			  myMotor3->run(dir);
			  myMotor3->setSpeed(vel);
	}
		break;
	case 4:
	{
			  myMotor4->run(dir);
			  myMotor4->setSpeed(vel);
	}
		break;
	default:
		Serial.print("No motor selected.\n");
	}
}


