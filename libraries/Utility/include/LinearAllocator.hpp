#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP

#include <cstddef>

namespace utility {

constexpr size_t KB(size_t size) {
  return size * 1024ULL;
}

constexpr size_t MB(size_t size) {
  return size * 1024ULL * 1024ULL;
}

constexpr size_t GB(size_t size) {
  return size * 1024ULL * 1024ULL * 1024ULL;
}

constexpr size_t TB(size_t size) {
  return size * 1024ULL * 1024ULL * 1024ULL * 1024ULL;
}

constexpr auto roundup(size_t size, size_t alignment) {
  return (size + alignment - 1) & ~(alignment - 1);
}

// A C++ linear memory allocator using virtual memory
struct LinearAllocator {
  /* Capacity is in GB and represents the reserved address space
   * Always choose the capacity to be a multiple of the page size
   * The capacity cannot be changed so make sure that is big enough
   */

  LinearAllocator(size_t capacity = GB(128));
  ~LinearAllocator();

  int allocate(size_t size);
  void reset(); // Keep reserved space

  // Delete copy constructor and assignment
  LinearAllocator(const LinearAllocator&) = delete;
  LinearAllocator& operator=(const LinearAllocator&) = delete;

  size_t capacity;
  size_t committed;
  size_t used;
  void* begin;
};

} // namespace utility

#endif 