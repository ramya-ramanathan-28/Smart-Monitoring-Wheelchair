#include <NewPing.h>
#include <AFMotor.h> 
#include <Servo.h>    


#define TRIG_PIN A4 
#define ECHO_PIN A5
#define MAX_DISTANCE_POSSIBLE 1000 
#define MAX_SPEED 150 // 
#define MOTORS_CALIBRATION_OFFSET 3
#define COLL_DIST 20 
#define TURN_DIST COLL_DIST+10 
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE_POSSIBLE);

long randNumber;
#define buzzer  9

AF_DCMotor leftMotor(4, MOTOR12_8KHZ); 
AF_DCMotor rightMotor(3, MOTOR12_8KHZ); 
Servo neckControllerServoMotor;


int pos = 0; 
int maxDist = 0;
int maxAngle = 0;
int maxRight = 0;
int maxLeft = 0;
int maxFront = 0;
int course = 0;
int curDist = 0;
String motorSet = "";
int speedSet = 0;

#include <QLearningMachine.h>
#include <SimpleMemory.h>


#include <RestState.h>
#include <LedState.h>


#include <SimpleButton.h>
#include <SimpleTimer.h>

using namespace q_behave;

RestState restState = RestState();
LedState right = LedState("right",12);
LedState left = LedState("left",13);



Action restActions[] = { { &right, 0.0 }, { &left, 0.0 } };
Action leftActions[] = { { &restState, 0.0 }};
Action rightActions[] = { { &restState, 0.0 }};

StateActions states[] = {
    {&restState,2, restActions },
    {&right,1,rightActions},
    {&left,1,leftActions},
                
};
StateMap stateMap = { 3, states };

double prob1[6];
double* internalMemmory[]={prob1};
SimpleMemory memory = SimpleMemory(stateMap,internalMemmory);

// learning 
QLearningMachine brain = QLearningMachine(&memory,&restState);


SimpleButton button1 = SimpleButton(8);
SimpleTimer rewardTimer;
int timerId;


//Button configuration
void onReward(){
    Serial.println("onReward: ");
    brain.stop(10.0);
}

void onButton1(SimpleButton& b){
    Serial.print("onButton1: ");
    Serial.println(b.pin);
    brain.start(0);
    if (rewardTimer.isEnabled(timerId)){
      
      rewardTimer.restartTimer(timerId);
    } else {
      timerId = rewardTimer.setTimeout(5000, onReward);  
    }
}

void moveForward() {
    motorSet = "FORWARD";
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)
  {
    leftMotor.setSpeed(speedSet+MOTORS_CALIBRATION_OFFSET);
    rightMotor.setSpeed(speedSet);
    delay(5);
  }
}
void moveBackward() {
    motorSet = "BACKWARD";
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)
  {
    leftMotor.setSpeed(speedSet+MOTORS_CALIBRATION_OFFSET);
    rightMotor.setSpeed(speedSet);
    delay(5);
  }
}  

void turnRight() {
  motorSet = "RIGHT";
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
  delay(400);
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);      
}  

void turnLeft() {
  motorSet = "LEFT";
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
  delay(400);
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
}  
void setup() {
  Serial.begin(9600);
  
  pinMode(buzzer, 9);
  neckControllerServoMotor.attach(10);  
  neckControllerServoMotor.write(90); 
  delay(2000);
   
  motorSet = "FORWARD"; 
  neckControllerServoMotor.write(90);
  moveForward();
  Serial.print("forward");
  button1.pressHandler(onButton1);
}



void loop() {
  delay(500);  
  unsigned int distanceInCms = sonar.ping() / US_ROUNDTRIP_CM; 
  Serial.println("Distance: " + String(distanceInCms) + "cm");
  button1.process();
  rewardTimer.run();
  if(distanceInCms>10)
  {
    moveForward();
    Serial.print("forward");
   noTone(buzzer);     
    delay(1000); 
  }
  else
  {  

    tone(9, 1000, 500); 
    delay(1000);        
    noTone(buzzer);     
    delay(1000); 
        
    if(digitalRead(12)==HIGH){
    turnRight();
    Serial.print("right");
  }
  else if(digitalRead(13)==HIGH){
    turnLeft();
    Serial.print("left");
  }
    
   
 
}
}





  
