#ifndef SCREEN_CONTROL_H
#define SCREEN_CONTROL_H

#include "commonTypes.h"
#include "ThreadSafeVar.h"
#include <mutex>

namespace client {

class ScreenControl {
  public:
    enum screenState_t {
      IDLE,
      WRITTING
    };

  private:
    ThreadSafeVar<tankbot::arm_t> actualData;
    ThreadSafeVar<tankbot::arm_t> modelData;
    ThreadSafeVar<screenState_t> state;
    std::thread t_;
    ThreadSafeVar<bool> done;

    void mainLoop();

  public:
    ScreenControl();
    ~ScreenControl();

    // Clears the screen
    void clearScreen();

    // Prints the actual and model data to the screen
    void printData();

    void setModelData(tankbot::arm_t data);

    void setActualData(tankbot::arm_t data);

    void startThread();

    void killThread();
};

} // namespace client

#endif