set(This Tests)

add_executable(${This} "")
target_link_libraries(${This} PUBLIC
  GTest::gtest_main
  GTest::gmock_main
  Canvas
  Geometry
  Utility
  Scene
  Material
)

add_subdirectory(Canvas)
add_subdirectory(Geometry)
add_subdirectory(Utility)
add_subdirectory(Scene)
add_subdirectory(Material)

gtest_discover_tests(${This})