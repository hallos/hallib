cmake_minimum_required(VERSION 3.5)

project(hallib)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

include_directories(
    external
    include
)

add_subdirectory(test)
add_subdirectory(src/network)