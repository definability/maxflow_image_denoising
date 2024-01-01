#ifndef MAXFLOW_IMAGE_DENOISING_GREYSCALE_IMAGE_HPP
#define MAXFLOW_IMAGE_DENOISING_GREYSCALE_IMAGE_HPP

#include "types.hpp"

#include <filesystem>
#include <memory>
#include <string_view>

namespace cv
{
  class Mat;
}

class GreyscaleImage
{
public:
  explicit GreyscaleImage(const std::filesystem::path& path);

  void load(const std::filesystem::path& path);

  void save(const std::filesystem::path& path) const;

  [[nodiscard]] ImageSize height() const;

  [[nodiscard]] ImageSize width() const;

  [[nodiscard]] PixelValue operator()(ImageSize y, ImageSize x) const;

  PixelValue& operator()(ImageSize y, ImageSize x);

  ~GreyscaleImage();

private:
  std::unique_ptr<cv::Mat> image;
};

#endif //MAXFLOW_IMAGE_DENOISING_GREYSCALE_IMAGE_HPP
