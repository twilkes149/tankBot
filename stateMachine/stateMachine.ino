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
bool on = false;//flag to tell if the motors are moving
bool tick = false;

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
  Serial.begin(9600);
  Serial.println("ready");
  time = micros();
}

void loop() {
  if (Serial.available()) {//if we have an incoming message
    int message = Serial.read();
    Serial.println(message);
    on = true;//turn on motors
    
    //set direction of motors
    //subtract off '0' because the incoming bytes are the string
    //representation of 1,2,3,4,5 -> 49,50,etc
    switch (message-48) {
      case 1://forward
        forward();
      break;

      case 2://reverse
        reverse();
      break;

      case 3://left
        left();
      break;

      case 4://right
        right();
      break;
      
      case 5: //stop message or any other message
      default:
        on = false;//turn off motors
      break;
    }
  }

  if (on && (micros() > time + 1200)) {//if we need to tick the motors
     time = micros();
     digitalWrite(LEFT_STEP,tick);
     digitalWrite(RIGHT_STEP,tick);
     tick = !tick;
  }
}

void reverse() {
  digitalWrite(LEFT_DIR,LOW);
  digitalWrite(RIGHT_DIR,HIGH);
}

void forward() {
  digitalWrite(LEFT_DIR,HIGH);
  digitalWrite(RIGHT_DIR,LOW);
}

void left() {
  digitalWrite(LEFT_DIR,LOW);
  digitalWrite(RIGHT_DIR,LOW);
}

void right() {
  digitalWrite(LEFT_DIR,HIGH);
  digitalWrite(RIGHT_DIR,HIGH);
}

