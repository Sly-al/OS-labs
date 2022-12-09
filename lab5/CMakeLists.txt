include_directories(include) 

add_library(dyn1 SHARED src/func1.c)
add_library(dyn2 SHARED src/func2.c)

add_executable(prog11 src/prog1.c include/lab5.h)
add_executable(prog12 src/prog1.c include/lab5.h)
target_link_libraries(prog11 dyn1)
target_link_libraries(prog12 dyn2)

add_executable(prog2 src/prog2.c)
target_link_libraries(prog2 ${CMAKE_DL_LIBS})
