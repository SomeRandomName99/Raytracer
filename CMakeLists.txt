cmake_minimum_required(VERSION 3.13)
project(Raytracer)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

add_library(buildingBlocks STATIC
           src/buildingBlocks/tuple.cpp
           src/buildingBlocks/color.cpp
           src/buildingBlocks/canvas.cpp
          src/buildingBlocks/transformations.cpp)
target_compile_features(buildingBlocks PUBLIC cxx_std_20)
target_include_directories(buildingBlocks PUBLIC src/buildingBlocks)

add_executable(rayTests src/buildingBlocks/tests/tuple_test.cpp 
                        src/buildingBlocks/tests/maths_test.cpp
                        src/buildingBlocks/tests/color_test.cpp
                        src/buildingBlocks/tests/canvas_test.cpp
                        src/buildingBlocks/tests/matrix_test.cpp
                        src/buildingBlocks/tests/transformations_test.cpp)
target_compile_features(rayTests PRIVATE cxx_std_20)
target_include_directories(rayTests PUBLIC src/buildingBlocks)
target_link_libraries(rayTests buildingBlocks)
target_link_libraries(rayTests GTest::GTest GTest::Main)

add_executable(projectile src/extras/projectile.cpp)
target_compile_features(projectile PRIVATE cxx_std_20)
target_include_directories(projectile PUBLIC src/buildingBlocks)
target_link_libraries(projectile buildingBlocks)
target_link_libraries(projectile GTest::GTest GTest::Main)

add_executable(clock src/extras/clock.cpp)
target_compile_features(clock PRIVATE cxx_std_20)
target_include_directories(clock PUBLIC src/buildingBlocks)
target_link_libraries(clock buildingBlocks)
target_link_libraries(clock GTest::GTest GTest::Main)

gtest_discover_tests(rayTests)
