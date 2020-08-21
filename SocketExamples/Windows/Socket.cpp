#include "Socket.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define WIN32_LEAN_AND_MEAN
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace tankbot {
    Socket::Socket(const char* host, const char* port) {
        strcpy(serverHost, host);
        strcpy(serverPort, port);
        recTimeout = RTIMEOUT; // 5000ms timeout
        recFlags = 0;
    }

    int Socket::init() {
        // Initialize Winsock
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cout << "WSAStartup failed with error: " << iResult << std::endl;
            return FAILED;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(serverHost, serverPort, &hints, &result);
        if (iResult != 0) {
            std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
            WSACleanup();
            return FAILED;
        }

        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            clientSocket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (clientSocket == INVALID_SOCKET) {
                std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
                WSACleanup();
                return FAILED;
            }
            // Set the timeout value for when we receive
            setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*) & recTimeout, sizeof(recTimeout));

            // Connect to server.
            iResult = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
              std::cout << "socket failed to connect" << std::endl;
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Unable to connect to server!" << std::endl;
            WSACleanup();
            return FAILED;
        }
        return SUCCESS;
    }

    int Socket::sendData(const char* dataToSend, int len) {
        // Send the data
        int iResult = send(clientSocket, dataToSend, len, 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return FAILED;
        }
        return SUCCESS;
    }

    int Socket::sendData(const char* dataToSend) {
        return sendData(dataToSend, (int)strlen(dataToSend));
    }

    void Socket::clearRecFlags() {
      this->recFlags = 0;
    }

    void Socket::setRecFlags(int flags) {
      this->recFlags |= flags;
    }

    int Socket::receiveData(char* recData, int maxLength) {
        int iResult = recv(clientSocket, recData, maxLength, recFlags);   

        if (iResult == 0) {
            std::cout << "Connection closed" << std::endl;
        } else if (iResult < 0) {
            std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
            return -1 * WSAGetLastError();
        }
        return iResult;
    }

    int Socket::close() {
        int iResult = shutdown(clientSocket, SD_BOTH);
        
        if (iResult == SOCKET_ERROR) {
            std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return FAILED;
        } else {
            closesocket(clientSocket);
            WSACleanup();
        }
        return SUCCESS;
    }
} // tankbot