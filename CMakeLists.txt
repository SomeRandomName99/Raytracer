cmake_minimum_required(VERSION 3.13)
project(Raytracer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_subdirectory(test)
add_subdirectory(libraries)
add_subdirectory(TestPrograms)



