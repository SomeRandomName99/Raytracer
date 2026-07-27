#!/bin/bash

# Standalone build script for the SingleTriangle test program.
# Run this from the Raytracer root directory:  ./TestPrograms/build_single_triangle.sh

set -e

echo "Building SingleTriangle..."

# Compiler / TBB settings (GCC + oneTBB submodule, no -fexperimental-library).
source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/tbb_flags.sh"
CXXFLAGS="-std=c++20 -O2 -g -Wall -Wextra"

# All source includes are written relative to the project root (e.g.
# "libraries/Geometry/include/Shape.hpp"), so the project root must be an
# include directory. 3rdParty is added for perlin/stb headers.
INCLUDES="-I . -I 3rdParty $TBB_INCLUDES"

# Every library implementation, EXCEPT libraries/Scene/src/main.cpp, which is a
# stale duplicate of World/Camera and provides no main().
SOURCES=""
SOURCES="$SOURCES libraries/Utility/src/Color.cpp"
SOURCES="$SOURCES libraries/Utility/src/LinearAllocator.cpp"
SOURCES="$SOURCES libraries/Utility/src/Ray.cpp"
SOURCES="$SOURCES libraries/Utility/src/Transformations.cpp"
SOURCES="$SOURCES libraries/Geometry/src/Intersections.cpp"
SOURCES="$SOURCES libraries/Geometry/src/Shape.cpp"
SOURCES="$SOURCES libraries/Canvas/src/Canvas.cpp"
SOURCES="$SOURCES libraries/Material/src/Pattern.cpp"
SOURCES="$SOURCES libraries/Scene/src/Camera.cpp"
SOURCES="$SOURCES libraries/Scene/src/Renderer.cpp"
SOURCES="$SOURCES libraries/Scene/src/World.cpp"
SOURCES="$SOURCES TestPrograms/SingleTriangle.cpp"

# Compile
$CXX $CXXFLAGS $INCLUDES $SOURCES $TBB_LINK -o TestPrograms/SingleTriangle

echo "Build complete! Run with: ./TestPrograms/SingleTriangle"
echo "Output image: SingleTriangle.ppm"
