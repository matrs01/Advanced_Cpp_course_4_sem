project(homework_12)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(homework_12_Julia_set main.cpp calculator.cpp visualizer.cpp)
target_link_libraries(homework_12_Julia_set sfml-graphics)

