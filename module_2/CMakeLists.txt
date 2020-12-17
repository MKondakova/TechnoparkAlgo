cmake_minimum_required(VERSION 3.15)
project(module_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(module_2_task_1 string_set_with_hash_table_1.cpp)
add_executable(module_2_task_2 tree_with_pre_order_traversal.cpp)
add_executable(module_2_task_4 avl_tree.cpp)
add_executable(test test.cpp)