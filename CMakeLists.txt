cmake_minimum_required(VERSION 3.14)

project(ServerMultimetr LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(SOURCES Sources/Server.h Sources/Server.cpp
            Sources/Multimetr.h Sources/Multimetr.cpp
            Sources/Channel.h Sources/Channel.cpp
            Sources/Command.h
            Sources/StartMeasure.h Sources/StartMeasure.cpp
            Sources/StopMeasure.h Sources/StopMeasure.cpp
            Sources/SetRange.h Sources/SetRange.cpp
            Sources/GetResult.h Sources/GetResult.cpp
            Sources/GetStatus.h Sources/GetStatus.cpp
            Sources/RangeVoltage.h Sources/RangeVoltage.cpp
            Sources/SocketServer.h Sources/SocketServer.cpp
            Sources/System.h Sources/System.cpp
            Sources/CommunicationCenter.h Sources/CommunicationCenter.cpp
            Sources/Dispatcher.h Sources/Dispatcher.cpp
            )

add_executable(ServerMultimetr
  main.cpp
  ${SOURCES}
)

target_include_directories(ServerMultimetr PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
target_link_libraries(ServerMultimetr PRIVATE Threads::Threads)
