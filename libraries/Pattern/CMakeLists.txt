add_library(Pattern STATIC "")

target_sources(
  Pattern
  PRIVATE
    src/Pattern.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Pattern.hpp
)

target_include_directories(
  Pattern
  PUBLIC
    include
)

target_link_libraries(
  Pattern
  PRIVATE
    Geometry
    Utility
)