cmake_minimum_required(VERSION 3.13.1)
project(LinuxUtils)

set(SOURCES Source/Clock.cpp Source/CmdLine.cpp Source/CSVCore.cpp Source/CSVFile.cpp Source/FileFind.cpp Source/FileInfo.cpp Source/FileObject.cpp Source/I2CPort.cpp Source/IniFile.cpp Source/RTThread.cpp Source/SerialPort.cpp Source/SocketObject.cpp Source/Thread.cpp )

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC fmt pthread)

target_include_directories(${PROJECT_NAME} PRIVATE Include)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PUBLIC "-Wall")

