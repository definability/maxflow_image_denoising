add_library(greyscale_image greyscale_image.cpp)
add_library(binary_image_denoiser binary_image_denoiser.cpp max_flow_denoiser.cpp)

add_executable(maxflow_image_denoising main.cpp types.cpp)

target_include_directories(greyscale_image PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(maxflow_image_denoising PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(greyscale_image PRIVATE ${OpenCV_LIBRARIES})
target_link_libraries(binary_image_denoiser PRIVATE Boost::graph)
target_link_libraries(maxflow_image_denoising PRIVATE greyscale_image binary_image_denoiser)
