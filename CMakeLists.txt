cmake_minimum_required(VERSION 2.8)
project(Bytebeater)

find_package(SDL REQUIRED)
find_package(Threads)

include_directories("src/")
include_directories(${SDL_INCLUDE_DIR})


add_executable(bytebeater "src/bytebeater.c")
target_link_libraries(bytebeater ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(bytebeater ${SDL_LIBRARY})

