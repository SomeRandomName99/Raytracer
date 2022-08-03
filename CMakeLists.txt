cmake_minimum_required(VERSION 3.13)
project(Raytracer)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

add_library(buildingBlocks STATIC
            ${PROJECT_SOURCE_DIR}/src/buildingBlocks/tuple.cpp)
target_include_directories(buildingBlocks PUBLIC src/buildingBlocks)

add_executable(rayTests src/buildingBlocks/tuple_test.cpp src/buildingBlocks/tuple.cpp)
target_compile_features(rayTests PRIVATE cxx_std_20)
target_include_directories(rayTests PRIVATE src/buildingBlocks)
target_link_libraries(rayTests GTest::GTest GTest::Main)

gtest_discover_tests(rayTests)
