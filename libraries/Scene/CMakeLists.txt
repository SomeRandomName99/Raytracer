add_library(Scene STATIC "")

target_sources(
  Scene
  PRIVATE
    src/Light.cpp
    src/World.cpp
    src/Camera.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Light.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/World.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Camera.hpp
)

target_include_directories(
  Scene
  PUBLIC
    include
)

target_link_libraries(
  Scene
  PRIVATE
    Utility
    Material
    Geometry
    Canvas
)