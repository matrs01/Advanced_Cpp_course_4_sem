project(task_3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(Boost 1.75.0)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(sem_11_02_21_2 main.cpp)
endif()

add_executable(task_3 main.cpp)