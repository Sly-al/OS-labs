# project(KP)
# cmake_minimum_required(VERSION 3.25)

add_executable(kp include/parser.h include/graph.h src/test.cpp)

target_include_directories(kp PRIVATE include)

target_link_libraries(kp PRIVATE PRIVATE Threads::Threads)
target_link_libraries(kp PRIVATE jsoncpp)