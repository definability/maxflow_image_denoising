#ifndef MAXFLOW_IMAGE_DENOISING_GREYSCALE_IMAGE_HPP
#define MAXFLOW_IMAGE_DENOISING_GREYSCALE_IMAGE_HPP

#include "types.hpp"

#include <filesystem>
#include <memory>
#include <string_view>

namespace cv
{
  /// \brief Forward declaration of OpenCV's cv::Mat class.
  class Mat;
}

/// \class GreyscaleImage
/// \brief A class for representing and manipulating greyscale images.
///
/// \details
/// The GreyscaleImage class provides functionality
/// for loading, saving, accessing, and modifying greyscale images.
/// It uses OpenCV's cv::Mat class to handle image data.
class GreyscaleImage
{
public:
  /// \brief Construct a new GreyscaleImage object from an image file.
  ///
  /// \param path The path to the image file.
  ///
  /// \note The image will be automatically converted to greyscale format.
  explicit GreyscaleImage(const std::filesystem::path& path);

  /// \brief Load a greyscale image from the specified path.
  ///
  /// \param path The path of the image to be loaded.
  ///
  /// \note The image will be automatically converted to greyscale format.
  void load(const std::filesystem::path& path);

  /// \brief Save the greyscale image to the specified path.
  ///
  /// \param path The path to save the image.
  /// File extension defines the output format.
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
