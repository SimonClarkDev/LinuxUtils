add_library(libLinuxUtils Clock.cpp CmdLine.cpp CSVCore.cpp CSVFile.cpp FileFind.cpp FileInfo.cpp FileObject.cpp I2CPort.cpp IniFile.cpp RTThread.cpp SerialPort.cpp SocketObject.cpp Thread.cpp)
target_include_directories(libLinuxUtils PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(libLinuxUtils PRIVATE fmt spdlog)

set_property(TARGET libLinuxUtils PROPERTY CXX_STANDARD 20)
target_compile_options(libLinuxUtils PRIVATE -O3 -Werror -Wall -Wextra -pedantic)
