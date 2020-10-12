#include <iostream>
#include "TCPClient.h"
#include "commonTypes.h"
#include "Keyboard.h"
#include "Comms.h"
#include "ScreenControl.h"
#include <cstdlib> // for std::system call to use cls to clear screen
#include <thread>
#include <tuple>
#include <mutex>

tankbot::TCPClient tcpClient("localhost", "8080");
tankbot::arm_t currArmState;
tankbot::arm_t armMessage;
std::mutex tcpMutex;
std::mutex msgMutex;
std::mutex feedbackMutex;

void clearScreen() {
  std::system("cls");
}

void arduinoCommunication() {
  bool requested = false;
  int status;
  char requestFeedbackCh[2] = { 0 };
  requestFeedbackCh[0] = tankbot::Comms::ARM_FEEDBACK_CH;
  char buf[sizeof(tankbot::arm_t)];

  while (true) {
    msgMutex.lock();
    tankbot::arm_t msg = armMessage;
    msgMutex.unlock();

    // Send data to tank
    status = tcpClient.sendData(reinterpret_cast<char*>(&msg), sizeof(tankbot::arm_t));
    if (status != tankbot::Socket::SUCCESS) {
      std::cout << "error: " << status << std::endl;
    }
    Sleep(10);
  }
}

int main(int argc, char** argv) {

  client::ScreenControl screen;
  screen.startThread();  

  /*client::Keyboard keyboard;
  const int KEY_INC                 = 1; // when a key is pressed for this many ticks it will increment the joint angle
  char buf[sizeof(tankbot::arm_t)]  = { 0 };
  char requestFeedbackCh[2]         = { 0 };
  requestFeedbackCh[0]              = tankbot::Comms::ARM_FEEDBACK_CH;
  int keyboardCounter[client::Keyboard::NUM_KEYS] = { 0 };
  // Map the keyboard keys to inc/dec joints
  std::tuple<client::Keyboard::key_t, tankbot::joint_id_t, int> keyJointMap[tankbot::NUM_JOINTS*2];
  keyJointMap[0] = std::make_tuple(client::Keyboard::Z_KEY, tankbot::TURRET,    1);
  keyJointMap[1] = std::make_tuple(client::Keyboard::X_KEY, tankbot::TURRET,   -1);
  keyJointMap[2] = std::make_tuple(client::Keyboard::A_KEY, tankbot::SHOULDER,  1);
  keyJointMap[3] = std::make_tuple(client::Keyboard::S_KEY, tankbot::SHOULDER, -1);
  keyJointMap[4] = std::make_tuple(client::Keyboard::Q_KEY, tankbot::ELBOW,     1);
  keyJointMap[5] = std::make_tuple(client::Keyboard::W_KEY, tankbot::ELBOW,    -1);
  keyJointMap[6] = std::make_tuple(client::Keyboard::C_KEY, tankbot::WRIST,     1);
  keyJointMap[7] = std::make_tuple(client::Keyboard::V_KEY, tankbot::WRIST,    -1);
  keyJointMap[8] = std::make_tuple(client::Keyboard::D_KEY, tankbot::CLAW,      1);
  keyJointMap[9] = std::make_tuple(client::Keyboard::F_KEY, tankbot::CLAW,     -1);


  std::cout << "Connecting to host...";
  int status = tcpClient.init();
  if (status == tankbot::Socket::INIT_FAILURE) {
    std::cout << "failed to connect to server" << std::endl;
    return 1;
  }
  std::cout << "Success." << std::endl;

  
  // Send request to server to get current status of arm
  status = tcpClient.sendData(requestFeedbackCh, 2);
  if (status != tankbot::Socket::SUCCESS) {
    std::cout << "Feedback request failed with status: " << status << std::endl;
    return 1;
  }
  // receive initial arm data
  status = tcpClient.receiveData(buf, sizeof(tankbot::arm_t), 5000, 0);
  if (status != tankbot::Socket::SUCCESS) {
    std::cout << "Failed to get init data from tankbot. Status: " << status << std::endl;
    return 1;
  }
  currArmState = *(reinterpret_cast<tankbot::arm_t*>(buf));
  armMessage = currArmState;


  // Start thread to handle tcp communication
  std::thread comm(arduinoCommunication);

  // Keyboard thread:
  while (true) {
    clearScreen();
    feedbackMutex.lock();
    std::cout << "Arm status: " << armMessage << std::endl << std::endl;
    feedbackMutex.unlock();

    // Keyboard SM
    for (std::tuple<client::Keyboard::key_t, tankbot::joint_id_t, int> map : keyJointMap) {
      client::Keyboard::key_t key = std::get<0>(map);
      tankbot::joint_id_t joint   = std::get<1>(map);
      int increment               = std::get<2>(map);
      int keyIndex                = keyboard.keyToIndexHash(key);
      
      if (keyboard.isPressed(key)) {
        if (keyboardCounter[keyIndex] == 0 ||
          keyboardCounter[keyIndex] == KEY_INC
          ) {
          msgMutex.lock();
          armMessage.joints[joint].angle += increment;
          msgMutex.unlock();
        }
        else if (keyboardCounter[keyIndex] > KEY_INC) {
          keyboardCounter[keyIndex] = 0;
        }
        keyboardCounter[keyIndex]++;
      }
      else {
        keyboardCounter[keyIndex] = 0;
      }
    }
  }

  comm.join();
  tcpClient.closeSocket();*/

	return 0;
}