cmake_minimum_required(VERSION 3.20)
project(frostline LANGUAGES CXX)
include(cmake/devventura.cmake)

# The code you write lives in this library.
add_library(frost src/readings.cpp)
target_include_directories(frost PUBLIC include)

dv_milestone_test(m1 tests/m1_parse_test.cpp frost)
dv_milestone_test(m2 tests/m2_summary_test.cpp frost)
