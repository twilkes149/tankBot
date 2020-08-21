#include "TCPClient.h"
#include <iostream>

namespace tankbot {
  TCPClient::TCPClient(const char* host, const char* port)
    : clientSocket(INVALID_SOCKET),
    result(0),
    ptr(0)
  {
    int hLength = (int)strlen(host);
    int pLength = (int)strlen(port);

    serverHost = new char[hLength + 1];
    serverPort = new char[pLength + 1]; // make room for the \0

    strcpy(serverHost, host);
    strcpy(serverPort, port);
  }


  int TCPClient::init() {
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
      return INIT_FAILURE;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(serverHost, serverPort, &hints, &result);
    if (iResult != 0) {
      WSACleanup();
      return INIT_FAILURE;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

      // Create a SOCKET for connecting to server
      clientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        return INIT_FAILURE;
      }
      // Set the timeout value for when we receive
      //setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*) & recTimeout, sizeof(recTimeout));

      // Connect to server.
      iResult = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
      if (iResult == SOCKET_ERROR) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        continue;
      }
      break;
    }

    freeaddrinfo(result);
    // Make sure that we actually connected
    if (clientSocket == INVALID_SOCKET) {
      WSACleanup();
      return INIT_FAILURE;
    }
    return SUCCESS;
  }

  int TCPClient::sendData(const char* dataToSend, int dataLen) {
    if (clientSocket == SOCKET_ERROR) {
      return NO_CLIENT_AVAILABLE;
    }

    int status = send(clientSocket, dataToSend, dataLen, 0);
    if (status == SOCKET_ERROR) {
      return SEND_ERROR;
    }
    return SUCCESS;
  }

  int TCPClient::receiveData(char* recData, int maxLength, int flags) {
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

  int TCPClient::receiveData(char* recData, int maxLength, int timeout, int flags) {
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

  int TCPClient::closeSocket() {
    if (clientSocket != SOCKET_ERROR) {
      closesocket(clientSocket);
      clientSocket = SOCKET_ERROR;
    }

    WSACleanup();
    return SUCCESS;
  }

  TCPClient::~TCPClient() {
    this->closeSocket();
    delete[] serverHost;
    delete[] serverPort;
  }
} // tankbot namespace