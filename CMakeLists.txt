cmake_minimum_required(VERSION 3.18)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_definitions("-g" "-Wall" "-Wextra" "-Werror" "-pedantic")

project(Schiffeversenken)

add_subdirectory(src)
add_subdirectory(lib)

enable_testing()
add_subdirectory(test)
