#include "TCPServer.h"
#include <iostream>

namespace tankbot {

  TCPServer::TCPServer(const char* port)
  : listenSocket(INVALID_SOCKET),
    result(0),
    clientConnected(false)
  {
    int length = (int) strlen(port);
    
    serverPort = new char[length+1]; // make room for the \0
    strcpy(serverPort, port);
  }

  int TCPServer::init() {
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
      return INIT_FAILURE;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(0, serverPort, &hints, &result);
    if (iResult != 0) {
      WSACleanup();
      return INIT_FAILURE;
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
      freeaddrinfo(result);
      WSACleanup();
      return INIT_FAILURE;
    }

    // Setup the TCP listening socket
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      freeaddrinfo(result);
      closesocket(listenSocket);
      WSACleanup();
      return INIT_FAILURE;
    }

    freeaddrinfo(result);

    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
      closesocket(listenSocket);
      WSACleanup();
      return INIT_FAILURE;
    }

    // Change the socket mode on the listening socket from blocking to
    // non-block so the application will not block waiting for requests
    unsigned long NonBlock = 1;
    if (ioctlsocket(listenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR) {
      return INIT_FAILURE;
    }
    return SUCCESS;
  }

  int TCPServer::clientAvailable() {
    // Accept a client socket
    clientSocket = accept(listenSocket, 0, 0);
    
    if (clientSocket == INVALID_SOCKET) {
      if (WSAGetLastError() == WSAEWOULDBLOCK) {
        return NO_CLIENT_AVAILABLE;
      } else {
        return UNKNOWN_ERROR;
      }
    }

    clientConnected = true;
    return SUCCESS;
  }

  int TCPServer::waitForClient(int freq, int timeoutms) {
    int count = 0;

    while (true) {
      // check if there is a pending connection
      int status = clientAvailable();
      if (status == SUCCESS) {
        return SUCCESS;
      }

      // sleep for a bit
      Sleep(freq);
      // increment counter and check if we timed out
      if (timeoutms > 0) {
        count += freq;
        if (count >= timeoutms) {
          return TIMEOUT;
        }
      }
    }
    return SUCCESS;
  }

  int TCPServer::sendData(const char* dataToSend, int dataLen) {
    if (!clientConnected) {
      return NO_CLIENT_AVAILABLE;
    }

    int status = send(clientSocket, dataToSend, dataLen, 0);
    if (status == SOCKET_ERROR) {
      int code = WSAGetLastError();
      std::cout << "send error: " << code << std::endl;
      return SEND_ERROR;
    }
    return SUCCESS;
  }

  int TCPServer::receiveData(char* recData, int maxLength, int flags) {
    int status = recv(clientSocket, recData, maxLength, flags);
    if (status == SOCKET_ERROR) {
      int errorCode = WSAGetLastError();
      if (errorCode == WSAEWOULDBLOCK) {
        return NO_DATA_AVAILABLE;
      } else {
        return UNKNOWN_ERROR;
      }
    }

    return SUCCESS;
  }

  int TCPServer::receiveData(char* recData, int maxLength, int timeout, int flags) {
    int freq = 500; // Number of ms to sleep between receive tries
    int count = 0;

    while (true) {
      // check if there is data to receive
      int status = receiveData(recData, maxLength, flags);
      if (status == SUCCESS || status == UNKNOWN_ERROR) {
        return status;
      }

      // sleep for a bit
      Sleep(freq);
      // increment counter and check if we timed out
      if (timeout > 0) {
        count += freq;
        if (count >= timeout) {
          return TIMEOUT;
        }
      }
    }
  }

  int TCPServer::closeSocket() {
    if (clientConnected) {
      closesocket(clientSocket);
      clientConnected = false;
    }

    closesocket(listenSocket);
    WSACleanup();
    return SUCCESS;
  }

  TCPServer::~TCPServer() {
    closeSocket();
    delete serverPort;
  }
}