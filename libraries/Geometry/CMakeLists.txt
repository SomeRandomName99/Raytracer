add_library(Geometry STATIC "")

target_sources(
  Geometry
  PRIVATE
    src/Sphere.cpp
    src/Intersections.cpp
    src/Plane.cpp
    src/Cube.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Sphere.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Intersections.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Shape.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Plane.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Cube.hpp
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
    Material
)