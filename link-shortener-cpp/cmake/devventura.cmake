# Shared build setup for Devventura C++ projects. You don't need to change this file.
#
# Include it right after project() in CMakeLists.txt:
#   include(cmake/devventura.cmake)
# Then register one test executable per milestone:
#   dv_milestone_test(m1 tests/m1_something_test.cpp your_library)
# and run a milestone's tests with:  ctest --test-dir build -L m1 --output-on-failure

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W4 /permissive- /EHsc)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

enable_testing()

set(DV_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

# doctest's main(), compiled once and shared by every test executable.
add_library(dv_doctest_main OBJECT "${DV_ROOT}/tests/test_main.cpp")
target_include_directories(dv_doctest_main PUBLIC "${DV_ROOT}/third_party/doctest")

# dv_milestone_test(<label> <test source> <library>)
function(dv_milestone_test label source library)
  set(target "tests_${label}")
  add_executable(${target} ${source} $<TARGET_OBJECTS:dv_doctest_main>)
  target_include_directories(${target} PRIVATE "${DV_ROOT}/third_party/doctest")
  target_link_libraries(${target} PRIVATE ${library} Threads::Threads)
  add_test(NAME ${target} COMMAND ${target})
  set_tests_properties(${target} PROPERTIES LABELS ${label} TIMEOUT 60)
endfunction()
