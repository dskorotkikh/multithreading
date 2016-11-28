cmake_minimum_required(VERSION 3.6)
project(multi_tread)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(multi_tread ${SOURCE_FILES})
target_link_libraries(multi_tread Threads::Threads)