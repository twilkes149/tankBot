#/PiCode
cmake_minimum_required (VERSION 3.7.0)
project ("TankbotServer")

option(BUILD_CLIENT "Build the client code for windows" ON)
option(BUILD_SERVER "Build the server code for linux" ON)

message(STATUS "Configuration:")
message(STATUS "BUILD_CLIENT:  " ${BUILD_CLIENT})
message(STATUS "BUILD_SERVER:  " ${BUILD_SERVER})

if (WIN32)
  set(OpenCV_DIR "C:\\opencv\\build\\x64\\vc15\\lib" )  
endif (WIN32)

if (UNIX)
  set(OpenCV_DIR "/home/pi/opencv/build" )
endif (UNIX)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add the code
add_subdirectory("src")
