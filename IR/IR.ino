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

int time = millis();

int cliffTime;
int avgTime;


int timeTaken(){
  int tempTime = millis();
  
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
  return (millis() - tempTime);
}

int irBounce(int pinNo) //1 means something in the way; 0 means no
{
  tone(pinNo, 38000, 1000);
  //interpret the signal
  int timeTakenVar = timeTaken(); //time it took for IR to hit something and bounce back if it bounced back
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  /*
  TCCR1B = bit (WGM20) | bit (WGM21) | bit (COM2B1); // fast PWM, clear OC2B on compare
  TCCR2B = bit (WGM22) | bit (CS21);         // fast PWM, prescaler of 8
  OCR2A =  ((F_CPU / 8) / frequency) - 1;    // zero relative  
  OCR2B = ((OCR2A + 1) / 2) - 1;             // 50% duty cycle
  //this stuff makes pwm at 38khz
  */
  
  for(int i = 2; i < 6; i++)
    pinMode(i, OUTPUT); //sets pins 2-5 ie the IR LEDs as outputs
    
  pinMode(CliffPin, OUTPUT);
  pinMode(IRpin1, INPUT);
  
  int temp[3];
  blinkLED(CliffPin);
  cliffTime = timeTaken(); //pretty constant time so I blink the pin once and see  how long it takes to get back.
  for(int i = 3; i < 6; i++){
    blinkLED(i);
    temp[i-3] = timeTaken();    
  }
  
  int biggest = 1;
  for(int i = 1; i < 3; i++){
    if(temp[i]>temp[biggest])
      biggest = i;
  }
  
  int sum = 0;
  for( int i = 0; i < 3; i++){
    if(i != biggest)
      sum += temp[i];
  }
  avgTime = sum/2; //gets average time it takes for the signal to come back
  Serial.println(avgTime);
}

void loop() {
  
  blinkLED(CliffPin);
  //blinkLED
  if(timeTaken() > cliffTime/* + tolerance */) //interpret the cliff pin so that we can determine if its about to fall off or not 
  {
    /*determine whether I need to turn right, left, or around. I like how I don't have to actually 
    put anything after if branches as long as I have the braces.*/
  }
  for(int i = 3; i<6; i++)
  {
    int ifWall = irBounce(i);
    //do stuff with those LEDs
    if(i == 4 && ifWall == 0)
      Serial.println("go straight");//since this is separate I just needed something to fill in
    else
      Serial.println("do something else");
  }
  
}
