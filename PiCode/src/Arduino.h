#ifndef __ARDUIO_H__
#define __ARDUIO_H__
#include "commonTypes.h"
#include "Comms.h"
#include "Serial.h"
#include <chrono>

namespace tankbot {
class Arduino {
  public:
    enum {
      SUCCESS       = 0,
      CMD_MODE_FAIL = -1,
      TIMEOUT       = -2,
      WRITE_FAIL    = -3,
      READ_FAIL     = -4
    };
  
    static drive_t currDriveState;
    static tankbot::arm_t currArmState;
  
  private:
    static const char* armCmdPrefix[5];

    drive_t nextDriveState;
    arm_t nextArmState;

    Serial* arduino;
    int guardTimeMs;
    bool inCmdMode;
    std::chrono::time_point<std::chrono::system_clock> enterTime;

    int enterCmdMode();

    /*
    * \brief - waits for the arduino to write numBytes, if the bytes aren't 
    *          available after timeoutms then a failed status is returned
    */
    int waitForBytes(const int numBytes, const int timeoutms);

    /*
    * \brief - reads the response after writing data to the arduino
    *
    * \return returns true if response was "OK\r"
    */
    bool readResponse();

    /*
    * \brief - reads the data for a specific joint of the arm
    *
    * \return returns SUCCESS or error code
    */
    int readJoint(joint_t& joint);

  public:
    Arduino();

    int init(const char* arduinoPort);

    void tick();

    arm_t getCurrArmState();

    int writeJoint(const joint_t joint);

    int writeWheel(const wheel_t wheel);

    ~Arduino();
};
}

#endif