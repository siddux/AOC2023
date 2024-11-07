cmake_minimum_required (VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component (CURRENT_DAY ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable (${CURRENT_DAY}part1 "part1.cpp")
add_executable (${CURRENT_DAY}part2 "part2.cpp")

target_link_libraries(${CURRENT_DAY}part1 PUBLIC aoc_tools)
target_link_libraries(${CURRENT_DAY}part2 PUBLIC aoc_tools)

set_property(TARGET ${CURRENT_DAY}part1 PROPERTY CXX_STANDARD 23)
set_property(TARGET ${CURRENT_DAY}part1 PROPERTY CMAKE_CXX_STANDARD_REQUIRED  YES)
set_property(TARGET ${CURRENT_DAY}part2 PROPERTY CXX_STANDARD 23)
set_property(TARGET ${CURRENT_DAY}part2 PROPERTY CMAKE_CXX_STANDARD_REQUIRED  YES)