// vertex.hpp

#pragma once

struct vertex {
  vertex(float x_, float y_, float r_, float g_, float b_)
      : x{x_}, y{y_}, r{r_}, g{g_}, b{b_} {};
  float x;
  float y;
  float r;
  float g;
  float b;
};
