#ifndef MAXFLOW_IMAGE_DENOISING_MAX_FLOW_SOLVER_IMPLEMENTATION_HPP
#define MAXFLOW_IMAGE_DENOISING_MAX_FLOW_SOLVER_IMPLEMENTATION_HPP

#include "max_flow_solver.hpp"

#include "greyscale_image.hpp"
#include "types.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>

class MaxFlowSolver::MaxFlowSolverImplementation
{
public:
  MaxFlowSolverImplementation(
    ImageSize height, ImageSize width, EdgeCapacity discontinuity_penalty);

  void operator()(const GreyscaleImage& noisy_image);

  void operator>>(GreyscaleImage& noisy_image);

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

  MaxFlowSolverImplementation(
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

#endif //MAXFLOW_IMAGE_DENOISING_MAX_FLOW_SOLVER_IMPLEMENTATION_HPP
