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
// NewPing setup of pin and maximum distance.
long randNumber;




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


void setup() {
  Serial.begin(9600);
  neckControllerServoMotor.attach(10);  
  neckControllerServoMotor.write(90); 
  delay(2000);
  //checkPath(); 
  motorSet = "FORWARD"; 
  neckControllerServoMotor.write(90);
  randomSeed(analogRead(0));
  moveForward();
}



void loop() {
  delay(500);  // Wait 500ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int distanceInCms = sonar.ping() / US_ROUNDTRIP_CM; // Send ping, get ping time in microseconds (uS) and convert it to centimeters (cm)
  Serial.println("Distance: " + String(distanceInCms) + "cm");
  if(distanceInCms>10)
  {
    moveForward();
  }
  else
  {  


    randNumber = random(0, 2);
    Serial.println(randNumber);
    if(randNumber==0)
    {
      turnLeft();
    }
    else
    {
      turnRight();
    }
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

------------------------------------------------------------------------------------------------------------------------


#include <QLearningMachine.h>
#include <SimpleMemory.h>

//States
#include <RestState.h>
#include <LedState.h>

//Interaction
#include <SimpleButton.h>
#include <SimpleTimer.h>

using namespace q_behave;

//Define states and buttons
RestState restState = RestState();
LedState right = LedState("right",9);
LedState left = LedState("left",10);


//Prepare connection map between states. Note that it is allow only to move
//from rest state to led states.
Action restActions[] = { { &right, 0.0 }, { &left, 0.0 } };
Action leftActions[] = { { &restState, 0.0 }};
Action rightActions[] = { { &restState, 0.0 }};

StateActions states[] = {
    {&restState,2, restActions },
    {&right,1,rightActions},
    {&left,1,leftActions},
                
};
StateMap stateMap = { 3, states };

//Simple memory definition and configuration. All memory is blocked in heap.
//Please note that memory is volatile right now and will not survive restart.
double prob1[6];
double* internalMemmory[]={prob1};
SimpleMemory memory = SimpleMemory(stateMap,internalMemmory);

//Main learning process. 
QLearningMachine brain = QLearningMachine(&memory,&restState);

//Interactions
SimpleButton button1 = SimpleButton(14);
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
      //if timer was already enable than restart it
      rewardTimer.restartTimer(timerId);
    } else {
      timerId = rewardTimer.setTimeout(5000, onReward);  
    }
}


//Startup code
void setup() {
   Serial.begin(9600);
   button1.pressHandler(onButton1);
}

// the loop routine runs over and over again forever:
void loop() {
  button1.process();
  rewardTimer.run();
}