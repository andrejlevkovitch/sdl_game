// load_png.cpp

#include "load_png.hpp"
#include <png.h>
#include <stdexcept>

const size_t max_sig{8};

levi::image levi::load_png_as_rgba(const std::string &file) {
  image image{};

  std::FILE *fb = std::fopen(file.c_str(), "rb");

  if (!fb) {
    throw std::runtime_error{"can't open file: " + file};
  }

  char sig_buf[max_sig]{};
  std::fread(sig_buf, 1, max_sig, fb);

  if (::png_sig_cmp(reinterpret_cast<::png_bytep>(sig_buf), 0, max_sig)) {
    std::fclose(fb);
    throw std::runtime_error{"file: " + file + " is not png"};
  }

  auto png_read = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                           nullptr, nullptr);

  if (!png_read) {
    throw std::runtime_error{"png reader can't be created"};
  }

  auto png_info = ::png_create_info_struct(png_read);

  if (!png_info) {
    std::fclose(fb);
    ::png_destroy_read_struct(&png_read, nullptr, nullptr);
    throw std::runtime_error{"file: " + file + " not valid"};
  }

  auto png_end_info = ::png_create_info_struct(png_read);

  if (!png_end_info) {
    std::fclose(fb);
    ::png_destroy_read_struct(&png_read, &png_info, nullptr);
    throw std::runtime_error{"file: " + file + " not valid"};
  }

  ::png_init_io(png_read, fb);

  ::png_set_sig_bytes(png_read, max_sig);

  ::png_read_info(png_read, png_info);

  image.width = ::png_get_image_width(png_read, png_info);
  image.height = ::png_get_image_height(png_read, png_info);
  if (::png_get_bit_depth(png_read, png_info) < 8) {
    ::png_set_packing(png_read);
  }
  if (::png_get_valid(png_read, png_info, PNG_INFO_tRNS)) {
    ::png_set_tRNS_to_alpha(png_read);
  }
  ::png_set_interlace_handling(png_read);

  ::png_read_update_info(png_read, png_info);

  size_t byte_width = ::png_get_rowbytes(png_read, png_info);

  image.data.resize(image.height * byte_width);

  std::vector<::png_bytep> row_pointers(image.height, nullptr);
  for (int i{}; i < row_pointers.size(); ++i) {
    row_pointers[i] =
        reinterpret_cast<::png_bytep>(&image.data[i * byte_width]);
  }
  ::png_read_image(png_read, &row_pointers[0]);

  ::png_read_end(png_read, png_info);

  ::png_destroy_read_struct(&png_read, &png_info, &png_end_info);
  std::fclose(fb);

  return image;
}
