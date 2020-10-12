#include "ScreenControl.h"
#include <iostream> // for std::system
#include <chrono>
#include <thread>

namespace client {

  ScreenControl::ScreenControl() {
    state.write(WRITTING);
    done.write(false);
  }

  void ScreenControl::clearScreen() {
    std::system("cls");
  }

  void ScreenControl::printData() {
    tankbot::arm_t actual = actualData.read();
    tankbot::arm_t model  = modelData.read();
    
    std::cout << "Command: " << model << std::endl << std::endl
      << "Actual: " << actual << std::endl;
  }

  void ScreenControl::setActualData(tankbot::arm_t data) {
    actualData.write(data);
    state.write(WRITTING);
  }

  void ScreenControl::setModelData(tankbot::arm_t data) {
    modelData.write(data);
    state.write(WRITTING);
  }

  void ScreenControl::mainLoop() {
    
    while (!done.read())
    {
      if (state.read() == WRITTING) {
        clearScreen();
        printData();
        state.write(IDLE);
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  void ScreenControl::startThread() {
    t_ = std::thread(&ScreenControl::mainLoop, this);
  }

  void ScreenControl::killThread() {
    done.write(true);
    t_.join();
  }

  ScreenControl::~ScreenControl() {
    killThread();
  }

} // namespace client