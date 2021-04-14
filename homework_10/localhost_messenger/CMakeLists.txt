project(localhost_messenger)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(localhost_telegram_server messenger_server.cpp)
add_executable(localhost_telegram_client messenger_client.cpp)