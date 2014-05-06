#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <AccelStepper.h> //for moving both at once

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//^ create motorshield object
Adafruit_StepperMotor *myMotorLeft = AFMS.getStepper(200, 1); 
Adafruit_StepperMotor *myMotorRight = AFMS.getStepper(200, 2); 

void forwardstep1() 
{  
  myMotorLeft->onestep(FORWARD, DOUBLE);
}
void backwardstep1() 
{  
  myMotorLeft->onestep(BACKWARD, DOUBLE);
}
// wrappers for the second motor!
void forwardstep2() 
{  
  myMotorRight->onestep(FORWARD, DOUBLE);
}
void backwardstep2() 
{  
  myMotorRight->onestep(BACKWARD, DOUBLE);
}

AccelStepper stepperLeft(forwardstep1, backwardstep1);
AccelStepper stepperRight(forwardstep2, backwardstep2);

void goStraight()
{
  // Change direction at the limits
  stepperLeft.run();
  stepperRight.run();
}

void turnLeft()
{
  stepperRight.run();
}

void turnRight()
{
  stepperLeft.run();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  AFMS.begin();
  //myMotorLeft->setSpeed(10); //10 revs perminute
  //myMotorRight->setSpeed(10);
  
  stepperLeft.setMaxSpeed(200);
  stepperLeft.setAcceleration(100);
  //stepperLeft.moveTo(50000);
  
  stepperRight.setMaxSpeed(200);
  stepperRight.setAcceleration(100);
  //stepperRight.moveTo(50000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //myMotorRight->step(100, FORWARD, DOUBLE);
  //myMotorLeft->step(100,FORWARD, DOUBLE);
  //goStraight();
  //blink cliff sensor
  //check sensor
  //blink front
  //check front
  //blink side
  stepperLeft.run();
  stepperRight.run();
  //check side
}


