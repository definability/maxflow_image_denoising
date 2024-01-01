#include "greyscale_image.hpp"
#include "max_flow_solver.hpp"
#include "types.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

int main(const int argc, const char* argv[]) try
{
  if (argc != 4)
  {
    std::cout << "Usage: " << argv[0]
              << " <input image> <output image> <discontinuity penalty>"
              << std::endl;
    return EXIT_FAILURE;
  }

  const auto input_path{std::filesystem::absolute(argv[1])};
  if (!std::filesystem::exists(input_path))
  {
    std::cerr << "Input file does not exist: " << input_path << std::endl;
    return EXIT_FAILURE;
  }

  const auto output_path{std::filesystem::absolute(argv[2])};
  if (!std::filesystem::exists(output_path.parent_path()))
  {
    std::cerr << "Output folder does not exist: " << output_path.parent_path()
              << std::endl;
    return EXIT_FAILURE;
  }

  DiscontinuityPenalty discontinuity_penalty;
  try
  {
    discontinuity_penalty = std::stoul(argv[3]);
  }
  catch (const std::invalid_argument& exception)
  {
    std::cerr
      << "Discontinuity penalty should be a valid integer in ranges from "
      << std::to_string(std::numeric_limits<DiscontinuityPenalty>::min())
      << " to "
      << std::to_string(std::numeric_limits<DiscontinuityPenalty>::max())
      << " but got: '" << argv[3] << '\'' << std::endl;
    std::cerr << "Error message: " << exception.what();
    return EXIT_FAILURE;
  }
  catch (const std::out_of_range& exception)
  {
    std::cerr
      << "Discontinuity penalty should be a valid integer in ranges from "
      << std::to_string(std::numeric_limits<DiscontinuityPenalty>::min())
      << " to "
      << std::to_string(std::numeric_limits<DiscontinuityPenalty>::max())
      << " but got: '" << argv[3] << '\'' << std::endl;
    std::cerr << "Error message: " << exception.what();
    return EXIT_FAILURE;
  }

  GreyscaleImage image{input_path.string()};
  MaxFlowSolver max_flow_solver{image.height(), image.width(),
                                discontinuity_penalty};
  max_flow_solver(image);
  image.save(output_path);

  return EXIT_SUCCESS;
}
catch (const std::exception& exception)
{
  std::cerr << "Unhandled exception: " << exception.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "Unknown exception." << std::endl;
  return EXIT_FAILURE;
}