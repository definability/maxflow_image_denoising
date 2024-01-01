#ifndef MAXFLOW_IMAGE_DENOISING_MAX_FLOW_DENOISER_HPP
#define MAXFLOW_IMAGE_DENOISING_MAX_FLOW_DENOISER_HPP

#include "binary_image_denoiser.hpp"

#include "greyscale_image.hpp"
#include "types.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>

/// \class MaxFlowDenoiser
/// \brief Class for denoising greyscale images
/// using the Boykov-Kolmogorov Max-Flow algorithm.
///
/// \details
/// The MaxFlowDenoiser class provides functionality for denoising greyscale
/// images using the Boykov-Kolmogorov Max-Flow algorithm implementation
/// from The Graph Boost Library (BGL).
/// It constructs a graph based on the
/// given image and computes the maximum flow to determine the denoised image.
class BinaryImageDenoiser::MaxFlowDenoiser
{
public:
  /// \brief Construct a new Max-Flow solver
  /// for images of specific height and width.
  ///
  /// \param height The input image(s) height.
  /// \param width The input image(s) width.
  /// \param discontinuity_penalty A smoothness term for the denoising problem,
  /// which is a weight of edges between the neighbouring pixels.
  MaxFlowDenoiser(
    ImageSize height, ImageSize width, EdgeCapacity discontinuity_penalty);

  /// \brief Apply the denoising algorithm to the given noisy image.
  ///
  /// \description
  /// The results are stored in the MaxFlowDenoiser::graph.
  /// To fetch the denoised image, use MaxFlowDenoiser::operator>>.
  ///
  /// \param noisy_image The input noisy image.
  /// It must have the same height and width
  /// specified during the solver construction.
  void operator()(const GreyscaleImage& noisy_image);

  /// @brief Extract the denoised image.
  ///
  /// @note
  /// You must use the MaxFlowDenoiser::operator() to denoise an image first.
  ///
  /// \param output_image The image to store the result in.
  void operator>>(GreyscaleImage& output_image) const;

private:
  using EdgeDescriptor = boost::detail::csr_edge_descriptor<
    VertexCount,
    EdgeCount
  >;

  using VIndexProperty = boost::property<boost::vertex_index_t, VertexCount>;
  using VDistanceProperty = boost::property<
    boost::vertex_distance_t,
    VertexCount,
    VIndexProperty
  >;
  using VPredecessorProperty = boost::property<
    boost::vertex_predecessor_t,
    EdgeDescriptor,
    VDistanceProperty
  >;
  using VColourProperty = boost::property<
    boost::vertex_color_t,
    boost::default_color_type,
    VPredecessorProperty
  >;
  using VertexProperties = VColourProperty;

  using EReverse = boost::property<boost::edge_reverse_t, EdgeDescriptor>;
  using ECapacity = boost::property<
    boost::edge_capacity_t,
    EdgeCapacity,
    EReverse
  >;
  using EResidual = boost::property<
    boost::edge_residual_capacity_t,
    EdgeCapacity,
    ECapacity
  >;
  using EdgeProperties = EResidual;

  using Graph = boost::compressed_sparse_row_graph<
    boost::directedS,
    VertexProperties,
    EdgeProperties,
    boost::no_property,
    VertexCount,
    EdgeCount
  >;
  using VertexDescriptor = boost::graph_traits<Graph>::vertex_descriptor;

  MaxFlowDenoiser(
    ImageSize height,
    ImageSize width,
    EdgeCapacity discontinuity_penalty,
    VertexCount vertices_count);

  Graph construct_graph(VertexCount vertices_count);

  void add_edges(EdgeCapacity discontinuity_penalty);

  void replace_pixel_edges(const GreyscaleImage& image);

  const ImageSize rows;
  const ImageSize columns;

  const VertexCount source_index;
  const VertexCount sink_index;

  Graph graph;

  const VertexDescriptor source;
  const VertexDescriptor sink;
};

#endif //MAXFLOW_IMAGE_DENOISING_MAX_FLOW_DENOISER_HPP
