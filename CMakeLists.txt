cmake_minimum_required(VERSION 3.22.1)

project(spsc_queue)

set(CMAKE_CXX_STANDARD 17)


add_library(queue_test SHARED queue_test.cpp)