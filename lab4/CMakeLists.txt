add_executable(lab4 main.c include/lab4.h src/lab4.c)

target_include_directories(lab4 PRIVATE include)

target_link_libraries(lab4 PRIVATE Threads::Threads)