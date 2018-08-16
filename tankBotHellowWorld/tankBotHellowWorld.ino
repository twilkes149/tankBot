#define RIGHT_DIR 3
#define RIGHT_STEP 2
#define LEFT_DIR 5
#define LEFT_STEP 4
#define TURRET_STEP 6
#define TURRET_DIR 7
#define TURRET_SLEEP 8

#include <Servo.h>

Servo shoulder, elbow;
long time = 0;
bool flag = false;

void setup() {
  elbow.attach(9);
  shoulder.attach(10);

  pinMode(RIGHT_DIR,OUTPUT);
  pinMode(RIGHT_STEP,OUTPUT);
  pinMode(LEFT_DIR,OUTPUT);
  pinMode(LEFT_STEP,OUTPUT);
  pinMode(TURRET_STEP,OUTPUT);
  pinMode(TURRET_DIR,OUTPUT);
  pinMode(TURRET_SLEEP,OUTPUT);

  digitalWrite(LEFT_STEP,LOW);
  digitalWrite(RIGHT_STEP,LOW);
  
  digitalWrite(LEFT_DIR,LOW);
  digitalWrite(RIGHT_DIR,HIGH);
  
  digitalWrite(TURRET_STEP,LOW);
  digitalWrite(TURRET_DIR,LOW);
  digitalWrite(TURRET_SLEEP,LOW);
  
  shoulder.write(90);
  elbow.write(90);
  time = millis();
  digitalWrite(TURRET_SLEEP,LOW);
  delay(5000);
}

void loop() {
  digitalWrite(LEFT_STEP,HIGH);
  digitalWrite(RIGHT_STEP,HIGH);
  //digitalWrite(TURRET_STEP,HIGH);
  delayMicroseconds(1200);
  digitalWrite(LEFT_STEP,LOW);
  digitalWrite(RIGHT_STEP,LOW);
  //digitalWrite(TURRET_STEP,LOW);
  delayMicroseconds(1200);

  if (millis() > time+1000) {
    time = millis();
    digitalWrite(13,HIGH);
    if (flag)
      shoulder.write(0);
    else
      shoulder.write(90);
      digitalWrite(13,LOW);
    flag = !flag;
  }
}
