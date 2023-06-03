add_library(Geometry STATIC "")

target_sources(
  Geometry
  PRIVATE
    src/Sphere.cpp
    src/Intersections.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Sphere.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Intersections.hpp
)

target_include_directories(
  Geometry
  PUBLIC
    include
)

target_link_libraries(
  Geometry
  PRIVATE
    Utility
)