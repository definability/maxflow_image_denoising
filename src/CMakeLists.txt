add_library(greyscale_image greyscale_image.cpp)
add_library(max_flow_solver max_flow_solver.cpp max_flow_solver_implementation.cpp)

add_executable(maxflow_image_denoising main.cpp types.cpp)

target_include_directories(greyscale_image PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(maxflow_image_denoising PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(greyscale_image PRIVATE ${OpenCV_LIBRARIES})
target_link_libraries(max_flow_solver PRIVATE Boost::graph)
target_link_libraries(maxflow_image_denoising PRIVATE greyscale_image max_flow_solver)
