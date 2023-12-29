#ifndef MAXFLOW_IMAGE_DENOISING_TYPES_HPP
#define MAXFLOW_IMAGE_DENOISING_TYPES_HPP

#include <cstdint>

/// \brief 8-bit greyscale pixel value.
using PixelValue = std::uint8_t;
/// \brief Image size along one dimension.
using ImageSize = std::uint16_t;
/// \brief Vertex index in the graph.
/// \details Must be able to represent any pixel index in the image
/// plus two additional vertices for the source and the sink.
using VertexCount = std::uint32_t;
/// \brief Edge index in the graph.
/// \details Must be able to represent edges from source to any pixel,
/// from any pixel to its neighbours and from any pixel to the sink.
using EdgeCount = std::uint64_t;
/// \brief Discontinuity penalty.
using DiscontinuityPenalty = std::uint32_t;
/// \brief Edge capacity.
/// \details Must be able to represent the maximum possible flow,
/// which cannot exceed the sum of all edge capacities.
using EdgeCapacity = std::uint64_t;

#endif //MAXFLOW_IMAGE_DENOISING_TYPES_HPP
