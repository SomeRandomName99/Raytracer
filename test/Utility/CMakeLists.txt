# Main test executable (includes all tests)
target_sources(
  Tests
  PRIVATE
    ColorTests.cpp 
    FloatUtilsTests.cpp 
    MatrixTests.cpp 
    RayTests.cpp 
    TransformationsTests.cpp 
    TupleTests.cpp
    ArenaTests.cpp
    LinearAllocatorTests.cpp
)
