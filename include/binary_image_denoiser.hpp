#ifndef MAXFLOW_IMAGE_DENOISING_BINARY_IMAGE_DENOISER_HPP
#define MAXFLOW_IMAGE_DENOISING_BINARY_IMAGE_DENOISER_HPP

#include <memory>

#include "types.hpp"

class GreyscaleImage;

/// \class BinaryImageDenoiser
/// \brief The BinaryImageDenoiser class solves the maximum flow problem
/// to denoise a binary image.
///
/// \note
/// The input image can be greyscale, but the result will be a binary image.
///
/// \details
/// Example usage:
/// \code{.cpp}
/// const GreyscaleImage image = load_image("input.png");
/// const DiscontinuityPenalty penalty = 128;
///
/// BinaryImageDenoiser solver{image.height(), image.width(), penalty};
/// solver(image);
/// image.save("output.png");
/// \endcode
class BinaryImageDenoiser
{
public:
  /// \brief Construct a new denoiser
  /// for images of specific height and width.
  ///
  /// \param height Input image(s) height.
  /// \param width Input image(s) width.
  /// \param discontinuity_penalty Smoothness term for the denoising problem.
  BinaryImageDenoiser(
    ImageSize height,
    ImageSize width,
    DiscontinuityPenalty discontinuity_penalty);

  BinaryImageDenoiser(const BinaryImageDenoiser&) = delete;

  BinaryImageDenoiser(BinaryImageDenoiser&&) noexcept;

  BinaryImageDenoiser& operator=(const BinaryImageDenoiser&) = delete;

  BinaryImageDenoiser& operator=(BinaryImageDenoiser&&) noexcept;


  /// \brief Applies the Max-Flow algorithm to the provided noisy image.
  ///
  /// \param noisy_image The image to denoise.
  ///
  /// \note The noisy_image will be modified in-place with the result of the Max-Flow algorithm.
  void operator()(GreyscaleImage& noisy_image) const;

  ~BinaryImageDenoiser();

private:
  /// \brief Forward declaration of the denoising algorithm implementation.
  class MaxFlowDenoiser;

  std::unique_ptr<MaxFlowDenoiser> implementation;
};

#endif //MAXFLOW_IMAGE_DENOISING_BINARY_IMAGE_DENOISER_HPP
