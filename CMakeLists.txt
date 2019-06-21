cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)
include_directories(src/ECS)
link_directories(lib)
link_directories(lib-linux)

file(GLOB_RECURSE SOURCES "src/*.cpp")

project(BunnyMadness)

add_executable(bunny "${SOURCES}" src/glad.c)
target_link_libraries(bunny glfw3)
target_link_libraries(bunny rt)
target_link_libraries(bunny m)
target_link_libraries(bunny dl)
target_link_libraries(bunny X11)
target_link_libraries(bunny pthread)
target_link_libraries(bunny Xrandr)
target_link_libraries(bunny Xinerama)
target_link_libraries(bunny Xxf86vm)
target_link_libraries(bunny Xcursor)
