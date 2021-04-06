project(local_telegram)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(local_telegram main.cpp)