#include "commonTypes.h"

namespace tankbot {
std::ostream& operator<<(std::ostream& os, joint_t j) {
  const char* joints[] = {
    "TURRET  " ,
    "SHOULDER" ,
    "ELBOW   " ,
    "WRIST   " ,
    "CLAW    " ,
    "UN_INIT "};

  os << "Joint{ id: " << joints[j.id] << ", angle: " << j.angle << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, arm_t arm) {
  os << "Arm{ id: " << arm.id << ",\n  Joints[\n";
  for (int i = 0; i < NUM_JOINTS; i++) {
    os << "    " << arm.joints[i] << ",\n";
  }
  os << "] }";

  return os;
}

std::ostream& operator<<(std::ostream& os, wheel_t wheel) {
  const char* wheelState[] = {
    "", // the wheel states start at 1
    "FORWARD" ,
    "STOP   " ,
    "REVERSE" };

  const char* wheelSide[] = {
    "LEFT ",
    "RIGHT" };

  os << "Wheel{ " << wheelSide[wheel.id] << ", state: " << wheelState[wheel.state] << ", speed: " << wheel.speed << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, drive_t drive) {
  os << "Drive{ id: " << drive.id << ",\n  Wheels: [\n";

  for (int i = 0; i < NUM_WHEELS; i++) {
    os << "    " << drive.wheels[i] << ",\n";
  }
  os << "] }";

  return os;
}

joint::joint() {
  id    = UN_INIT;
  angle = 0;
}

bool joint::operator==(const joint& rhs) {
  return (id == UN_INIT || rhs.id == UN_INIT) || (id == rhs.id && angle == rhs.angle);
}

bool joint::operator!=(const joint& rhs) {
  return !joint::operator==(rhs);
}

joint joint::operator=(const joint& rhs) {
  id = rhs.id;
  angle = rhs.angle;

  return *this;
}

arm::arm() {
  id = ARM;
}

arm arm::operator=(const arm& rhs) {
  id = rhs.id;
  for (int i = 0; i < NUM_JOINTS; i++) {
    joints[i] = rhs.joints[i];
  }
  return *this;
}


wheel::wheel() {
  id    = LEFT;
  speed = 0;
  state = STOP;
}

bool wheel::operator==(const wheel& rhs) {
  return (id == rhs.id && speed == rhs.speed && state == rhs.state);
}

bool wheel::operator!=(const wheel& rhs) {
  return !wheel::operator==(rhs);
}

wheel wheel::operator=(const wheel& rhs) {
  id = rhs.id;
  speed = rhs.speed;
  state = rhs.state;

  return *this;
}

drive::drive() {
  id = DRIVE;
}

} // namespace tankbot