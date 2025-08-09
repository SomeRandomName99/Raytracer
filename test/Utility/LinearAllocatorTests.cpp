#include "LinearAllocator.hpp"
#include <gtest/gtest.h>
#include <iterator>
#include <vector>
#include <cstring>
#include <iostream>

// Tests originally written by Cursor
namespace raytracer {
namespace utility {
namespace test {

class LinearAllocatorTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(LinearAllocatorTest, Construction) {
  LinearAllocator allocator(MB(1));
  
  EXPECT_EQ(allocator.capacity, MB(1));
  EXPECT_EQ(allocator.committed, 0);
  EXPECT_NE(allocator.begin, nullptr);
  
  char* begin = static_cast<char*>(allocator.begin);
  EXPECT_EQ(begin, allocator.begin);
}

TEST_F(LinearAllocatorTest, BasicAllocation) {
  LinearAllocator allocator(MB(1));
  
  // Allocate some memory
  int result = allocator.allocate(1024);
  EXPECT_EQ(result, 0);
  EXPECT_GT(allocator.committed, 0);
  
  // Allocate more memory
  result = allocator.allocate(2048);
  EXPECT_EQ(result, 0);
  EXPECT_GT(allocator.committed, 1024);
}

TEST_F(LinearAllocatorTest, AllocationBeyondCapacity) {
  LinearAllocator allocator(KB(4)); // 4KB capacity
  
  int result = allocator.allocate(MB(1));
  EXPECT_EQ(result, -1);
  EXPECT_EQ(allocator.committed, 0);
}

TEST_F(LinearAllocatorTest, Reset) {
  LinearAllocator allocator(MB(1));
  
  int result = allocator.allocate(1024);
  EXPECT_EQ(result, 0);
  EXPECT_GT(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 1024);
  
  // Reset should decommit memory and reset counters
  allocator.reset();
  EXPECT_EQ(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 0);
  
  // Should be able to allocate again (memory will be recommitted)
  result = allocator.allocate(512);
  EXPECT_EQ(result, 0);
  EXPECT_GT(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 512);
}

TEST_F(LinearAllocatorTest, MemoryAccess) {
  LinearAllocator allocator(MB(1));
  
  int result = allocator.allocate(1024);
  EXPECT_EQ(result, 0);
  
  char* memory = static_cast<char*>(allocator.begin);
  std::strcpy(memory, "Hello, World!");
  
  EXPECT_STREQ(memory, "Hello, World!");
}

TEST_F(LinearAllocatorTest, MultipleAllocations) {
  LinearAllocator allocator(MB(1));
  
  std::vector<int> results;
  std::vector<size_t> sizes = {64, 128, 256, 512, 1024};
  
  for (size_t size : sizes) {
    int result = allocator.allocate(size);
    results.push_back(result);
  }
  
  // All allocations should succeed
  for (int result : results) {
    EXPECT_EQ(result, 0);
  }
  
  // Committed should be at least as much as allocated
  size_t totalRequested = 0;
  for (size_t size : sizes) {
    totalRequested += size;
  }
  EXPECT_GE(allocator.committed, totalRequested);
}

TEST_F(LinearAllocatorTest, ZeroAllocation) {
  LinearAllocator allocator(MB(1));
  
  int result = allocator.allocate(0);
  EXPECT_EQ(result, 0);
  EXPECT_EQ(allocator.committed, 0);
}

TEST_F(LinearAllocatorTest, LargeAllocation) {
  LinearAllocator allocator(MB(10));
  
  // Allocate a large chunk
  int result = allocator.allocate(MB(5));
  EXPECT_EQ(result, 0);
  EXPECT_GE(allocator.committed, MB(5));
}

TEST_F(LinearAllocatorTest, MemoryReuseAfterReset) {
  LinearAllocator allocator(MB(1));
  
  // First allocation
  EXPECT_EQ(allocator.allocate(1024), 0);
  void* firstPtr = allocator.begin;
  EXPECT_GT(allocator.committed, 0);
  
  // Reset should decommit memory but keep the same address space
  allocator.reset();
  EXPECT_EQ(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 0);
  
  // Second allocation should reuse the same address space
  EXPECT_EQ(allocator.allocate(1024), 0);
  void* secondPtr = allocator.begin;
  
  // Should get the same memory location (same reserved address space)
  EXPECT_EQ(firstPtr, secondPtr);
  EXPECT_EQ(allocator.used, 1024);
  EXPECT_GT(allocator.committed, 0);
}

TEST_F(LinearAllocatorTest, MemoryDecommit) {
  LinearAllocator allocator(MB(1));
  
  // Allocate multiple chunks to commit several pages
  int result1 = allocator.allocate(1024);
  int result2 = allocator.allocate(2048);
  int result3 = allocator.allocate(4096);
  
  EXPECT_EQ(result1, 0);
  EXPECT_EQ(result2, 0);
  EXPECT_EQ(result3, 0);
  
  size_t committedBefore = allocator.committed;
  size_t usedBefore = allocator.used;
  
  EXPECT_GT(committedBefore, 0);
  EXPECT_EQ(usedBefore, 1024 + 2048 + 4096);
  
  // Reset should decommit all committed memory
  allocator.reset();
  
  EXPECT_EQ(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 0);
  
  // Allocate again - should recommit memory
  int result4 = allocator.allocate(1024);
  EXPECT_EQ(result4, 0);
  EXPECT_GT(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 1024);
}

TEST_F(LinearAllocatorTest, MultipleResetCycles) {
  LinearAllocator allocator(MB(1));
  
  // First cycle
  int result1 = allocator.allocate(1024);
  EXPECT_EQ(result1, 0);
  EXPECT_GT(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 1024);
  
  allocator.reset();
  EXPECT_EQ(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 0);
  
  // Second cycle
  int result2 = allocator.allocate(2048);
  EXPECT_EQ(result2, 0);
  EXPECT_GT(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 2048);
  
  allocator.reset();
  EXPECT_EQ(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 0);
  
  // Third cycle
  int result3 = allocator.allocate(512);
  EXPECT_EQ(result3, 0);
  EXPECT_GT(allocator.committed, 0);
  EXPECT_EQ(allocator.used, 512);
  
  // Verify that the address space remains the same
  void* ptr1 = allocator.begin;
  allocator.reset();
  allocator.allocate(1024);
  void* ptr2 = allocator.begin;
  EXPECT_EQ(ptr1, ptr2);
}

TEST_F(LinearAllocatorTest, CapacityLimits) {
  LinearAllocator allocator(KB(1)); // 1KB capacity
  
  // Try to allocate exactly the capacity
  int result = allocator.allocate(KB(1));
  EXPECT_EQ(result, 0);
  auto prevCapacity = allocator.capacity;
  auto prevCommitted = allocator.committed;
  
  // Try to allocate one more byte
  result = allocator.allocate(1);
  EXPECT_EQ(result, -1);
  EXPECT_EQ(prevCapacity, allocator.capacity);
  EXPECT_EQ(prevCommitted, allocator.committed);
}

TEST_F(LinearAllocatorTest, DifferentCapacities) {
  std::vector<size_t> capacities = {KB(1), KB(4), KB(16), MB(1), MB(10)};
  
  for (size_t capacity : capacities) {
    LinearAllocator allocator(capacity);
    
    EXPECT_EQ(allocator.capacity, capacity);
    EXPECT_NE(allocator.begin, nullptr);
    
    // Should be able to allocate
    int result = allocator.allocate(64);
    EXPECT_EQ(result, 0);
  }
}

TEST_F(LinearAllocatorTest, MemoryProtection) {
  LinearAllocator allocator(MB(1));
  
  int result = allocator.allocate(1024);
  EXPECT_EQ(result, 0);
  
  // Should be able to read and write
  char* memory = static_cast<char*>(allocator.begin);
  memory[0] = 'A';
  memory[1] = 'B';
  memory[2] = 'C';
  memory[3] = '\0';
  
  EXPECT_EQ(memory[0], 'A');
  EXPECT_EQ(memory[1], 'B');
  EXPECT_EQ(memory[2], 'C');
  EXPECT_EQ(memory[3], '\0');
}

TEST_F(LinearAllocatorTest, StressTest) {
  LinearAllocator allocator(MB(1));
  
  // Allocate many small chunks
  size_t totalAllocated = 0;
  for (int i = 0; i < 1000; ++i) {
    int result = allocator.allocate(64);
    totalAllocated += 64;
    if(totalAllocated > allocator.capacity){
      EXPECT_EQ(result, -1);
    } else {
      EXPECT_EQ(result, 0);
    }
  }
  
  EXPECT_GT(allocator.committed, 0);
  EXPECT_LE(allocator.committed, allocator.capacity);
}

// Test LinearAllocator with mixed allocation sizes
TEST_F(LinearAllocatorTest, MixedAllocationSizes) {
  LinearAllocator allocator(MB(1));
  
  std::vector<size_t> sizes = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
  
  for (size_t size : sizes) {
    int result = allocator.allocate(size);
    EXPECT_EQ(result, 0);
  }
  
  EXPECT_GT(allocator.committed, 0);
}

} // namespace test
} // namespace utility 
} // namespace raytracer