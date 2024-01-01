#include "binary_image_denoiser.hpp"

#include "max_flow_denoiser.hpp"

#include <memory>

BinaryImageDenoiser::BinaryImageDenoiser(
  const ImageSize height,
  const ImageSize width,
  const DiscontinuityPenalty discontinuity_penalty)
  : implementation{
    std::make_unique<MaxFlowDenoiser>(
      height, width, discontinuity_penalty
    )
  }
{
}

void BinaryImageDenoiser::operator()(GreyscaleImage& noisy_image) const
{
  (*implementation)(noisy_image);
  (*implementation) >> noisy_image;
}

BinaryImageDenoiser::BinaryImageDenoiser(BinaryImageDenoiser&&) noexcept = default;

BinaryImageDenoiser& BinaryImageDenoiser::operator=(BinaryImageDenoiser&&) noexcept = default;

BinaryImageDenoiser::~BinaryImageDenoiser() = default;
