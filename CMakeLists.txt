cmake_minimum_required(VERSION 3.13.0)
project(LinuxUtils VERSION 0.1.0)

add_subdirectory(Source/)

target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Werror -Wall -Wextra -pedantic)
target_include_directories(${PROJECT_NAME} PRIVATE Include)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt pthread)

