cmake_minimum_required(VERSION 3.13)
project(Raytracer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native -Rpass=loop-vectorize")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
enable_testing()

add_subdirectory(external/oneTBB EXCLUDE_FROM_ALL)

add_subdirectory(test)
add_subdirectory(libraries)
add_subdirectory(TestPrograms)
add_subdirectory(3rdParty)


