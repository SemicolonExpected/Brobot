#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <AccelStepper.h> //for moving both at once
#include <TTree.h>

#define IRpin1 A1
#define IRpin2 A0

#define CliffPin 2   
#define RightPin 5   
#define FrontPin 4  
#define LeftPin 4   


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//^ create motorshield object
Adafruit_StepperMotor *mLeft = AFMS.getStepper(200, 2); 
Adafruit_StepperMotor *mRight = AFMS.getStepper(200, 1);

TTree maze = TTree(); //creates the TTree object used to map the maze
TTree *node = &maze; //points to where maze is stored 

void turnLeft(){
  mLeft->step(225,FORWARD,DOUBLE);
}
void backLeft(){
  mLeft->step(225,BACKWARD,DOUBLE);
}

//int stepCount = 0;
void goStraight(){
  for(int i = 0; i<10; i++)
  {
    mLeft->step(25,FORWARD,DOUBLE);
    mRight->step(25,BACKWARD,DOUBLE);
    //stepCount+=25;
    //Serial.println(stepCount);
  }
}

void turnRight(){
   mRight->step(225,BACKWARD,DOUBLE);
}
void backRight(){
   mRight->step(225,FORWARD,DOUBLE);
}

void goBack(){
  for(int i = 0; i<10; i++)
  {
    mLeft->step(25,BACKWARD,DOUBLE);
    mRight->step(25,FORWARD,DOUBLE);
    //stepCount -= 25;
    //Serial.println(stepCount);
  }
}
//int time = micros();
void backitUp(){
  if(node->back == NULL){
       //no way to back up anymore
  }
  else if(node->back->forward == node)
    goBack();
  else if(node->back->left == node){
    backLeft();
    goBack();
  }
  else{
    backRight();
    goBack();
  }
  node = node->back;
}
//int cliffTime;
int avgTime;
int times[3];

int readCliff(){
  tone(2,38000,1000);
  //int tempTime = micros();
   //while((!digitalRead(IRpin2))&& (micros()-tempTime <= 3000)){ 
    /*this should only do something (well in this case nothing)
      when it doesnt get a signal*/
    //do nothing
   //}
  return (analogRead(A2));
}

int irBounce(int pinNo) 
{
  tone(pinNo, 38000, 1);
  //interpret the signal
  delay(1);
  return (analogRead(A2)); //the higher this number is, the closer the object.
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  AFMS.begin();
  
  mLeft->setSpeed(255); 
  mRight->setSpeed(255);
  //Wheee Maximum Warp!!
  
  /*********************************************************
    In case we need to turn the bot right after the first
    blinks, this will ensure the steppers lock in place
    as if they arent one gets dragged around as if it were
    being driven differentially. 
   **********************************************************/
  mLeft->onestep(FORWARD,SINGLE);
  mRight->onestep(FORWARD,SINGLE);
  mLeft->onestep(BACKWARD,SINGLE);
  mRight->onestep(BACKWARD,SINGLE);
  
   /*********************************************************
   Test the IRs and determine what constitutes as traversable 
   or if there is a wall. Save each result in a temporary
   variable possibly Array[3] where [0],[1]and [2] correspond 
   to pin 3, 4, and 5 respectively, as well as directions
   0, 1, and 2.
   
   It will then take its first steps in the direction that 
   is availible of the lowest number. Ie left, right availible
   then it goes left. Forward, and right? Goes forward. Left,
   forward, then left. It will take x amount of steps then it
   will have reached a checkpoint. Generally it would be num
   of steps its body size is.
   **********************************************************/
   //cliffTime = readCliff();
   
   int timeSum = 0;
   uint8_t maxi = 0; 
   for(int i = 3; i <6; i++)
   {
     times[i-3] = irBounce(i);
     Serial.println(times[i-3]);
     if(times[i-3] > times[maxi])
       maxi = i-3;
     timeSum += times[i-3];
   }
   avgTime = timeSum/3;
   /*for(int i = 0; i<3; i++)
   {
     if(i == maxi)
       timeSum - times[i];
   }
   //ok so timeSum is more of the average time rather than JUST the sum.
   timeSum = timeSum/2; */
   /*********************************************************************
    Arduino IDE should have line highlighting so I can comeback here when
    I figure out a better way to determine what is a wall and what is not.
    **********************************************************************/
   if(times[0] < avgTime){
     turnLeft();
     goStraight(); 
   }
   if(times[1] < avgTime){
     goStraight(); 
   }
   if(times[2] < avgTime){
     turnRight();
     goStraight(); 
   }
   else{
     /*****************************************************************
      Since this is in set up it shows that there is no way in the maze
      Thus, I will have to alert the user that there is no way in the
      maze.
      *****************************************************************/
      goBack();
      node = node->back;
   }
}

void loop() {
    /*******************************************************
    Checks are we there yet. Not implimented. I am just happy
    if my robot makes it through the maze and doesnt tell us
    if its out, but we see its out. I hope I remember to 
    delete this. If not, pretend I said under time
    constraints, I am unable to implement something that 
    tells both us, and the robot if it is there yet.
    Poor robot, it'll forever believe it is trapped in the
    maze of doom, which really does make it a maze of doom.
    ********************************************************/
   
   /*********************************************************
   It checks again where it is at for more options and takes
   the option with the lowest value. As well as inserts a new
   node where there is a new node ie a path to traverse. It
   will also check if the path it is about to take has been
   visited, and if so, if all its paths are expended.
   *********************************************************/
   for(int i = 3; i <6; i++)
   {
     times[i-3] = irBounce(i);
     if(times[i] < avgTime)
       node->insert(i);
       /*****************************
       Doesn't matter if there is
       already a node there as the
       class handles this by checking
       for a node before adding. If 
       there is one it does nothing
       *****************************/
   }
   
   /*********************************************************
   First this tests if the lowest node has been visited:
   if not then visit it and turn it into true.
   If it has been visited, it then sees which is the next 
   unvisited node. If the right node is being visited, then
   this branch is expended and it backs out.
   **********************************************************/
   if(times[0] < avgTime){
     if(node->left->ifVisited == false){
       turnLeft();
       goStraight();
       node = node->left;
       node->ifVisited = true;
     }
     else{    
       if((times[1] < avgTime) && (node->forward->ifVisited == false)){
         goStraight();
         node = node->forward;
         node->ifVisited = true;
       }
       else if((times[2] < avgTime) && (node->right->ifVisited == false)){
        turnRight();
        goStraight();
        node = node->right;
        node->ifVisited = true;
       }       
       else{
         backitUp();
       }
     }
   }
   
   else if(times[1] < avgTime){
     if(node->forward->ifVisited == false){
      goStraight();
      node = node->forward;
      node->ifVisited = true;
     }
     else if((times[2] < avgTime) && (node->right->ifVisited == false)){
       turnRight();
       goStraight();
       node = node->right;
       node->ifVisited = true;
     }       
     else{
       backitUp();
     }
   }
   
   else if(times[2] < avgTime){
     if(node->right->ifVisited == false){
      turnRight();
      goStraight();
      node = node->right;
      node->ifVisited = true;
     }
     else{
       backitUp();
   }
   else{
     /*****************************************
     What do we do when there is nowhere to go?
     Check the way you came from then go back!
     ******************************************/
       backitUp();
   }
   
   /*********************************************************
   This checks if its about to fall off the face off the
   earth, but blinking the cliff pin :D
   *********************************************************/
   /*int temp = readCliff();
   if(temp < cliffTime){
     node->forward = NULL;
     if((node->left->ifVisited == false) && (irBounce(0) > avgTime)){
       node->insert(stepCount,0);
       turnLeft();
       goStraight();
       node = node->left;
       node->ifVisited = true;
     }
     else if((times[2] < avgTime) && (node->right->ifVisited == false)){
       turnRight();
       goStraight();
       node = node->right;
       node->ifVisited = true;
     }       
     else{
       if(node->back->forward == node)
         goBack();
       else if(node->back->left == node){
         backLeft();
         goBack();
       }
       else{
         backRight();
         goBack();
       }
       node = node->back;
     }
   }*/
}
