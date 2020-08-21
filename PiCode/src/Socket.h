#ifndef __SOCKET_H__
#define __SOCKET_H__

namespace tankbot {
  class Socket {
  public:

    enum ConnectionStatus {
      SUCCESS             = 0,
      INIT_FAILURE        = -1,
      TIMEOUT             = -2,
      NO_CLIENT_AVAILABLE = -3,
      UNKNOWN_ERROR       = -4,
      SEND_ERROR          = -5,
      NO_DATA_AVAILABLE   = -6
    };

    /**
    * \brief inits the socket connection in server or client mode
    */
    virtual int init() = 0;

    /**
    * \brief sends data to the client
    *
    * \param data - the data to send
    * \param dataLen - how many bytes to send
    */
    virtual int sendData(const char* dataToSend, int dataLen) = 0;
    
    /**
    * \brief Function to receive data from the socket
    *
    * \param recData - the buffer the received data will be placed in
    * \parma maxLength - the max number of bytes we can receive
    * \param flags - any flags to use for this receive
    * \return If successful it will return the number of bytes received > 0
              on a failure it will return an error code that is < 0
    */
    virtual int receiveData(char* recData, int maxLength, int flags = 0) = 0;

    /**
    * \brief closes the socket connection
    */
    virtual int closeSocket() = 0;
  };
} // tankbot

#endif
