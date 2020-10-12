#include "Comms.h"
#include "Arduino.h"
#include <iostream>

namespace tankbot {

  TCPServer* Comms::server = 0;
  arm_t Comms::armBuffer;
  drive_t Comms::driveBuffer;
  int Comms::currState;
  int Comms::maxCommSize;
  int Comms::recTimeout;
  bool Comms::armAvail;
  bool Comms::driveAvail;

  enum commState {
    WAIT,
    CLIENT_CONNECTED,
  };

  int Comms::init(const char* port) {
    currState = WAIT;
    maxCommSize = max(sizeof(arm_t), sizeof(drive_t));
    recTimeout = 500; // wait 500ms for a client message before timeout
    server = new TCPServer(port);
    
    int status = server->init();
    if (status != tankbot::Socket::SUCCESS) {
      std::cout << "Comms: server init failed with status: " << status << std::endl;
    }
    armAvail = false;
    driveAvail = false;

    return SUCCESS;
  }

  int Comms::tick() {

    // if client isn't connected yet, attempt to connect
    if (currState == WAIT) {
      int connectStatus = server->clientAvailable();

      if (connectStatus == Socket::SUCCESS) {
        currState = CLIENT_CONNECTED;
      } else {
        return WAITING_FOR_CLIENT;
      }
    }

    // If we get here, then we are connected to a client
    // ... Check if data is available
    char recBuffer[sizeof(arm_t)];
    int status = server->receiveData(recBuffer, sizeof(arm_t), recTimeout, 0);
    // Check what kind of data we have been sent
    if (status == Socket::SUCCESS) {
      if (recBuffer[0] == ARM_CH) {
        armBuffer = *(reinterpret_cast<arm_t*>(recBuffer));
        Comms::armAvail = true;
      } else if (recBuffer[0] == DRIVE_CH) {
        driveBuffer = *(reinterpret_cast<drive_t*>(recBuffer));
        Comms::driveAvail = true;
      } else if (recBuffer[0] == ARM_FEEDBACK_CH) {   // Client is requesting feedback on the current state of the arm
        server->sendData(reinterpret_cast<char*>(&tankbot::Arduino::currArmState), sizeof(arm_t));
      } else if (recBuffer[0] == DRIVE_FEEDBACK_CH) { // Client is requesting feedback on the current state of the drive

      }
    }

    return SUCCESS;
  }

  bool Comms::armAvailable() {
    return armAvail;
  }

  arm_t Comms::readArmBuffer() {
    Comms::armAvail = false;
    return armBuffer;
  }

  bool Comms::driveAvailable() {
    return driveAvail;
  }

  drive_t Comms::readDriveBuffer() {
    Comms::driveAvail = false;
    return driveBuffer;
  }

  int Comms::finalize() {
    if (server != 0)
    {
      server->closeSocket();
      delete server;
    }
    currState = WAIT;
    return SUCCESS;
  }

} // namespace tankbot