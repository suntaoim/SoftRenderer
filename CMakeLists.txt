cmake_minimum_required(VERSION 2.8)

project(SoftRenderer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include")

aux_source_directory(./src CPP_LIST)
add_executable(SoftRenderer ${CPP_LIST})