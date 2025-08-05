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

# Create isolated allocator library for testing
add_library(AllocatorTestLib STATIC
  ../../libraries/Utility/src/LinearAllocator.cpp
  ../../libraries/Utility/include/LinearAllocator.hpp
  ../../libraries/Utility/include/Arena.hpp
)

target_include_directories(AllocatorTestLib PUBLIC
  ../../libraries/Utility/include
)

# Separate Arena test executable - only depends on allocator code
add_executable(ArenaTests ArenaTests.cpp)
target_link_libraries(ArenaTests
  GTest::gtest_main
  GTest::gmock_main
  AllocatorTestLib
)

# Separate LinearAllocator test executable - only depends on allocator code
add_executable(LinearAllocatorTests LinearAllocatorTests.cpp)
target_link_libraries(LinearAllocatorTests
  GTest::gtest_main
  GTest::gmock_main
  AllocatorTestLib
)

# Add to CTest
add_test(NAME ArenaTests COMMAND ArenaTests)
add_test(NAME LinearAllocatorTests COMMAND LinearAllocatorTests)