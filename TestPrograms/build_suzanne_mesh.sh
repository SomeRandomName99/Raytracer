#!/bin/bash

# Standalone build script for the SuzanneMesh test program.
# Run this from the Raytracer root directory:  ./TestPrograms/build_suzanne_mesh.sh

set -e

echo "Building SuzanneMesh..."

# Compiler / TBB settings (GCC + oneTBB submodule, no -fexperimental-library).
source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/tbb_flags.sh"
CXXFLAGS="-std=c++20 -O2 -g -Wall -Wextra"

# All source includes are written relative to the project root (e.g.
# "libraries/Geometry/include/Shape.hpp"), so the project root must be an
# include directory. 3rdParty is added for perlin/stb/tinyobjloader headers.
INCLUDES="-I . -I 3rdParty $TBB_INCLUDES"

# Every library implementation, EXCEPT libraries/Scene/src/main.cpp, which is a
# stale duplicate of World/Camera and provides no main().
SOURCES="libraries/Utility/src/*.cpp libraries/Geometry/src/*.cpp libraries/Canvas/src/*.cpp libraries/Material/src/*.cpp libraries/Scene/src/Camera.cpp libraries/Scene/src/Renderer.cpp libraries/Scene/src/World.cpp TestPrograms/SuzanneMesh.cpp"

# Compile
$CXX $CXXFLAGS $INCLUDES $SOURCES $TBB_LINK -o TestPrograms/SuzanneMesh

echo "Build complete! Run with: ./TestPrograms/SuzanneMesh"
echo "Output image: SuzanneMesh.ppm"
