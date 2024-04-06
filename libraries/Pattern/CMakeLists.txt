add_library(Pattern STATIC "")

target_sources(
  Pattern
  PRIVATE
    src/StripePattern.cpp
    src/GradientPattern.cpp
    src/RingPattern.cpp
    src/CheckerPattern.cpp
    src/PerturbedPattern.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/Pattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/StripePattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/GradientPattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/RingPattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/CheckerPattern.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/PerturbedPattern.hpp
)

target_include_directories(
  Pattern
  PUBLIC
    include
    ${CMAKE_CURRENT_LIST_DIR}/../../3rdParty
)

target_link_libraries(
  Pattern
  PRIVATE
    Geometry
    Utility
)