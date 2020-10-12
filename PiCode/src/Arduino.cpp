#include "Arduino.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h> // For atoi

#define AT_PRE         "AT"
#define TURRET_ANGLE   "TS"
#define SHOULDER_ANGLE "SS"
#define ELBOW_ANGLE    "ES"
#define WRIST_ANGLE    "WS"
#define CLAW_ANGLE     "CS"
#define END_CMD        "\n\0"

namespace tankbot {

  arm_t Arduino::currArmState;
  drive_t Arduino::currDriveState;

  const char* Arduino::armCmdPrefix[] = {
    "TS", // Turret Servo Angle
    "SS", // Shoulder Servo Angle
    "ES", // Elbow Servo Angle
    "WS", // Wrist Servo Angle
    "CS", // Claw Server Angle
  };

Arduino::Arduino() 
  : arduino(0)
  , inCmdMode(false)
  , guardTimeMs(1000)
{
}

int Arduino::init(const char* arduinoPort) {
  currArmState.joints[0].id = TURRET;
  currArmState.joints[1].id = SHOULDER;
  currArmState.joints[2].id = ELBOW;
  currArmState.joints[3].id = WRIST;
  currArmState.joints[4].id = CLAW;

  currDriveState.wheels[0].id = LEFT;
  currDriveState.wheels[1].id = RIGHT;

  // read from arduino here and get default joint angles
  arduino = new Serial(arduinoPort);
  int status = enterCmdMode();
  int index = 0;
  // read in all of the default joint angles
  while (status == SUCCESS && index < NUM_JOINTS) {
    status = readJoint(currArmState.joints[index++]);
  }

  return status;
}

void Arduino::tick() {
  if (Comms::armAvailable()) {
    nextArmState = tankbot::Comms::readArmBuffer();
  }

  if (Comms::driveAvailable()) {
    nextDriveState = tankbot::Comms::readDriveBuffer();
  }

  for (joint_t currJoint : nextArmState.joints) {
    if (currJoint.id < NUM_JOINTS && currJoint != currArmState.joints[currJoint.id]) {
      currArmState.joints[currJoint.id].angle = currJoint.angle;
      int status = writeJoint(currJoint);
      if (status == SUCCESS) {
        std::cout << "Wrote: " << currJoint << std::endl;
      } else {
        std::cout << "Arduino write fail: " << status << std::endl;
      }
    }
  }

  for (wheel_t currWheel : nextDriveState.wheels) {
    if (currWheel.id < NUM_WHEELS && currWheel != currDriveState.wheels[currWheel.id]) {
      currDriveState.wheels[currWheel.id] = currWheel;
      writeWheel(currWheel);
    }
  }
}

int Arduino::waitForBytes(const int numBytes, const int timeoutms) {
  int count = 0;
  while (arduino->available() < numBytes && count < timeoutms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    count++;
  }

  if (arduino->available()) {
    return SUCCESS;
  }
  return TIMEOUT;
}

bool Arduino::readResponse() {
  const char* OK = "OK\n";
  const char* PARTIAL_ERROR = "ERR";
  const unsigned int OKLength = 3;
  
  // Read the "OK\n" response
  int status = waitForBytes(OKLength, 50);
  char buf[OKLength + 1] = { 0 }; // make buffer big enough to hold \0
  int bytesRead = arduino->readData(buf, OKLength);
  if (bytesRead != OKLength || strcmp(buf, OK)) {
    if (!strcmp(buf, PARTIAL_ERROR)) {
      int bytesRead = arduino->readData(buf, 3); // Read the last 3 bytes of the "ERROR\n" response
      return false;
    }
  }
  return true;
}

int Arduino::enterCmdMode() {
  const char* commandModeSeq = "+++";
  const unsigned int cmdModeSeqLength = 3;
  const char* OK = "OK\n";
  const unsigned int OKLength = 3;
  const int TEN_SEC = 10;

  // Check if we timed out of command mode
  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = enterTime - now;

  // If we are not in cmd mode, wait for the guard time then enter cmd mode
  if (!inCmdMode || diff.count() > TEN_SEC) {
    std::this_thread::sleep_for(std::chrono::milliseconds(guardTimeMs));
  } else if (inCmdMode) {
    return SUCCESS;
  }

  // write the cmd mode sequence ("+++")
  int status = arduino->writeData(commandModeSeq, cmdModeSeqLength);
  if (!status) {
    return CMD_MODE_FAIL;
  }
  
  // Read the "OK\n" response
  if (readResponse()) {
    inCmdMode = true;
    enterTime = std::chrono::system_clock::now();
    return SUCCESS;
  }
  return CMD_MODE_FAIL;
}

int Arduino::readJoint(joint_t& joint) {
  int status = enterCmdMode();
  if (status != SUCCESS) {
    return status;
  }

  // Generate the query string
  std::string buf = AT_PRE;
  buf += armCmdPrefix[joint.id];
  buf += END_CMD;

  // Write the query to the arduino
  arduino->writeData(buf.c_str(), static_cast<int>(buf.size()));
  
  // Wait for the arduino to respond.
  const int MIN_BYTES = 3;
  status = waitForBytes(MIN_BYTES, 50);
  if (status != SUCCESS) {
    return status;
  }

  // Read the response back
  const int MAX_RESPONSE_BYTES = 5;
  char readData[MAX_RESPONSE_BYTES+1] = { 0 };
  status = arduino->readData(readData, MAX_RESPONSE_BYTES);
  if (status <= 0) {
    return READ_FAIL;
  }
  // Convert the string of bytse read to an int
  joint.angle = atoi(readData);

  return SUCCESS;
}

int Arduino::writeJoint(const joint_t joint) {
  const int OKLength = 3;
  int status = enterCmdMode();
  if (status != SUCCESS) {
    return status;
  }

  std::cout << "Writing: " << joint << " to arduino" << std::endl;
  // Generate command string
  std::string buf(AT_PRE);
  buf += armCmdPrefix[joint.id];
  buf += " ";
  buf += joint.angle;
  buf += END_CMD;

  // Write the data to the arduino
  status = arduino->writeData(buf.c_str(), static_cast<int>(buf.size()));
  if (!status) {
    return WRITE_FAIL;
  }
  
  // Read the OK response
  if (readResponse()) {
    return SUCCESS;
  }

  return WRITE_FAIL;
}

int Arduino::writeWheel(const wheel_t wheel) {
  std::cout << "TODO: Writing: " << wheel << " to arduino " << std::endl;
  return 0;
}

arm_t Arduino::getCurrArmState() {
  return currArmState;
}

Arduino::~Arduino() {
  if (arduino != 0) {
    delete arduino;
    arduino = 0;
  }
}

}