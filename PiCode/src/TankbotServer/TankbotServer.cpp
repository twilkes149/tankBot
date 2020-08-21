// TankbotServer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "TCPServer.h"

int main()
{
  tankbot::TCPServer server("8080\0");
  int result = server.init();

  if (result == tankbot::Socket::INIT_FAILURE) {
    std::cout << "init failed" << std::endl;
    return 1;
  }

  result = server.waitForClient(100, 10000);
  if (result == tankbot::Socket::TIMEOUT) {
    std::cout << "timed out" << std::endl;
  } else {
    std::cout << "client connected" << std::endl;
  }
  // send data
  server.sendData("hello\0", 6);

  // wait for response
  char buffer[50] = { 0 };
  // wait 10 seconds for data
  int status = server.receiveData(buffer, 50, 10000, 0);
  if (status == tankbot::Socket::TIMEOUT) {
    std::cout << "received timed out" << std::endl;
  } else if (status == tankbot::Socket::UNKNOWN_ERROR) {
    std::cout << "unknown receive error" << std::endl;
  } else {
    std::cout << "received data: " << buffer << std::endl;
  }

  server.closeSocket();

	return 0;
}
