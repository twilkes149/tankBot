// TankbotServer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "TCPServer.h"
#include "commonTypes.h"
#include "Comms.h"
#include "Arduino.h"

int main()
{
  tankbot::Comms::init("8080");
  tankbot::Arduino arduino;

  // Init the arduino
  std::cout << "Init Arduino... ";
  int arduinoStatus = arduino.init("\\\\.\\COM22");
  if (arduinoStatus == tankbot::Arduino::CMD_MODE_FAIL) {
    std::cout << "Failed to enter cmd mode" << std::endl;
    return 1;
  } else if (arduinoStatus != tankbot::Arduino::SUCCESS) {
    std::cout << "Failed with status code: " << arduinoStatus << std::endl;
    return 1;
  }
  std::cout << "Success" << std::endl << " Initial Arm state: " << arduino.getCurrArmState() << std::endl;


  bool printedStatus = false;
  while (true) {
    int status = tankbot::Comms::tick();
    if (status == tankbot::Comms::WAITING_FOR_CLIENT) {
      if (!printedStatus) {
        printedStatus = true;
        std::cout << "Waiting for client...";
      }
      Sleep(100);
      continue;
    } else if (printedStatus) {
      printedStatus = false;
      std::cout << "Connected" << std::endl;
    }

    arduino.tick();

    
    /*if (status == tankbot::Comms::WAITING_FOR_CLIENT) {
      std::cout << "Waiting for client..." << std::endl;
      Sleep(100);
    }
    else if (tankbot::Comms::armAvailable()) {
      tankbot::arm_t armData = tankbot::Comms::readArmBuffer();
      std::cout << i++ << ": Arm: " << armData << std::endl;
    }
    else if (tankbot::Comms::driveAvailable()) {
      tankbot::drive_t driveData = tankbot::Comms::readDriveBuffer();
      std::cout << ": Drive: " << driveData << std::endl;
    }*/
  }


	return 0;
}
