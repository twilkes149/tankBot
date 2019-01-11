#define RIGHT_DIR 6
#define RIGHT_STEP 5
#define RIGHT_ENABLE 7

#define LEFT_DIR 3
#define LEFT_STEP 2
#define LEFT_ENABLE 4

#define SHOULDER 12
#define ELBOW 11
#define WRIST 10
#define CLAW 9
#define TURRET 8

#include <Servo.h>

Servo shoulder, elbow, wrist, claw;
long time = 0;
bool flag = false;

void setup() {
  shoulder.attach(SHOULDER);
  elbow.attach(ELBOW);
  //wrist.attach(WRIST);
  //claw.attach(CLAW);

  pinMode(RIGHT_DIR,OUTPUT);
  pinMode(RIGHT_STEP,OUTPUT);
  pinMode(LEFT_DIR,OUTPUT);
  pinMode(LEFT_STEP,OUTPUT);
  pinMode(LEFT_ENABLE,OUTPUT);
  pinMode(RIGHT_ENABLE,OUTPUT);

  pinMode(SHOULDER,OUTPUT);
  pinMode(ELBOW, OUTPUT);

  digitalWrite(LEFT_STEP,LOW);
  digitalWrite(RIGHT_STEP,LOW);
  
  digitalWrite(LEFT_DIR,HIGH);
  digitalWrite(RIGHT_DIR,LOW);
  
  digitalWrite(RIGHT_ENABLE,LOW);
  digitalWrite(LEFT_ENABLE,LOW);

  shoulder.write(0);  
  elbow.write(0);
}

void loop() {
  digitalWrite(LEFT_STEP,HIGH);
  digitalWrite(RIGHT_STEP,HIGH);  
  delayMicroseconds(1200);
  digitalWrite(LEFT_STEP,LOW);
  digitalWrite(RIGHT_STEP,LOW);  
  delayMicroseconds(1200);
}
