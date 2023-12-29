cmake_minimum_required(VERSION 3.12)
project(maxflow_image_denoising)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV 4 REQUIRED core imgcodecs)

add_library(greyscale_image greyscale_image.cpp)

add_executable(maxflow_image_denoising main.cpp types.cpp)

target_include_directories(greyscale_image PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(greyscale_image PRIVATE ${OpenCV_LIBRARIES})
target_link_libraries(maxflow_image_denoising PRIVATE greyscale_image)
