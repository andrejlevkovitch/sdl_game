// get_file_line_func.hpp

#pragma once

#include <sstream>
#include <string>

namespace levi {
inline std::string get_file_line_func() {
  std::stringstream ss;
  const size_t max_buf_size{512};
  char buf[max_buf_size]{};
  ss << __FILE__ << ':' << __LINE__ << '(' << __FUNCTION__ << ')';
  ss.getline(buf, max_buf_size);
  return buf;
};
}; // namespace levi
