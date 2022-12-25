add_executable(client include/node.h src/distribution.cpp)
target_include_directories(client PRIVATE include)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(client zmq)

add_executable(server src/computing.cpp)
target_link_libraries(server zmq)