#include <Wire.h>

#define IRpin_PIN1 PIND 

#define IRpin1 6

#define CliffPin 2   
#define RightPin 3   
#define FrontPin 4  
#define LeftPin 5   

#define RESOLUTION 20

int ledValue = LOW;

#define BLINKTERVAL 1000
/*^ duration in which the IR is on :D */

int time = micros();

int cliffTime;
int avgTime;

int timeTaken(){
  int tempTime = micros();
  
  /*while(!(IRpin_PIN1 & (1 << IRpin1))){ //while the value of IRpin1 and IRpin1 shifted bitwise with a 1 doesnt = 1. Ie while IRpin1 = 0
    //honestly its suppose to do nothing, but I dont know how to tell it to wait until the ir recievers is high
    if((millis() - tempTime) > 65000) */
    /*if it took way too long for the signal to come back assume there is nothing in the way (though this
      might be a problem within the next ir signal gets sent out it might jumble the whole thing though this is 
      a fail safe to ensure the program doesnt stop if the signal gets totally lost*/
     // return 0;
  //}
  while(!digitalRead(IRpin1)){
    //do nothing
  }
  return (micros() - tempTime);
}

int irBounce(int pinNo) //1 means something in the way; 0 means no
{
  tone(pinNo, 38000, 1000);
  //interpret the signal
  int timeTakenVar = timeTaken(); //time it took for IR to hit something and bounce back if it bounced back
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}
