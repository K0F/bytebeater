cmake_minimum_required(VERSION 2.8)
project(Bytebeater)

find_package(SDL REQUIRED)
find_package(SDL_ttf)
find_package(Threads)

include_directories("include/")
include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLTTF_INCLUDE_DIR})


add_executable(bytebeater "src/bytebeater.c")
target_link_libraries(bytebeater ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(bytebeater ${SDL_LIBRARY})
target_link_libraries(bytebeater ${SDLTTF_LIBRARY})

