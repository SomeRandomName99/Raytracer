#ifndef ARENA_HPP
#define ARENA_HPP
#include "LinearAllocator.hpp"
#include <optional>

namespace utility {

template <typename T>
struct Arena {
  LinearAllocator allocator;
  T* data;
  size_t size; // Number of elements in the arena
  size_t capacity; 

  Arena(size_t capacity = GB(128)) : allocator(capacity), data{nullptr}, size{0}, capacity{0} {
    data = static_cast<T*>(allocator.begin);
  }

  bool allocate(size_t count){
    if(count > capacity) return false;
    return allocator.allocate(count * sizeof(T)) == 0;
  }

  // TODO: Make sure that there is enough committed memory in the linear allocator
  void pushBack(const T& item){
    if(size*sizeof(T) > allocator.committed){
    }
    if(size >= capacity) return;
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

#endif // ARENA_HPP