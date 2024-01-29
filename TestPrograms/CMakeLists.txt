add_executable(projectile projectile.cpp)
target_link_libraries(projectile Utility Geometry Canvas)

add_executable(clock clock.cpp)
target_link_libraries(clock Utility Geometry Canvas)

add_executable(ballProjection ballProjection.cpp)
target_link_libraries(ballProjection Utility Geometry Canvas Material Scene)

add_executable(ballWorld ballWorld.cpp)
target_link_libraries(ballWorld Utility Geometry Canvas Material Scene)