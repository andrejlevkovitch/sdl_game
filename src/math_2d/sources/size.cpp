// size.cpp

#include "size.hpp"

levi::size::size() : width_{}, height_{} {}

levi::size::size(int width, int height) : width_{width}, height_{height} {}

int levi::size::get_width() const { return width_; }

int levi::size::get_height() const { return height_; }

void levi::size::set_width(int width) { width_ = width; }

void levi::size::set_height(int height) { height_ = height; }
