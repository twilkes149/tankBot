#/PiCode/src
add_subdirectory("Ports")

if( ${BUILD_SERVER} )
  add_executable (tankbotServer "TankbotServer/TankbotServer.cpp")
endif( ${BUILD_SERVER} )

if( ${BUILD_CLIENT} )
  add_executable (tankbotClient "TankbotClient/TankbotClient.cpp")
endif( ${BUILD_CLIENT} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("Ports")

if (WIN32)
  include_directories("Ports/Windows")
endif(WIN32)

if (UNIX)
  include_directories("Ports/RPi")
endif(UNIX)


if ( ${BUILD_SERVER} )
  # Link to the tcp created libraries and the opencv libraries
  target_link_libraries (tankbotServer LINK_PUBLIC tcp_server ${OpenCV_LIBS})
endif ( ${BUILD_SERVER} )

if( ${BUILD_CLIENT} )
  # Link to the tcp created libraries and the opencv libraries
  target_link_libraries (tankbotClient LINK_PUBLIC tcp_client ${OpenCV_LIBS})
endif( ${BUILD_CLIENT} )
