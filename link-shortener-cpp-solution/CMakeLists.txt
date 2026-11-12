cmake_minimum_required(VERSION 3.20)
project(hop_links LANGUAGES CXX)
include(cmake/devventura.cmake)

# The code you write lives in this library.
add_library(hop
  src/short_code.cpp
  src/link_store.cpp
  src/console.cpp
)
target_include_directories(hop PUBLIC include)

# The support console from milestone 3: ./build/hop
add_executable(hop_console app/main.cpp)
target_link_libraries(hop_console PRIVATE hop)
set_target_properties(hop_console PROPERTIES OUTPUT_NAME hop)

dv_milestone_test(m1 tests/m1_short_code_test.cpp hop)
dv_milestone_test(m2 tests/m2_link_store_test.cpp hop)
dv_milestone_test(m3 tests/m3_console_test.cpp hop)
