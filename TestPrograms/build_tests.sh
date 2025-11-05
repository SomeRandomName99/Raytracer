#!/bin/bash

# Simple standalone build script for BasicTests
# Run this from the Raytracer root directory

set -e

echo "Building BasicTests..."

# Compiler settings
CXX="c++"
CXXFLAGS="-std=c++20 -O0 -g -Wall -Wextra"

# Include directories  
INCLUDES="-I libraries/Utility/include"
INCLUDES="$INCLUDES -I libraries/Geometry/include"
INCLUDES="$INCLUDES -I libraries/Canvas/include"
INCLUDES="$INCLUDES -I libraries/Material/include"
INCLUDES="$INCLUDES -I libraries/Scene/include"
INCLUDES="$INCLUDES -I 3rdParty"

# Source files - add all your library implementations here
SOURCES="libraries/Utility/src/*.cpp"
SOURCES="$SOURCES libraries/Geometry/src/*.cpp"
SOURCES="$SOURCES libraries/Canvas/src/*.cpp"
SOURCES="$SOURCES libraries/Material/src/*.cpp"
SOURCES="$SOURCES libraries/Scene/src/*.cpp"
SOURCES="$SOURCES TestPrograms/BasicTests.cpp"

# Compile
$CXX $CXXFLAGS $INCLUDES $SOURCES -o TestPrograms/BasicTests

echo "Build complete! Run with: ./TestPrograms/BasicTests"
