#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "Socket.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#define WIN32_LEAN_AND_MEAN
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace tankbot {
  class TCPClient : public Socket {
    private:
      WSADATA wsaData;
      SOCKET clientSocket = INVALID_SOCKET;
      struct addrinfo* result = NULL, * ptr = NULL, hints;

      char* serverHost;
      char* serverPort;

    public:
      TCPClient() = delete;

      TCPClient(const char* host, const char* port);

      /**
      * \brief inits the socket connection with the server
      */
      int init();

      /**
      * \brief sends data to the client
      *
      * \param data - the data to send
      * \param dataLen - how many bytes to send
      */
      int sendData(const char* dataToSend, int dataLen);

      /**
      * \brief Non-blocking. Function to receive data from the socket
      *
      * \param recData - the buffer the received data will be placed in
      * \parma maxLength - the max number of bytes we can receive
      * \param flags - any flags to use for this receive
      *
      * \return If successful it will return the number of bytes received > 0
      *         on a failure it will return an error code that is < 0
      *         {TIMEOUT, UNKNOWN_ERROR, SUCCESS}
      */
      int receiveData(char* recData, int maxLength, int flags);

      /**
      * \brief Function to receive data from the socket, will block until data is received
      *        or until timeout is reached
      *
      * \param recData - the buffer the received data will be placed in
      * \parma maxLength - the max number of bytes we can receive
      * \param timeout - the amount of ms to wait before return a timeout failure
      *                  if timeout is 0, will wait forever
      * \param flags - any flags to use for this receive
      *
      * \return If successful it will return the number of bytes received > 0
      *         on a failure it will return an error code that is < 0
      *         {TIMEOUT, UNKNOWN_ERROR, SUCCESS}
      */
      int receiveData(char* recData, int maxLength, int timeout, int flags);

      /**
      * \brief closes the socket connection
      */
      int closeSocket();

      ~TCPClient();
  };
}

#endif