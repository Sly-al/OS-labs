project(KP)
cmake_minimum_required(VERSION 3.25)

add_executable(kp include/parser.h src/test.cpp)

target_include_directories(kp PRIVATE include)

target_link_libraries(kp PRIVATE pthread)
target_link_libraries(kp PRIVATE jsoncpp)