cmake_minimum_required(VERSION 4.1)
project(MolluExp)

set(CMAKE_CXX_STANDARD 23)

add_executable(MolluExp main.cpp MediaCatalog.cpp)

target_include_directories(MolluExp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/_3rd)