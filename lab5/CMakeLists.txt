add_library(dyn1 SHARED src/func1.c)
add_library(dyn2 SHARED src/func2.c)
target_include_directories(dyn1 PRIVATE include)
target_include_directories(dyn2 PRIVATE include)

add_executable(prog11 src/prog1.c include/lab5.h)
add_executable(prog12 src/prog1.c include/lab5.h)
target_include_directories(prog11 PRIVATE include)
target_include_directories(prog12 PRIVATE include)
target_link_libraries(prog11 PRIVATE dyn1)
target_link_libraries(prog12 PRIVATE dyn2)

add_executable(prog2 src/prog2.c)
target_link_libraries(prog2 PRIVATE ${CMAKE_DL_LIBS})
target_include_directories(prog2 PRIVATE include)
