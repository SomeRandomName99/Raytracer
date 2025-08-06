#include "LinearAllocator.hpp"
#include <cstddef>

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
  #define PLATFORM_WINDOWS
  #include <windows.h>
#elif defined(__APPLE__) || defined(__MACH__)
  #define PLATFORM_MACOS
  #include <unistd.h>
  #include <sys/mman.h>
#elif defined(__linux__) || defined(__unix__) || defined(__posix__)
  #define PLATFORM_POSIX
  #include <unistd.h>
  #include <sys/mman.h>
#else
  #error "Unsupported platform"
#endif

namespace utility {

// Platform-specific implementation functions
namespace platform {

#ifdef PLATFORM_WINDOWS
void* reserveMemory(size_t size) {
  return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

int commitMemory(void* ptr, size_t size) {
  if(VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) == nullptr) {
    return -1;
  }
  return 0;
}

void decommitMemory(void* ptr, size_t size) {
  VirtualFree(ptr, 0, MEM_DECOMMIT);
}

void freeMemory(void* ptr, size_t size) {
  VirtualFree(ptr, 0, MEM_RELEASE);
}

size_t pageSize() {
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return si.dwPageSize;
}

#elif defined(PLATFORM_MACOS) || defined(PLATFORM_POSIX)
void* reserveMemory(size_t size) {
  void* ptr = mmap(nullptr, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  return (ptr == MAP_FAILED) ? nullptr : ptr;
}

void decommitMemory(void* ptr, size_t size) {
  madvise(ptr, size, MADV_DONTNEED);
}

int commitMemory(void* ptr, size_t size) {
  return mprotect(ptr, size, PROT_READ | PROT_WRITE);
}

void freeMemory(void* ptr, size_t size) {
  munmap(ptr, size);
}

size_t pageSize() {
  return sysconf(_SC_PAGESIZE);
}

#endif

} // namespace platform

size_t page_size = platform::pageSize();

LinearAllocator::LinearAllocator(size_t capacity) : capacity{capacity}, committed{0}, used{0}, begin{nullptr} {
   size_t pageAlignedSize = roundup(capacity, page_size);
  begin = platform::reserveMemory(pageAlignedSize);
}

LinearAllocator::~LinearAllocator() {
  if (begin != nullptr) {
    size_t pageAlignedSize = roundup(capacity, page_size);
    platform::freeMemory(begin, pageAlignedSize);
  }
}

int LinearAllocator::allocate(size_t size) {
  if (this->begin == nullptr || this->used + size > this->capacity) {
    return -1;
  }

  if(this->used + size > this->committed){
    size_t additionalNeeded = (this->used + size) - this->committed;
    size_t pageAlignedSize = roundup(additionalNeeded, page_size);
    void* currentEndOfCommited = static_cast<char*>(this->begin) + this->committed;
    if(platform::commitMemory(currentEndOfCommited, pageAlignedSize) != 0) {
      return -1;
    }
    this->committed = this->committed + pageAlignedSize;
  }

  this->used += size;
  return 0;
}

void LinearAllocator::reset() {
  if (this->begin != nullptr) {
    platform::decommitMemory(this->begin, this->committed);
    this->used = 0;
    this->committed = 0;
  }
}

} // namespace utility 