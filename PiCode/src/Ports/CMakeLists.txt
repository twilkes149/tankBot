#PiCode/src/Ports

if (WIN32)
  # Create library for the TCP server
  add_library(tcp_server "Windows/TCPServer.cpp")
  # Create library for the TCP client
  add_library(tcp_client "Windows/TCPClient.cpp")
  
  include_directories("Ports/Windows")
  target_include_directories (tcp_server PUBLIC "Windows")
  target_include_directories (tcp_client PUBLIC "Windows")
endif(WIN32)

if (UNIX)
  include_directories("Ports/RPi")
endif(UNIX)

target_include_directories( tcp_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( tcp_server PUBLIC "../")
target_include_directories( tcp_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( tcp_client PUBLIC "../")