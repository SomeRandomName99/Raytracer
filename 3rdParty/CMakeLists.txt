add_library(3rdParty INTERFACE)
target_sources(3rdParty INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/perlin.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_perlin.h
)