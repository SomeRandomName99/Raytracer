add_library(Material STATIC "")

target_sources(
  Material
  PRIVATE
    src/Material.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Material.hpp
)

target_include_directories(
  Material
  PUBLIC
    include
)

target_link_libraries(
  Material
  PRIVATE
    Utility
    Pattern
)