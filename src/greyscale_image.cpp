#include "greyscale_image.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

GreyscaleImage::GreyscaleImage(const std::filesystem::path& path)
  : image{
  std::make_unique<cv::Mat>(cv::imread(path.string(), cv::IMREAD_GRAYSCALE))
}
{
}

void GreyscaleImage::load(const std::filesystem::path& path)
{
  *this->image = cv::imread(path.string(), cv::IMREAD_GRAYSCALE);
}

void GreyscaleImage::save(const std::filesystem::path& path) const
{
  cv::imwrite(path.string(), *this->image);
}

ImageSize GreyscaleImage::height() const
{
  return this->image->rows;
}

ImageSize GreyscaleImage::width() const
{
  return this->image->cols;
}

PixelValue GreyscaleImage::operator()(ImageSize y, ImageSize x) const
{
  return this->image->at<std::uint8_t>(
    static_cast<int>(y), static_cast<int>(x));
}

PixelValue& GreyscaleImage::operator()(ImageSize y, ImageSize x)
{
  return this->image->at<std::uint8_t>(
    static_cast<int>(y), static_cast<int>(x));
}

GreyscaleImage::~GreyscaleImage() = default;
