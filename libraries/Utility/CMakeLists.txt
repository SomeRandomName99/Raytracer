add_library(Utility STATIC "")

target_sources(
  Utility
  PRIVATE
    src/Color.cpp
    src/Ray.cpp
    src/Transformations.cpp
    src/LinearAllocator.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Color.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/floatUtils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Matrix.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Ray.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Tuple.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Transformations.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/AABB.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Arena.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/LinearAllocator.hpp
)

target_include_directories(
  Utility
  PUBLIC
    include
)
