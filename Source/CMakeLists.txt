add_library(LinuxUtils Clock.cpp CmdLine.cpp CSVCore.cpp CSVFile.cpp FileFind.cpp FileInfo.cpp FileObject.cpp I2CPort.cpp IniFile.cpp RTThread.cpp SerialPort.cpp SocketObject.cpp SPIPort.cpp Thread.cpp)
target_include_directories(LinuxUtils PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(LinuxUtils PRIVATE fmt spdlog)

set_property(TARGET LinuxUtils PROPERTY CXX_STANDARD 20)
target_compile_options(LinuxUtils PRIVATE -O3 -Werror -Wall -Wextra -pedantic)
