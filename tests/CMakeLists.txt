include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)

# LAB 2 tests
add_executable(lab2_test lab2_test.cpp ../lab2/src/parent.c ../lab2/include/parent.h)

target_link_libraries(lab2_test PRIVATE GTest::GTest)
target_include_directories(lab2_test PRIVATE ../lab2/include)

add_test(lab2_test lab2_test)

# LAB 3 tests
add_executable(lab3_test lab3_test.cpp ../lab3/include/lab3.h ../lab3/src/lab3.cpp ../lab3/include/utils.h)

target_link_libraries(lab3_test PRIVATE GTest::GTest)
target_include_directories(lab3_test PRIVATE ../lab3/include)

add_test(lab3_test lab3_test)

# LAB 4 tests
add_executable(lab4_test lab4_test.cpp ../lab4/include/lab4.h ../lab4/src/lab4.c )

target_link_libraries(lab4_test PRIVATE GTest::GTest)
target_include_directories(lab4_test PRIVATE ../lab4/include)

add_test(lab4_test lab4_test)

# LAB 5 tests
add_executable(lab5_test1 lab5_test1.cpp ../lab5/include/lab5.h ../lab5/src/func1.c )

target_link_libraries(lab5_test1 PRIVATE GTest::GTest)
target_include_directories(lab5_test1 PRIVATE ../lab5/include)

add_test(lab5_test1 lab5_test1)

add_executable(lab5_test2 lab5_test2.cpp ../lab5/include/lab5.h ../lab5/src/func2.c )

target_link_libraries(lab5_test2 PRIVATE GTest::GTest)
target_include_directories(lab5_test2 PRIVATE ../lab5/include)

add_test(lab5_test2 lab5_test2)