#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <iostream>

namespace tankbot {

#define PARAM_LEN 30
#define RTIMEOUT 50000 // the timeout in ms of the receive function

class Socket {
	public:
		enum ConnectionStatus {
			SUCCESS     = 0,
			FAILED      = -1,
			REC_TIMEOUT = -WSAETIMEDOUT // Receive timeout
		};

	private:
		Socket(); // Disable default constructor
		unsigned int recTimeout;
		char serverPort[PARAM_LEN];
		char serverHost[PARAM_LEN];

		WSADATA wsaData;
		SOCKET clientSocket = INVALID_SOCKET;
		struct addrinfo* result = NULL, *ptr = NULL, hints;

    unsigned int recFlags;

	public:
		Socket(const char* host, const char* port);
		int init();
		int sendData(const char* dataToSend);
		int sendData(const char* dataToSend, int dataLen);

		/**
		* \brief Function to receive data from the socket
		*
		* \param recData - the buffer the received data will be placed in
		* \parma maxLength - the max number of bytes we can receive
		* \return If successful it will return the number of bytes received > 0
		          on a failure it will return an error code that is < 0
		*/
		int receiveData(char* recData, int maxLength);

    /**
    * \brief sets flags for the socket receive call
    * 
    * \param flags the bit mask for the flags
    */
    void setRecFlags(int flags);

    void clearRecFlags();
		int close();
};
} // tankbot

#endif
