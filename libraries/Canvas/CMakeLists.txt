add_library(Canvas STATIC "")

target_sources(
  Canvas
  PRIVATE
    src/Canvas.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Canvas.hpp
)

target_include_directories(
  Canvas
  PUBLIC
    include
)

target_link_libraries(
  Canvas
  PUBLIC
    Utility
)