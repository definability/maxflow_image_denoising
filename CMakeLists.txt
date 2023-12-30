cmake_minimum_required(VERSION 3.12)
project(maxflow_image_denoising)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS graph)
find_package(OpenCV 4 REQUIRED core imgcodecs)

add_library(greyscale_image greyscale_image.cpp)
add_library(max_flow_solver max_flow_solver.cpp impl/max_flow_solver_implementation.cpp)

add_executable(maxflow_image_denoising main.cpp types.cpp)

target_include_directories(greyscale_image PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(maxflow_image_denoising PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(greyscale_image PRIVATE ${OpenCV_LIBRARIES})
target_link_libraries(max_flow_solver PRIVATE Boost::graph)
target_link_libraries(maxflow_image_denoising PRIVATE greyscale_image max_flow_solver)
