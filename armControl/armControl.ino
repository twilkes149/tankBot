#define SHOULDER 3
#define ELBOW 5
#define WRIST 6
#define CLAW 9

#define SHOULDER_LOWER_LIMIT 0
#define SHOULDER_UPPER_LIMIT 180

#define ELBOW_LOWER_LIMIT 45
#define ELBOW_UPPER_LIMIT 180

#define WRIST_LOWER_LIMIT 90
#define WRIST_UPPER_LIMIT 270

//states for motion
#define ST_WAIT 0 //all stop
#define ST_SHOULDER_FWD 1
#define ST_SHOULDER_REV 2
#define ST_ELBOW_FWD 3
#define ST_ELBOW_REV 4
#define ST_WRIST_FWD 5
#define ST_WRIST_REV 6


#include <Servo.h>
Servo shoulder, elbow, wrist, claw;
int shoulderAngle, elbowAngle, wristAngle;
int state;

void setup() {
  shoulder.attach(SHOULDER);
  elbow.attach(ELBOW);
  wrist.attach(WRIST);
  claw.attach(CLAW);

  claw.write(0);
  shoulderAngle = 45;
  elbowAngle = 90;
  wristAngle = 110;
  
  setShoulderAngle(shoulderAngle);//init pos for shoulder
  setElbowAngle(elbowAngle);
  setWristAngle(wristAngle);

  Serial.begin(9600); 
  state = ST_WAIT; 
}
int message = 0;
void loop() {
  if (Serial.available()) {
    message = Serial.read();
    //change state based on message
    if (message == 0 || message == 1 || message == 2 || message == 3 || 
      message == 4 || message == 5 || message == 6) {
      state = message;
    }
    else {
      state = ST_WAIT;
    }
  }
  
  if (millis() % 50 == 0) {//tick 10 times a second
    switch (state) {
      case ST_WAIT:
      break;
      
      case ST_SHOULDER_FWD:
        if (shoulderAngle < SHOULDER_UPPER_LIMIT) {
          shoulderAngle++;
        }
        setShoulderAngle(shoulderAngle);
      break;
      
      case ST_SHOULDER_REV:
        if (shoulderAngle > SHOULDER_LOWER_LIMIT) {
          shoulderAngle--;
        }
        setShoulderAngle(shoulderAngle);
      break;
      
      case ST_ELBOW_FWD:
        if (elbowAngle < ELBOW_UPPER_LIMIT) {
          elbowAngle++;
        }
        setElbowAngle(elbowAngle);
      break;
      
      case ST_ELBOW_REV:
        if (elbowAngle > ELBOW_LOWER_LIMIT) {
          elbowAngle--;
        }
        setElbowAngle(elbowAngle);
      break;
      
      case ST_WRIST_FWD:
        if (wristAngle < WRIST_UPPER_LIMIT) {
          wristAngle++;
        }
        setWristAngle(wristAngle);
      break;
      
      case ST_WRIST_REV:
        if (wristAngle > WRIST_LOWER_LIMIT) {
          wristAngle--;
        }
        setWristAngle(wristAngle);
      break;
    }
  }
}

void setShoulderAngle(int angle) {
  shoulder.write(angle);
}

void setElbowAngle(int angle) {
  if (angle < ELBOW_LOWER_LIMIT) {
    angle = ELBOW_LOWER_LIMIT;
  }
  else if (angle > ELBOW_UPPER_LIMIT) {
    angle = ELBOW_UPPER_LIMIT;
  }
  angle = map(angle, ELBOW_LOWER_LIMIT,ELBOW_UPPER_LIMIT, 180, 20);
  elbow.write(angle);
}

void setWristAngle(int angle) {
  if (angle < WRIST_LOWER_LIMIT) {
    angle = WRIST_LOWER_LIMIT;
  }
  else if (angle > WRIST_UPPER_LIMIT) {
    angle = WRIST_UPPER_LIMIT;
  }
  angle = map(angle, WRIST_LOWER_LIMIT, WRIST_UPPER_LIMIT, 180,0);
  wrist.write(angle);
}

