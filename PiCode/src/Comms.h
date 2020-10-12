#ifndef __COMMS_H__
#define __COMMS_H__

#include "Socket.h"
#include "TCPServer.h"
#include "commonTypes.h"

namespace tankbot {

  class Comms {
    private:
      static TCPServer* server;       // tcp server

      static arm_t armBuffer;        // most recent data from client
      static drive_t driveBuffer;    // most recent data from client
      static int currState;          // state machine state
      static int maxCommSize;        // size of void* buffer we use to receive data
      static int recTimeout;         // ms to timeout during a receive
      static bool armAvail;
      static bool driveAvail;

    public:

      enum status_t {
        SUCCESS            = 0,
        WAITING_FOR_CLIENT = 1
      };

      enum channels_t {
        ARM_CH            = tankbot::ARM,
        DRIVE_CH          = tankbot::DRIVE,
        ARM_FEEDBACK_CH,
        DRIVE_FEEDBACK_CH
      };

      Comms() = delete; // this is a static class

      static int init(const char* port);

      static int tick();

      /**
      * \brief returns true if we have arm data from the client that we haven't read yet
      */
      static bool armAvailable();

      /**
      * \brief reads the most recent arm data from the client
      */
      static arm_t readArmBuffer();

      /**
      * \brief returns true if we have drive data from the client that we haven't read yet
      */
      static bool driveAvailable();

      /**
      * \brief reads the most recent drive data from the client
      */
      static drive_t readDriveBuffer();

      
      /**
      * \brief function to set the current state of the arm
      */
      static void setArmFeedback(arm_t buf);

      /**
      * \brief function to set the current state of the drive
      */
      static void setDriveFeedback(drive_t buf);
      
      static int finalize();
  };

} // namespace tankbot

#endif