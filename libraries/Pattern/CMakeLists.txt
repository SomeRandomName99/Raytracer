add_library(Pattern STATIC "")

target_sources(
  Pattern
  PRIVATE
    src/Pattern.cpp
    src/StripePattern.cpp
    src/GradientPattern.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Pattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/PatternT.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/StripePattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/GradientPattern.hpp
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