#include <NewPing.h>
#include <AFMotor.h>
#include <Servo.h>   
 
 
#define TRIG_PIN A4
#define ECHO_PIN A5
#define MAX_DISTANCE_POSSIBLE 1000
#define MAX_SPEED 150
#define MOTORS_CALIBRATION_OFFSET 3
#define COLL_DIST 20
#define TURN_DIST COLL_DIST+10
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE_POSSIBLE);

long randNumber;
 
 
const int buzzer = 9;
 
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
 
 
void moveForward() {
void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(8, INPUT);
  neckControllerServoMotor.attach(10); 
  neckControllerServoMotor.write(90);
  delay(2000);
  
  motorSet = "FORWARD";
  neckControllerServoMotor.write(90);
  randomSeed(analogRead(0));
  moveForward();
}
 
 
 
void loop() {
  delay(500);  
  unsigned int distanceInCms = sonar.ping() / US_ROUNDTRIP_CM;
  Serial.println("Distance: " + String(distanceInCms) + "cm");
  if(distanceInCms>10)
  {
    moveForward();
    noTone(buzzer);    
    delay(9);
  }
  else
  { 
 
    tone(9, 1000); 
    delay(1000);        
    noTone(9);     
    delay(1000);
    randNumber = random(0, 2);
    Serial.println(randNumber);
    int val=0;
    val = digitalRead(8); 
     
    if(randNumber==0)
    {
      turnLeft();
      if (val == HIGH) {        
      turnRight();
      }
    }
    else if(randNumber==1)
    {
      turnRight();
      if (val == HIGH) {        
      turnLeft();
      }
    }
   
 
}
}