cmake_minimum_required(VERSION 3.22)
project(comicsdb)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(pistache REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(comicsdb)
