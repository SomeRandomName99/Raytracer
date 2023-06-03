set(This Tests)

add_executable(${This} "")
target_link_libraries(${This} PUBLIC
  GTest::gtest_main
  Canvas
  Geometry
  Utility
)

add_subdirectory(Canvas)
add_subdirectory(Geometry)
add_subdirectory(Utility)

gtest_discover_tests(${This})