#include "types.hpp"

#include <algorithm>
#include <limits>

/// \brief Number of neighbours of each pixel.
constexpr std::uint8_t neighbours_count = 4;

// For the convenience of further calculations,
// we assume that the number of edges per pixel fits into a byte.
static_assert(neighbours_count < std::numeric_limits<std::uint8_t>::max() - 2);

/// \brief Number of edges per pixel.
/// \details 4 edges for each neighbour,
/// 1 edge for the source and 1 edge for the sink.
constexpr std::uint8_t edges_per_pixel = neighbours_count + 2;

// Vertex index must be able to represent any pixel index in the image
// plus two additional vertices for the source and the sink.
static_assert(2 * sizeof(ImageSize) <= sizeof(VertexCount));
// Edge index must be able to represent edges from source to any pixel,
// from any pixel to its neighbours and from any pixel to the sink.
static_assert(
  sizeof(VertexCount) + sizeof(neighbours_count) <= sizeof(EdgeCount));
// Edge capacity must be able to represent the maximum possible flow,
// which cannot exceed the sum of all edge capacities.
static_assert(std::max(sizeof(DiscontinuityPenalty), sizeof(PixelValue)) +
              sizeof(ImageSize) < sizeof(EdgeCapacity));

// The formulae to calculate the maximum possible values are:
[[maybe_unused]] constexpr auto max_pixel_value = std::numeric_limits<
  PixelValue>::max();
[[maybe_unused]] constexpr auto max_image_size = std::numeric_limits<
  ImageSize>::max();
[[maybe_unused]] constexpr auto max_vertex_count =
  static_cast<VertexCount>(max_image_size) * max_image_size;
[[maybe_unused]] constexpr auto max_edge_count =
  static_cast<EdgeCount>(max_vertex_count) * edges_per_pixel;
[[maybe_unused]] constexpr auto max_discontinuity_penalty = std::numeric_limits<
  DiscontinuityPenalty>::max();
[[maybe_unused]] constexpr auto max_edge_capacity =
  static_cast<EdgeCapacity>(max_discontinuity_penalty) * max_image_size * 4 +
  static_cast<EdgeCapacity>(max_pixel_value) * max_image_size;