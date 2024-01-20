add_library(Scene STATIC "")

target_sources(
  Scene
  PRIVATE
    src/Light.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Light.hpp
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
)