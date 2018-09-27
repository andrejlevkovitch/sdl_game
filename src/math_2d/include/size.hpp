// size.hpp

#pragma once

namespace levi {
class size {
public:
  size();
  size(int width, int height);
  int get_width() const;
  int get_height() const;
  void set_width(int);
  void set_height(int);

private:
  int width_;
  int height_;
};
}; // namespace levi
