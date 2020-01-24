#ifndef __TANKBOT_HW__
#define __TANKBOT_HW__
#include <Servo.h>

// ************************* PIN DEFINITIONS ************************
#define TURRET_PIN 7

// ************************* SERVO OBJECTS **************************
Servo turret_motor;

void tankbot_hardwareInit() {
  turret_motor.attach(TURRET_PIN);
}

#endif
