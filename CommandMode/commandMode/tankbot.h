#ifndef __TANKBOT__
#define __TANKBOT__
#include "tankbot_hardware.h"

extern int config_getSetConfig(char c1, char c2, void* param);

int getSetShoulderServoAngle(void* param);
int getSetElbowServoAngle(void* param);
int getSetShoulderAngle(void* param);
int getSetElbowAngle(void* param);

enum driveDir {
  FWD   = 0,
  REV   = 2,
};

// ********************************* HELPER FUNCTIONS **************************
void drivePWMTick() {
  left_wheel.Software_PWM_tick();
  right_wheel.Software_PWM_tick();
}

void setLeftSpeed(int speed) {
  //analogWrite(LEFT_PWM_PIN, speed);
  left_wheel.Software_PWM_setPercent(speed);
  if (speed == 0) {
    left_wheel.Software_PWM_stop();
  } else {
    left_wheel.Software_PWM_start();
  }
}

void setRightSpeed(int speed) {
  //analogWrite(RIGHT_PWM_PIN, speed);
  right_wheel.Software_PWM_setPercent(speed);
  if (speed == 0) {
    right_wheel.Software_PWM_stop();
  } else {
    right_wheel.Software_PWM_start();
  }
}

void setLeftDriveDir(int dir) {
  switch (dir) {
    case FWD:
      digitalWrite(LEFT_A_PIN, HIGH);
      digitalWrite(LEFT_B_PIN, LOW);
    break;

    case REV:
      digitalWrite(LEFT_A_PIN, LOW);
      digitalWrite(LEFT_B_PIN, HIGH);
    break;

    default:
      digitalWrite(LEFT_A_PIN, LOW);
      digitalWrite(LEFT_B_PIN, LOW);
    break;
  }
}

void setRightDriveDir(int dir) {
  switch (dir) {
    case FWD:
      digitalWrite(RIGHT_A_PIN, LOW);
      digitalWrite(RIGHT_B_PIN, HIGH);
    break;

    case REV:
      digitalWrite(RIGHT_A_PIN, HIGH);
      digitalWrite(RIGHT_B_PIN, LOW);
    break;

    default:
      digitalWrite(RIGHT_A_PIN, LOW);
      digitalWrite(RIGHT_B_PIN, LOW);
    break;
  }
}

// ********************************* CONFIG FUNCTIONS **************************
// ********************************* DRIVE FUNCTIONS ***************************
int getSetLeftDir(void* param) {
  if (param == NULL) {
    return config.lw_dir;
  } else {
    int result = config_getSetConfig('L', 'D', param);

    setLeftDriveDir(config.lw_dir);
    return result;    
  }
}

int getSetRightDir(void* param) {
  if (param == NULL) {
    return config.rw_dir;
  } else {
    int result = config_getSetConfig('R', 'D', param);

    setRightDriveDir(config.rw_dir);
    return result;    
  }
}

int getSetRightSpeed(void* param) {
  if (param == NULL) {
    return config.rw_speed;
  } else {
    int result = config_getSetConfig('R', 'S', param);
    
    setRightSpeed(config.rw_speed);
    return result;
  }
}

int getSetLeftSpeed(void* param) {
  if (param == NULL) {
    return config.lw_speed;
  } else {
    int result = config_getSetConfig('L', 'S', param);
    
    setLeftSpeed(config.lw_speed);
    return result;
  }
}

// ******************************** ARM FUNCTIONS ******************************
int getArmX(void* param) {
  return config.armX;
}

int getArmY(void* param) {
  return config.armY;
}

int getSetShoulderAngle(void* param) {
  if (param == NULL) {
    return config.shoulderAngle;
  } else {
    int result = config_getSetConfig('S','A', param);
    
    config.shoulderServoAngle = mapShoulderAngleToShoulderServo(config.shoulderAngle);
    shoulder_motor.write(config.shoulderServoAngle);    
    updateXY();
    return result;
  }
}

int getSetElbowAngle(void* param) {
  if (param == NULL) {
    return config.elbowAngle;
  } else {
    int result = config_getSetConfig('E','A', param);

    config.elbowServoAngle = mapElbowAngleToElbowServo(config.elbowAngle);
    elbow_motor.write(config.elbowServoAngle);    
    updateXY();
    return result;
  }
}

int getSetTurretServoAngle(void* param) {
  if (param == NULL) {
    return config.turretServoAngle;
  } else {
    int result = config_getSetConfig('T','S', param);
    
    turret_motor.write(config.turretServoAngle);
    return result;
  }
}

int getSetShoulderServoAngle(void* param) {
  if (param == NULL) {
    return config.shoulderServoAngle;
  } else {
    int result = config_getSetConfig('S','S', param);

    config.shoulderAngle = mapShoulderServoToShoulderAngle(config.shoulderServoAngle);
    shoulder_motor.write(config.shoulderServoAngle); 
    updateXY();   
    return result;
  }
}

int getSetElbowServoAngle(void* param) {
  if (param == NULL) {
    return config.elbowServoAngle;
  } else {
    int result = config_getSetConfig('E','S', param);

    config.elbowAngle = mapElbowServoToElbowAngle(config.elbowServoAngle);
    elbow_motor.write(config.elbowServoAngle);
    updateXY();
    return result;
  }
}

int getSetWristServoAngle(void* param) {
  if (param == NULL) {
    return config.wristServoAngle;
  } else {
    int result = config_getSetConfig('W','S', param);
        
    wrist_motor.write(config.wristServoAngle);
    return result;
  }
}

int getSetClawServoAngle(void* param) {
  if (param == NULL) {
    return config.clawServoAngle;
  } else {
    int result = config_getSetConfig('C','S', param);
        
    claw_motor.write(config.clawServoAngle);
    return result;
  }
}

#endif
