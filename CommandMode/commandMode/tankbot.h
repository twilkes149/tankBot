#ifndef __TANKBOT__
#define __TANKBOT__
#include "tankbot_hardware.h"

extern int config_getSetConfig(char c1, char c2, void* param);

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

void setTurretAngle(int angle) {
  turret_motor.write(angle);
}

void setShoulderAngle(int angle) {
  shoulder_motor.write(angle);
}

void setElbowAngle(int angle) {
  elbow_motor.write(angle);
}

void setWristAngle(int angle) {
  wrist_motor.write(angle);
}

void setClawAngle(int angle) {
  claw_motor.write(angle);
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
int getSetTurretAngle(void* param) {
  if (param == NULL) {
    return config.turretAngle;
  } else {
    int result = config_getSetConfig('T','A', param);
    
    setTurretAngle(config.turretAngle);
    return result;
  }
}

int getSetShoulderAngle(void* param) {
  if (param == NULL) {
    return config.shoulderAngle;
  } else {
    int result = config_getSetConfig('S','A', param);
    
    setShoulderAngle(config.shoulderAngle);
    return result;
  }
}

int getSetElbowAngle(void* param) {
  if (param == NULL) {
    return config.elbowAngle;
  } else {
    int result = config_getSetConfig('E','A', param);
    
    setElbowAngle(config.elbowAngle);
    return result;
  }
}

int getSetWristAngle(void* param) {
  if (param == NULL) {
    return config.wristAngle;
  } else {
    int result = config_getSetConfig('W','A', param);
    
    setWristAngle(config.wristAngle);
    return result;
  }
}

int getSetClawAngle(void* param) {
  if (param == NULL) {
    return config.clawAngle;
  } else {
    int result = config_getSetConfig('C','A', param);
    
    setClawAngle(config.clawAngle);
    return result;
  }
}

#endif
