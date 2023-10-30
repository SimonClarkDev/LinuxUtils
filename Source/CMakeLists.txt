add_library(LinuxUtilsLib Clock.cpp CmdLine.cpp CSVCore.cpp CSVFile.cpp FileFind.cpp FileInfo.cpp FileObject.cpp I2CPort.cpp IniFile.cpp RTThread.cpp SerialPort.cpp SocketObject.cpp Thread.cpp)
target_include_directories(LinuxUtilsLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(LinuxUtilsLib PRIVATE fmt spdlog)

set_property(TARGET LinuxUtilsLib PROPERTY CXX_STANDARD 20)
target_compile_options(LinuxUtilsLib PRIVATE -O3 -Werror -Wall -Wextra -pedantic)
