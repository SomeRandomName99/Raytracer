#ifndef ARENA_HPP
#define ARENA_HPP
#include "LinearAllocator.hpp"
#include <optional>

namespace raytracer {
namespace utility {

template <typename T>
struct Arena {
  LinearAllocator allocator;
  T* data;
  size_t size; // Number of elements in the arena
  size_t capacity; 

  Arena(size_t maxSize = GB(128), size_t initialCapacity = 100) : allocator(maxSize), data{nullptr}, size{0} {
    data = static_cast<T*>(allocator.begin);
    while(initialCapacity * sizeof(T) > maxSize) {
      initialCapacity /= 2;
    }
    if(allocate(initialCapacity)) {
      capacity = initialCapacity;
    } else {
      capacity = 0;
    }
  }

  bool allocate(size_t count){
    if(allocator.allocate(count * sizeof(T)) == 0){
      capacity += count;
      return true;
    }
    return false;
  }

  void pushBack(const T& item){
    if(capacity == 0) return; // Memory was not allocated during initialization
    if (size >= capacity) {
      if(!allocate(capacity)) {
        // If there isn't enough memory, the new data will be lost
        return;
      }
    }
    data[size++] = item;
  }

  void popBack(){
    if(size == 0) return;
    size--;
  }

  void clear(){
    size = 0;
  }

  void setPosBack(size_t index){
    if(index >= size) return;
    size = index;
  }

  T* operator[](size_t index){
    if(index >= size) return nullptr;
    return &data[index];
  }
  const T* operator[](size_t index) const{
    if(index >= size) return nullptr;
    return &data[index];
  }

  // Iterator support
  T* begin(){ return data; }
  const T* begin() const{ return data; }
  T* end(){ return data + size; }
  const T* end() const{ return data + size; }

  ~Arena() {
    allocator.reset();
  }
};

} // namespace utility
} // namespace raytracer

#endif // ARENA_HPP