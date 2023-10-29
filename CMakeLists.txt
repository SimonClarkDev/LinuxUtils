cmake_minimum_required(VERSION 3.13.1)
project(LinuxUtils)

set(INCLUDE Headers/Clock.h Headers/CmdLine.h Headers/CSVCore.h Headers/CSVFile.h Headers/DataBlock.h Headers/FileFind.h Headers/FileInfo.h Headers/FileObject.h Headers/I2CPort.h Headers/IniFile.h Headers/ISerialise.h Headers/RTThread.h Headers/SerialPort.h Headers/SocketObject.h Headers/Thread.h)
set(SOURCES Source/Clock.cpp Source/CmdLine.cpp Source/CSVCore.cpp Source/CSVFile.cpp Source/FileFind.cpp Source/FileInfo.cpp Source/FileObject.cpp Source/I2CPort.cpp Source/IniFile.cpp Source/RTThread.cpp Source/SerialPort.cpp Source/SocketObject.cpp Source/Thread.cpp )

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDE})

target_link_libraries(${PROJECT_NAME} PUBLIC fmt pthread)

target_include_directories(${PROJECT_NAME} PRIVATE Headers)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PUBLIC "-Wall")

