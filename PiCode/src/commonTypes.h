#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__
#include <ostream>

namespace tankbot {

  // constants:
  const int NUM_JOINTS = 5;
  const int NUM_WHEELS = 2;

  enum data_id_t {
    ARM   = 1,
    DRIVE = 2
  };

  enum joint_id_t {
    TURRET     = 0,
    SHOULDER   = 1,
    ELBOW      = 2,
    WRIST      = 3,
    CLAW       = 4,
    UN_INIT    = 5
  };

  typedef enum DRIVE_STATE {
    FWD     = 1,
    FORWARD = 1,
    STOP    = 2,
    REV     = 3,
    REVERSE = 3
  } drive_state_t;

  typedef enum WHEEL_ID {
    LEFT  = 0,
    RIGHT = 1
  } wheel_id_t;

  // Communication structures
  typedef struct joint {
    joint_id_t id; // used to tell which join this data structure is for
    int angle;

    joint();

    bool operator==(const joint& rhs);
    bool operator!=(const joint& rhs);
    joint operator=(const joint& rhs);
  } joint_t;
  
  typedef struct arm {
    data_id_t id;
    joint_t joints[NUM_JOINTS];

    arm();

    arm operator=(const arm& rhs);
  } arm_t;

  typedef struct wheel {
    wheel_id_t id;
    drive_state_t state;
    int speed;

    wheel();

    bool operator==(const wheel& rhs);
    bool operator!=(const wheel& rhs);
    wheel operator=(const wheel& rhs);
  } wheel_t;

  typedef struct drive {
    data_id_t id;
    wheel_t wheels[NUM_WHEELS];

    drive();
  } drive_t;

  // Streaming functions (toString)
  std::ostream& operator<<(std::ostream& os, joint_t j);

  std::ostream& operator<<(std::ostream& os, arm_t arm);

  std::ostream& operator<<(std::ostream& os, wheel_t wheel);

  std::ostream& operator<<(std::ostream& os, drive_t drive);

} // namespace tankbot

#endif