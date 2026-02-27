project(cpp-hse)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

include_directories(util)

# 0
add_subdirectory(tasks/multiplication)


# 1
add_subdirectory(tasks/next_even)
add_subdirectory(tasks/is_leap)
add_subdirectory(tasks/chocolate)
add_subdirectory(tasks/min_divisor)
add_subdirectory(tasks/fibonacci)
add_subdirectory(tasks/reduce_fraction)
add_subdirectory(tasks/palindrome)
add_subdirectory(tasks/password)
add_subdirectory(tasks/varint)
add_subdirectory(tasks/bitops)
add_subdirectory(tasks/safe_arithmetic)
add_subdirectory(tasks/fp16)


# 2
add_subdirectory(tasks/interpreter)
add_subdirectory(tasks/sort_students)
add_subdirectory(tasks/tests_checking)
add_subdirectory(tasks/admission)
add_subdirectory(tasks/scorer)
add_subdirectory(tasks/unixpath)


# 3
add_subdirectory(tasks/provinces)
add_subdirectory(tasks/search)
add_subdirectory(tasks/rational)
add_subdirectory(tasks/stack)
add_subdirectory(tasks/queue)
add_subdirectory(tasks/defer)
add_subdirectory(tasks/minmax)
add_subdirectory(tasks/rotate)
add_subdirectory(tasks/remove)
add_subdirectory(tasks/enumerate)
add_subdirectory(tasks/lru_cache)
add_subdirectory(tasks/http_builder)
add_subdirectory(tasks/bpe_tokenizer)
add_subdirectory(tasks/game_of_life)

# project
add_subdirectory(tasks/image_processor)
