cmake_minimum_required(VERSION 3.12)
project(maxflow_image_denoising)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS graph)
find_package(OpenCV 4 REQUIRED core imgcodecs)

include_directories(include)
add_subdirectory(src)
