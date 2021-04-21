project(homework_11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(homework_11_arkanoid arkanoid.cpp)
target_link_libraries(homework_11_arkanoid sfml-graphics)

add_executable(homework_11_good asteroids.cpp Asteroid.h Bullet.h constants.h Entity.h Player.h)
target_link_libraries(homework_11_good sfml-graphics)