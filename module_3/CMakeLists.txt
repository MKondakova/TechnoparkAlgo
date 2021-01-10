cmake_minimum_required(VERSION 3.15)
project(module_3)

set(CMAKE_CXX_STANDARD 14)


FILE(GLOB GRAPH_INTERFACE_SOURCES graph_interface/*.cpp)
FILE(GLOB GRAPH_INTERFACE_HEADERS graph_interface/*.h)

add_library(graph_interface ${GRAPH_INTERFACE_SOURCES} ${GRAPH_INTERFACE_HEADERS})

add_executable(module_3_task_1 test_different_graphs.cpp)
target_link_libraries(module_3_task_1 graph_interface)

add_executable(module_3_task_2 shortest_paths_count.cpp)
target_link_libraries(module_3_task_2 graph_interface)

add_executable(module_3_task_3 town_roads.cpp)

add_executable(module_3_task_4 15_puzzle.cpp)
