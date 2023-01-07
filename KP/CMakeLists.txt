add_executable(kp include/parser.h include/graph.h src/plan.cpp)

target_include_directories(kp PRIVATE include)

target_link_libraries(kp PRIVATE PRIVATE Threads::Threads)
target_link_libraries(kp PRIVATE jsoncpp)