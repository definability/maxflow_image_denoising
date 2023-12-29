cmake_minimum_required(VERSION 3.12)
project(maxflow_image_denoising)

set(CMAKE_CXX_STANDARD 20)

add_executable(maxflow_image_denoising main.cpp types.cpp)
