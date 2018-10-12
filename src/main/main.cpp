// main.cpp

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hpp"
#include "gl_loader.hpp"
#include "input_handler.hpp"
#include "time.hpp"
#include "triangles_parser.hpp"
#include "vertex.hpp"

#include "config.hpp"

const int fixedFPS{20};

int main(int argc, char *argv[]) {
  auto &engine = levi::engine::instance();
  auto &input_handler = levi::input_handler::instance();

  bool game_stop = false;

  auto &gl_func = levi::gl_loader::instance();

  GLuint vbo{};
  gl_func.glGenBuffers(1, &vbo);
  LEVI_CHECK();
  gl_func.glBindBuffer(GL_ARRAY_BUFFER, vbo);
  LEVI_CHECK();

  GLuint ebo{};
  gl_func.glGenBuffers(1, &ebo);
  LEVI_CHECK();
  gl_func.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  LEVI_CHECK();

  while (!game_stop) {
    auto last_tick = levi::get_time();

    input_handler.update();
    game_stop = input_handler.is_quit();
    engine.update();

    std::vector<vertex> vertices;
    std::vector<uint32_t> elements;

    if (!levi::triangles_parser(triangles_xml, vertices, elements)) {
      std::cerr << "problem in parser!\n";
    }

    gl_func.glBufferData(GL_ARRAY_BUFFER,
                         vertices.size() *
                             sizeof(decltype(vertices)::value_type),
                         &vertices[0], GL_STATIC_DRAW);
    LEVI_CHECK();

    gl_func.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         elements.size() *
                             sizeof(decltype(elements)::value_type),
                         &elements[0], GL_STATIC_DRAW);
    LEVI_CHECK();

    engine.render();

    auto dif_time = levi::get_time() - last_tick;
    if (dif_time < 1000 / fixedFPS) {
      levi::delay(1000 / fixedFPS - dif_time);
    }
  }

  gl_func.glDeleteBuffers(1, &ebo);
  LEVI_CHECK();
  gl_func.glDeleteBuffers(1, &vbo);
  LEVI_CHECK();

  return EXIT_SUCCESS;
}
