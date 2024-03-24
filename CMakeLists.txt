cmake_minimum_required(VERSION 3.13)
project(Raytracer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native -Rpass=loop-vectorize")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(TBB REQUIRED tbb)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT-REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.14.0
)
FetchContent_GetProperties(googletest)
if (WIN32)
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

add_subdirectory(test)
add_subdirectory(libraries)
add_subdirectory(TestPrograms)
add_subdirectory(3rdParty)


