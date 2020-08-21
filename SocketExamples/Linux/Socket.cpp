#include "Socket.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h> // for timeval struct
#include <netinet/in.h>
#include <fcntl.h>    // for non-blocking flag
#include <stdint.h>   // for uint16_t
#include <unistd.h>   // for usleep
#define US_TO_MS 1000 // 1ms in us

namespace tankbot {

Socket::Socket(const uint16_t port) {
  serverPort         = port;
  recTimeout.tv_sec  = 0;
  recTimeout.tv_usec = 0;
}


int Socket::init() {
  server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  if (server_fd == 0) {
    perror("socket failure");
    return FAILURE;
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    return FAILURE;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(serverPort);

  // binder server to port
  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    return FAILURE;
  }
  if (listen(server_fd, 3) < 0) {
    return FAILURE;
  }
  return SUCCESS;
}

int Socket::acceptClient() {
  clientSocket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
  
  if (clientSocket < 0) {
    return FAILURE;
  }

  // If we have set a timeout for receiving data, set that option in the client's socket
  if (recTimeout.tv_sec > 0 || recTimeout.tv_usec > 0) {
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &recTimeout, sizeof(recTimeout));
  }
	return SUCCESS;
}

int Socket::waitForClient(const unsigned int freq, const unsigned int timeout) {
  int status = FAILURE;
  unsigned int count = 0;

  do {
    std::cout << "checking queue..." << std::endl;
    status = acceptClient();
    usleep(freq*US_TO_MS);

    if (timeout > 0) {
      count += freq;
      if (count >= timeout) {
        return TIMEOUT;
      }
    }
  } while (status != SUCCESS);

  return SUCCESS;
}

void Socket::setRecTimeoutSec(int sec) {
  recTimeout.tv_sec = sec;
}

void Socket::setRecTimeoutUSec(int usec) {
  recTimeout.tv_usec = usec;
}

int Socket::receiveData(void* recBuf, int maxLen) {
  int flags = 0;

  int status = recv(clientSocket, recBuf, maxLen, flags);
  int error = errno;
  if (status >= 0) {
    return status;
  } else {
    return -1*error;
  }
}

int Socket::sendData(const char* data, int dataLen) {
  int flags = 0;

  return send(clientSocket, data, dataLen, flags);
}

int Socket::closeConnection() {
  int result = shutdown(clientSocket, SHUT_RDWR);
  if (result) {
    return FAILURE;
  }
  close(clientSocket);
  return SUCCESS;
}

int Socket::closeServer() {
  return close(server_fd);
}

}// tankbot