cmake_minimum_required(VERSION 3.22.1)

project(spsc_queue)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party/googletest)
add_library(queue_wrapper SHARED queue_wrapper.cpp)

enable_testing()

add_executable(queue_test test.cpp)
target_link_libraries(queue_test GTest::gtest_main)
target_link_libraries(queue_test queue_wrapper)

include(GoogleTest)
gtest_discover_tests(queue_test)