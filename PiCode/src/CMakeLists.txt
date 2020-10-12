if(WIN32)
  add_compile_definitions("WINDOWS")
endif(WIN32)

if(UNIX)
  add_compile_definitions("LINUX")
endif(UNIX)

add_library(server "Ports/Windows/TCPServer.cpp" 
                   "commonTypes.cpp"
                   )

add_library(client "Ports/Windows/TCPClient.cpp" 
                   "commonTypes.cpp" 
                   "Ports/Windows/Keyboard.cpp" 
                   "ThreadSafeVar.cpp" 
                   "Client/ScreenControl.cpp"
                   )

add_library(comms "${CMAKE_CURRENT_SOURCE_DIR}/Comms.cpp" 
                  "commonTypes.cpp" 
                  "Arduino.cpp" 
                  "Ports/Windows/Serial.cpp"
                  )

target_include_directories(server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(client PUBLIC "Ports/Windows")
target_include_directories(client PUBLIC "Client")

target_include_directories(comms PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(comms PUBLIC "Ports/Windows")

target_link_libraries(comms LINK_PUBLIC server)

# Building server code
add_executable(tankbot_server "TankbotServer/TankbotServer.cpp")
target_link_libraries(tankbot_server LINK_PUBLIC server)
target_link_libraries(tankbot_server LINK_PUBLIC comms)

# Building client code
add_executable(tankbot_client "TankbotClient/TankbotClient.cpp")
target_link_libraries(tankbot_client LINK_PUBLIC client)

