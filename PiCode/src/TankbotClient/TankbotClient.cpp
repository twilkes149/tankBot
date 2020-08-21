#include <iostream>
#include "TCPClient.h"

int main(int argc, char** argv) {
  tankbot::TCPClient client("localhost", "8080");

  int status = client.init();
  if (status == tankbot::Socket::INIT_FAILURE) {
    std::cout << "failed to connect to server" << std::endl;
    return 1;
  }

  //receive data
  char buffer[50] = {0};
  status = client.receiveData(buffer, 50, 10000, 0);
  if (status == tankbot::Socket::SUCCESS) {
    std::cout << "received data: " << buffer << std::endl;
  } else if (status == tankbot::Socket::TIMEOUT) {
    std::cout << "client receive timeout" << std::endl;
  } else {
    std::cout << "client receive unknown error" << std::endl;
  }

  //send data
  status = client.sendData("hi\r\n\0", 5);
  std::cout << "send status: " << status << std::endl;

  client.closeSocket();

	return 0;
}