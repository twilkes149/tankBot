#ifndef __TANKBOT_HW__
#define __TANKBOT_HW__
#include <Servo.h>

// ************************* PIN DEFINITIONS ************************
#define TURRET_PIN 8
#define SHOULDER_PIN 9
#define ELBOW_PIN 10
#define WRIST_PIN 11
#define CLAW_PIN 12

#define LEFT_PWM_PIN 7
#define LEFT_A_PIN 6
#define LEFT_B_PIN 5
#define RIGHT_A_PIN 4
#define RIGHT_B_PIN 3
#define RIGHT_PWM_PIN 2

// ************************* SERVO OBJECTS **************************
Servo turret_motor;
Servo shoulder_motor;
Servo elbow_motor;
Servo wrist_motor;
Servo claw_motor;

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
}

#endif
