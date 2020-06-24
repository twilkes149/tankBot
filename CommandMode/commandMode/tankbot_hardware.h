#ifndef __TANKBOT_HW__
#define __TANKBOT_HW__
#include <Servo.h>
#include "software_pwm.h"

// ************************* PIN DEFINITIONS ************************
#define TURRET_PIN    8
#define SHOULDER_PIN  9
#define ELBOW_PIN     10
#define WRIST_PIN     11
#define CLAW_PIN      12
#define LEFT_PWM_PIN  2
#define LEFT_A_PIN    5
#define LEFT_B_PIN    4
#define RIGHT_A_PIN   7
#define RIGHT_B_PIN   6
#define RIGHT_PWM_PIN 3

// ************************** ARM JOINT ANGLE LIMITS ***********************
#define SHOULDER_ANGLE_MIN 0
#define SHOULDER_ANGLE_MAX 107
#define ELBOW_ANGLE_MIN 74
#define ELBOW_ANGLE_MAX 239

// ************************** SERVO ANGLE LIMITS ***************************
#define SHOULDER_LOWER_LIMIT 0
#define SHOULDER_UPPER_LIMIT 130
#define ELBOW_LOWER_LIMIT    0
#define ELBOW_UPPER_LIMIT    180
#define WRIST_LOWER_LIMIT    0
#define WRIST_UPPER_LIMIT    180

// ************************* ARM LENGTHS ****************************
#define SHOULDER_LEN 80
#define ELBOW_LEN    80
#define CLAW_LEN     130

// ************************* SERVO OBJECTS **************************
Servo turret_motor;
Servo shoulder_motor;
Servo elbow_motor;
Servo wrist_motor;
Servo claw_motor;

Software_PWM left_wheel (LEFT_PWM_PIN, config.lw_speed);
Software_PWM right_wheel(RIGHT_PWM_PIN, config.rw_speed);
void setLeftSpeed(int speed);
void setRightSpeed(int speed);

void tankbot_hardwareInit() {
  // attach the servos
  turret_motor.attach(TURRET_PIN);
  shoulder_motor.attach(SHOULDER_PIN);
  elbow_motor.attach(ELBOW_PIN);
  wrist_motor.attach(WRIST_PIN);
  claw_motor.attach(CLAW_PIN);

  // Pins for the drive wheel h-bridge
  pinMode(LEFT_PWM_PIN, OUTPUT);
  pinMode(LEFT_A_PIN, OUTPUT);
  pinMode(LEFT_B_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(RIGHT_A_PIN, OUTPUT);
  pinMode(RIGHT_B_PIN, OUTPUT);

  digitalWrite(LEFT_PWM_PIN, LOW);
  digitalWrite(RIGHT_PWM_PIN, LOW);

  digitalWrite(LEFT_A_PIN, LOW);
  digitalWrite(LEFT_B_PIN, HIGH);
  digitalWrite(RIGHT_A_PIN, HIGH);
  digitalWrite(RIGHT_B_PIN, LOW);

  left_wheel.Software_PWM_init();
  right_wheel.Software_PWM_init();

  setRightSpeed(config.rw_speed);
  setLeftSpeed(config.lw_speed);
  
  turret_motor.write(config.turretServoAngle);
  shoulder_motor.write(config.shoulderServoAngle);
  elbow_motor.write(config.elbowServoAngle);
  wrist_motor.write(config.wristServoAngle);
  claw_motor.write(config.clawServoAngle);

  // Calculate the x,y position of the arm based on all of the default angles
  // Use the following coordinate system:
  //              ^
  //              |
  //              y
  //              |
  //              |
  //              |
  //              |   ______
  //              |  /      \
  //              | /        \
  //              |/_ _ _ _ _ _ _ _ _  _ _ x ->
  //     ________/___\_
  //    /              \
  //    \______________/
  //--------------------------------------------------------------
}
// Maps the value we write to the shouder servo to degrees of the shoulder relative to the ground
int mapShoulderServoToShoulderAngle(int shoulderServo) {
  return map(shoulderServo, SHOULDER_UPPER_LIMIT, SHOULDER_LOWER_LIMIT, SHOULDER_ANGLE_MIN, SHOULDER_ANGLE_MAX);
}

int mapShoulderAngleToShoulderServo(int shoulderAngle) {
  return map(shoulderAngle, SHOULDER_ANGLE_MIN, SHOULDER_ANGLE_MAX, SHOULDER_UPPER_LIMIT, SHOULDER_LOWER_LIMIT);
}

// Maps the value we write to the elbow to the degrees of the elbow relative to the shoulder
int mapElbowServoToElbowAngle(int elbowServo) {
  return map(elbowServo, ELBOW_UPPER_LIMIT, ELBOW_LOWER_LIMIT, ELBOW_ANGLE_MIN, ELBOW_ANGLE_MAX);
}

int mapElbowAngleToElbowServo(int elbowAngle) {
  return map(elbowAngle, ELBOW_ANGLE_MIN, ELBOW_ANGLE_MAX, ELBOW_UPPER_LIMIT, ELBOW_LOWER_LIMIT);
}

#endif
