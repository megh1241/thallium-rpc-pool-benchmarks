cmake_minimum_required (VERSION 3.15)
project (dummy C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -Wpedantic")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Wpedantic")

find_package(thallium REQUIRED)
find_package(MPI REQUIRED)
add_executable(server server.cpp server_provider.cpp)
target_link_libraries(server PRIVATE thallium MPI::MPI_CXX)
add_executable(client client.cpp server_provider.cpp)
target_link_libraries(client PRIVATE thallium MPI::MPI_CXX)
