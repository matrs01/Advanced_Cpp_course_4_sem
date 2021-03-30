project(Thread_functions_comparison)

find_package(Boost 1.75.0)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Thread_functions_comparison main.cpp)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)