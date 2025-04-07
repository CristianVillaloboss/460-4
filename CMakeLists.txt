cmake_minimum_required(VERSION 3.27)
project(programmingAssignment3)

set(CMAKE_CXX_STANDARD 17)

add_executable(programmingAssignment3 main.cpp
        Graph.h
        TreeNode.h
        SymbolTable.cpp
        SymbolTable.h)
