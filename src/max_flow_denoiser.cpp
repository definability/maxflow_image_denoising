#include "max_flow_denoiser.hpp"

#include "max_flow_exceptions.hpp"

#include <boost/graph/named_function_params.hpp>

#include <limits>
#include <string>

using namespace std::string_literals;

BinaryImageDenoiser::MaxFlowDenoiser::MaxFlowDenoiser(
  const ImageSize height,
  const ImageSize width,
  const EdgeCapacity discontinuity_penalty)
  : MaxFlowDenoiser{
    height,
    width,
    discontinuity_penalty,
    static_cast<VertexCount>(height) * width + 2,
  }
{
}

void BinaryImageDenoiser::MaxFlowDenoiser::operator()(
  const GreyscaleImage& image)
{
  this->replace_pixel_edges(image);

  auto&& edge_capacity_map = boost::get(boost::edge_capacity, this->graph);
  auto&& edge_residual_capacity_map = boost::get(
    boost::edge_residual_capacity, this->graph
  );
  auto&& edge_reverse_map = boost::get(boost::edge_reverse, this->graph);
  auto&& vertex_index_map = boost::get(boost::vertex_index, this->graph);
  auto&& vertex_distance_map = boost::get(boost::vertex_distance, this->graph);
  auto&& vertex_predecessor_map = boost::get(
    boost::vertex_predecessor,
    this->graph
  );
  auto&& vertex_colour_map = boost::get(boost::vertex_color, this->graph);

  boost::boykov_kolmogorov_max_flow(
    this->graph,
    edge_capacity_map,
    edge_residual_capacity_map,
    edge_reverse_map,
    vertex_predecessor_map,
    vertex_colour_map,
    vertex_distance_map,
    vertex_index_map,
    this->source,
    this->sink
  );
}

void
BinaryImageDenoiser::MaxFlowDenoiser::operator>>(GreyscaleImage& output_image) const
{
  auto&& colours = boost::get(boost::vertex_color, this->graph);

  auto&& source_colour = colours[this->source_index];
  auto&& sink_colour = colours[this->sink_index];
  if (source_colour != boost::black_color)
  {
    throw ResultConsistencyException{
      "Source is not black but "s + std::to_string(source_colour)
    };
  }
  if (source_colour == sink_colour)
  {
    throw ResultConsistencyException{
      "Source and sink have the same colour "s + std::to_string(source_colour)
    };
  }

  for (ImageSize y = 0; y < this->rows; ++y)
  {
    for (ImageSize x = 0; x < this->columns; ++x)
    {
      output_image(y, x) = colours[y * this->columns + x] == boost::black_color
                    ? std::numeric_limits<PixelValue>::max()
                    : 0x00;
    }
  }
}

BinaryImageDenoiser::MaxFlowDenoiser::MaxFlowDenoiser(
  const ImageSize height,
  const ImageSize width,
  const EdgeCapacity discontinuity_penalty,
  const VertexCount vertices_count)
  : rows{height}
  , columns{width}
  , source_index{vertices_count - 2}
  , sink_index{vertices_count - 1}
  , graph{construct_graph(vertices_count)}
  , source{boost::vertex(this->source_index, this->graph)}
  , sink{boost::vertex(this->sink_index, this->graph)}
{
  this->add_edges(discontinuity_penalty);
}

BinaryImageDenoiser::MaxFlowDenoiser::Graph
BinaryImageDenoiser::MaxFlowDenoiser::construct_graph(const VertexCount vertices_count)
{
  if (this->source_index < static_cast<VertexCount>(this->rows) * this->columns)
  {
    throw EdgeInitialisationException{
      "Source index "s + std::to_string(this->source_index) +
      " is less than number of pixels "s +
      std::to_string(this->rows * this->columns) +
      " but it must follow the last pixel"
    };
  }

  if (this->sink_index <= this->source_index)
  {
    throw EdgeInitialisationException{
      "Sink index "s + std::to_string(this->sink_index) +
      " is less than or equal to the source index "s +
      std::to_string(this->source_index) +
      " but it must follow the source"
    };
  }
  if (this->sink_index + 1 != vertices_count)
  {
    throw EdgeInitialisationException{
      "Sink index "s + std::to_string(this->sink_index) +
      " must be the last vertex index "s +
      std::to_string(vertices_count - 1)
    };
  }

  std::vector<std::pair<VertexCount, VertexCount>> edges;
  const EdgeCount edge_count =
    4 * (this->rows - 1) * (this->columns - 1) + 2 * (this->rows - 1) +
    2 * (this->columns - 1) + 4 * this->rows * this->columns;
  edges.reserve(edge_count);

  for (ImageSize y = 0; y < this->rows; ++y)
  {
    for (ImageSize x = 0; x < this->columns; ++x)
    {
      const auto&& current_vertex = y * this->columns + x;
      if (x + 1 < this->columns)
      {
        edges.emplace_back(current_vertex, current_vertex + 1);
      }
      if (x > 0)
      {
        edges.emplace_back(current_vertex, current_vertex - 1);
      }
      if (y + 1 < this->rows)
      {
        edges.emplace_back(current_vertex, current_vertex + this->columns);
      }
      if (y > 0)
      {
        edges.emplace_back(current_vertex, current_vertex - this->columns);
      }

      edges.emplace_back(current_vertex, this->source_index);
      edges.emplace_back(current_vertex, this->sink_index);
    }
  }

  for (ImageSize y = 0; y < this->rows; ++y)
  {
    for (ImageSize x = 0; x < this->columns; ++x)
    {
      edges.emplace_back(this->source_index, y * this->columns + x);
    }
  }
  for (ImageSize y = 0; y < this->rows; ++y)
  {
    for (ImageSize x = 0; x < this->columns; ++x)
    {
      edges.emplace_back(this->sink_index, y * this->columns + x);
    }
  }

  return {
    boost::edges_are_sorted, edges.cbegin(), edges.cend(), vertices_count,
  };
}

void BinaryImageDenoiser::MaxFlowDenoiser::add_edges(
  const EdgeCapacity discontinuity_penalty)
{
  const auto& edges_capacity_map = boost::get(
    boost::edge_capacity, this->graph
  );
  const auto& edge_reverse_map = boost::get(boost::edge_reverse, this->graph);
  for (auto [ei, ei_end] = boost::edges(this->graph); ei != ei_end; ++ei)
  {
    const auto& edge_source = boost::source(*ei, this->graph);
    const auto& edge_target = boost::target(*ei, this->graph);
    if (edge_source == this->sink or edge_source == this->source or
        edge_target == this->sink or edge_target == this->source)
    {
      edges_capacity_map[*ei] = 0;
    }
    else
    {
      edges_capacity_map[*ei] = discontinuity_penalty;
    }

    // Set reverse edge
    if (edge_source > boost::target(*ei, this->graph))
    {
      const auto& [reverse_edge, reverse_edge_exists] = boost::edge(
        edge_target, edge_source, this->graph
      );
      if (!reverse_edge_exists)
      {
        throw EdgeInitialisationException{
          "Edge from vertex "s +
          std::to_string(boost::target(*ei, this->graph)) + " to vertex "s +
          std::to_string(edge_source) + " does not exist in reverse edges list"s
        };
      }
      edge_reverse_map[*ei] = reverse_edge;
      edge_reverse_map[reverse_edge] = *ei;
    }
  }
}

void BinaryImageDenoiser::MaxFlowDenoiser::replace_pixel_edges(
  const GreyscaleImage& image)
{
  if (this->rows != image.height())
  {
    throw EdgeInitialisationException{
      "Wrong input image height. Expected "s + std::to_string(this->rows) +
      ", actual "s + std::to_string(image.height())
    };
  }
  if (this->columns != image.width())
  {
    throw EdgeInitialisationException{
      "Wrong input image width. Expected "s + std::to_string(this->columns) +
      ", actual "s + std::to_string(image.width())
    };
  }
  for (ImageSize y = 0; y < this->rows; ++y)
  {
    for (ImageSize x = 0; x < this->columns; ++x)
    {
      {
        const auto& [reverse_edge_descriptor, reverse_edge_exists] = boost::edge(
          boost::vertex(y * this->columns + x, this->graph),
          this->source,
          this->graph
        );
        if (!reverse_edge_exists)
        {
          throw EdgeInitialisationException{
            "Edge from pixel ("s + std::to_string(y) + ", "s +
            std::to_string(x) + ") to source does not exist"s
          };
        }
        const auto& edge_descriptor = boost::get(
          boost::edge_reverse, this->graph
        )[reverse_edge_descriptor];
        boost::get(boost::edge_capacity, graph)[edge_descriptor] = image(y, x);
      }
      {
        const auto& [edge_descriptor, edge_exists] = boost::edge(
          boost::vertex(y * this->columns + x, this->graph),
          this->sink,
          this->graph
        );
        if (!edge_exists)
        {
          throw EdgeInitialisationException{
            "Edge from pixel ("s + std::to_string(y) + ", "s +
            std::to_string(x) + ") to sink does not exist"s
          };
        }
        boost::get(boost::edge_capacity, graph)[edge_descriptor] =
          std::numeric_limits<PixelValue>::max() - image(y, x);
      }
    }
  }
}
