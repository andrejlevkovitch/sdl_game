// engine.cpp

#include "engine.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "event.hpp"
#include "gl_loader.hpp"
#include "vertex.hpp"

levi::engine &levi::engine::instance() {
  static engine retval{};
  return retval;
}

levi::engine::engine()
    : window_{nullptr},
      gl_context_{nullptr}, v_shader_{}, f_shader_{}, program_{} {
  if (::SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  window_ =
      ::SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 640, 480, ::SDL_WINDOW_OPENGL);
  if (!window_) {
    throw std::runtime_error{::SDL_GetError()};
  }
  ::glClearColor(0, 0, 0, 0);

  if (::SDL_GL_SetAttribute(::SDL_GL_CONTEXT_PROFILE_MASK,
                            ::SDL_GL_CONTEXT_PROFILE_CORE) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  int need_gl_version_major{2};
  int need_gl_version_minor{0};

  if (::SDL_GL_SetAttribute(::SDL_GL_CONTEXT_MAJOR_VERSION,
                            need_gl_version_major) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  if (::SDL_GL_SetAttribute(::SDL_GL_CONTEXT_MINOR_VERSION,
                            need_gl_version_minor) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  gl_context_ = ::SDL_GL_CreateContext(window_);
  if (!gl_context_) {
    throw std::runtime_error{::SDL_GetError()};
  }

  int profile{};
  if (::SDL_GL_GetAttribute(::SDL_GL_CONTEXT_PROFILE_MASK, &profile) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  } else {
    SDL_assert(profile == ::SDL_GL_CONTEXT_PROFILE_CORE);
  }

  int gl_version_major{};
  int gl_version_minor{};
  if (::SDL_GL_GetAttribute(::SDL_GL_CONTEXT_MAJOR_VERSION, &gl_version_major) <
      0) {
    throw std::runtime_error{::SDL_GetError()};
  } else {
    SDL_assert(gl_version_major == need_gl_version_major);
  }

  if (::SDL_GL_GetAttribute(::SDL_GL_CONTEXT_MINOR_VERSION, &gl_version_minor) <
      0) {
    throw std::runtime_error{::SDL_GetError()};
  } else {
    SDL_assert(gl_version_minor == need_gl_version_minor);
  }

  auto &gl_func = gl_loader::instance();

  const char *vertex_shader_code = R"(
    attribute vec2 position;
    attribute vec3 color;
    varying vec3 out_color;
    void main() {
     gl_Position = vec4(position, 0, 1);
     out_color = color;
    }
 )";

  v_shader_ = gl_func.glCreateShader(GL_VERTEX_SHADER);
  LEVI_CHECK();
  gl_func.glShaderSource(v_shader_, 1, &vertex_shader_code, nullptr);
  LEVI_CHECK();
  gl_func.glCompileShader(v_shader_);
  LEVI_CHECK();
  GLint compile_status{};
  gl_func.glGetShaderiv(v_shader_, GL_COMPILE_STATUS, &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLchar log[512];
    gl_func.glGetShaderInfoLog(v_shader_, 512, nullptr, log);
    throw std::runtime_error{
        std::string{"bad compile status for vertex_shader\n"} +
        reinterpret_cast<const char *>(log)};
  }

  const char *fragment_shader_code = R"(
    varying vec3 out_color;
    void main() {
     gl_FragColor = vec4(out_color, 1.0);
    }
  )";

  f_shader_ = gl_func.glCreateShader(GL_FRAGMENT_SHADER);
  LEVI_CHECK();
  gl_func.glShaderSource(f_shader_, 1, &fragment_shader_code, nullptr);
  LEVI_CHECK();
  gl_func.glCompileShader(f_shader_);
  LEVI_CHECK();
  gl_func.glGetShaderiv(f_shader_, GL_COMPILE_STATUS, &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLchar log[512];
    gl_func.glGetShaderInfoLog(f_shader_, 512, nullptr, log);
    throw std::runtime_error{
        std::string{"bad compile status for fragment_shader\n"} +
        reinterpret_cast<const char *>(log)};
  }

  program_ = gl_func.glCreateProgram();
  LEVI_CHECK();
  if (!program_) {
    throw std::runtime_error{"program not created"};
  }
  gl_func.glAttachShader(program_, v_shader_);
  LEVI_CHECK();
  gl_func.glAttachShader(program_, f_shader_);
  LEVI_CHECK();

  gl_func.glLinkProgram(program_);
  LEVI_CHECK();
  GLint link_status{};
  gl_func.glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
  LEVI_CHECK();
  if (!link_status) {
    throw std::runtime_error{"program not linked"};
  }
  gl_func.glUseProgram(program_);
  LEVI_CHECK();

  gl_func.glGenVertexArrays(1, &vao_);
  LEVI_CHECK();
  gl_func.glBindVertexArray(vao_);
  LEVI_CHECK();
}

levi::engine::~engine() {
  auto &gl_func = gl_loader::instance();

  gl_func.glDeleteVertexArrays(1, &vao_);
  gl_func.glDetachShader(program_, f_shader_);
  gl_func.glDetachShader(program_, v_shader_);
  gl_func.glDeleteShader(f_shader_);
  gl_func.glDeleteShader(v_shader_);
  gl_func.glDeleteProgram(program_);

  ::SDL_GL_DeleteContext(gl_context_);
  ::SDL_DestroyWindow(window_);
  ::SDL_Quit();
}

void levi::engine::update() {}

void levi::engine::render(size_t n_vertex) {
  ::glClear(GL_COLOR_BUFFER_BIT);

  auto &gl_func = gl_loader::instance();

  auto pos_attribute = gl_func.glGetAttribLocation(program_, "position");
  LEVI_CHECK();
  if (pos_attribute < 0) {
    throw std::runtime_error{"undefinded attribute"};
  }
  gl_func.glVertexAttribPointer(pos_attribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(vertex), 0);
  LEVI_CHECK();
  gl_func.glEnableVertexAttribArray(pos_attribute);
  LEVI_CHECK();

  auto color_attribute = gl_func.glGetAttribLocation(program_, "color");
  LEVI_CHECK();
  if (pos_attribute < 0) {
    throw std::runtime_error{"undefinded attribute"};
  }
  gl_func.glVertexAttribPointer(color_attribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(vertex),
                                reinterpret_cast<void *>(sizeof(float) * 2));
  LEVI_CHECK();
  gl_func.glEnableVertexAttribArray(color_attribute);
  LEVI_CHECK();

  ::glDrawElements(GL_TRIANGLES, n_vertex, GL_UNSIGNED_INT, 0);
  LEVI_CHECK();

  ::SDL_GL_SwapWindow(window_);
}

levi::size levi::engine::get_window_size() const { return win_size_; }
