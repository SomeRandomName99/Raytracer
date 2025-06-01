cmake_minimum_required(VERSION 3.13)
project(Raytracer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# Add compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Rpass=loop-vectorize")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize -fopt-info-vec")
endif()

add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
enable_testing()

add_subdirectory(external/oneTBB EXCLUDE_FROM_ALL)

add_subdirectory(test)
add_subdirectory(libraries)
add_subdirectory(TestPrograms)
add_subdirectory(3rdParty)


