#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <iostream>   // for cout

namespace tankbot {

#define PARAM_LEN 30

class Socket {
  public:
    enum ConnectionStatus {
      SUCCESS = 0,
      FAILURE = 1,
      TIMEOUT = 2,
      REC_TIMEOUT = -1 * EAGAIN,
    };
	private:
    uint16_t serverPort;

		int server_fd, clientSocket, valread;
		struct sockaddr_in address;
		int opt = 1;
		int addrlen = sizeof(address);

    struct timeval recTimeout;

	public:
		Socket(const uint16_t port);
		int init();

    /**
    * \brief creates a socket with an incoming client
    *        Non-blocking; will return a fail status if no clients available
    */
    int acceptClient();

    /**
    * \brief blocks until a client attempts to connect
    *
    * \param freq - the rate in ms at which to check for an incoming client
    * \param timeout - optional timeout in ms to exit. Will return a fail status if it times out
    *                  timeout of 0 means no timeout
    */
    int waitForClient(const unsigned int freq, const unsigned int timeout = 0);

    /**
    * \brief sends data to the client
    *
    * \param data - the data to send
    * \param dataLen - how many bytes to send
    */
		int sendData(const char* data, int dataLen);


    /**
    * \brief sets the timeout for receiving data in useconds
    *
    * \asumptions - this should be called before accepting a connection with a client
    */
    void setRecTimeoutUSec(int usec);

    /**
    * \brief sets the timeout for receiving data in seconds
    *
    * \asumptions - this should be called before accepting a connection with a client
    */
    void setRecTimeoutSec(int sec);

    /**
    * \brief retrieves data from the client
    *
    * \param recBuf - buffer for the received data
    * \param maxLen - max length of data we can receive
    * \return - returns the number of bytes read, or <0 if an error occured
    */
		int receiveData(void* recBuf, int maxLen);

    /**
    * \brief closes the socket connection with the client
    */
		int closeConnection();

    /**
    * \brief closes the server's listening socket
    */
    int closeServer();
};
} // tankbot

#endif

