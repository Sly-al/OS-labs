cmake_minimum_required(VERSION 3.24.2)

add_executable(parent.out
        main.c
        src/parent.c
        include/parent.h)
        
target_include_directories(parent.out PRIVATE include)

add_executable(child.out
        src/child.c
        )
        
add_dependencies(parent.out child.out)
