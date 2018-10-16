// engine.cpp

#include "engine.hpp"

#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "event.hpp"
#include "gl_loader.hpp"
#include "state_machine.hpp"
#include "texture_manager.hpp"
#include "vector2d.hpp"
#include "vertex.hpp"

const int default_number_of_ebo{6};

const char *const gl_vertex_shader_code{
    R"(
    attribute vec3 pos;
    attribute vec2 tex_pos;
    uniform vec2 center_of_rotation;
    uniform float radian_angle;
    uniform vec2 win_size;
    uniform vec2 tex_size;

    varying vec2 v_tex_pos;

    void main() {
      mat3 transfer;
      transfer[0] = vec3(1, 0, 0);
      transfer[1] = vec3(0, 1, 0);
      transfer[2] = vec3(-center_of_rotation.x, -center_of_rotation.y, 1);

      mat3 rotade;
      float sin_angle = sin(radian_angle);
      float cos_angle = cos(radian_angle);
      rotade[0] = vec3(cos_angle, sin_angle, 0);
      rotade[1] = vec3(-sin_angle, cos_angle, 0);
      rotade[2] = vec3(0, 0, 1);

      mat3 to_ndc;
      to_ndc[0] = vec3(2.0 / win_size.x, 0, 0);
      to_ndc[1] = vec3(0, -2.0 / win_size.y, 0);
      to_ndc[2] = vec3(-1, 1, 1);

      mat3 rezult = rotade * transfer;
      transfer[2][0] = center_of_rotation.x;
      transfer[2][1] = center_of_rotation.y;
      rezult = transfer * rezult;
      rezult = to_ndc * rezult;

      vec4 out_pos = vec4(rezult * pos, 1);
      gl_Position = vec4(rezult * pos, 1);

      mat2 tex_to_ndc;
      tex_to_ndc[0] = vec2(1.0 / tex_size.x, 0);
      tex_to_ndc[1] = vec2(0, 1.0 / tex_size.y);

      v_tex_pos = tex_to_ndc * tex_pos;
    }
    )"};

const char *const gl_fragment_shader_code{
    R"(
    varying vec2 v_tex_pos;

    uniform sampler2D tex;

    void main() {
      gl_FragColor = texture2D(tex, v_tex_pos);
    }
    )"};

levi::engine &levi::engine::instance() {
  static engine retval{};
  return retval;
}

levi::engine::engine()
    : window_{nullptr}, gl_context_{}, state_machine_{new levi::state_machine},
      texture_manager_{new levi::texture_manager{}}, shader_program_{}, vao_{} {
  if (::SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  window_ = ::SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, 1000, 500,
                               ::SDL_WINDOW_OPENGL);
  if (!window_) {
    throw std::runtime_error{::SDL_GetError()};
  }

  int gl_major_version{2};
  int gl_minor_version{0};

  if (::SDL_GL_SetAttribute(::SDL_GL_CONTEXT_PROFILE_MASK,
                            ::SDL_GL_CONTEXT_PROFILE_CORE) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  if (::SDL_GL_SetAttribute(::SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version) <
      0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  if (::SDL_GL_SetAttribute(::SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version) <
      0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  gl_context_ = ::SDL_GL_CreateContext(window_);
  if (!gl_context_) {
    throw std::runtime_error{::SDL_GetError()};
  }

  int cur_major_version{};
  int cur_minor_version{};
  int profile{};
  if (::SDL_GL_GetAttribute(::SDL_GL_CONTEXT_PROFILE_MASK, &profile) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }
  if (::SDL_GL_GetAttribute(::SDL_GL_CONTEXT_MAJOR_VERSION,
                            &cur_major_version) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }
  if (::SDL_GL_GetAttribute(::SDL_GL_CONTEXT_MINOR_VERSION,
                            &cur_minor_version) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  if (profile != ::SDL_GL_CONTEXT_PROFILE_CORE ||
      cur_major_version != gl_major_version ||
      cur_minor_version != gl_minor_version) {
    throw std::runtime_error{"couldn't created right gl_context"};
  }
  ::glClearColor(0, 1, 0, 1);

  auto &gl_functions = gl_loader::instance();

  auto vertex_shader = gl_functions.glCreateShader(GL_VERTEX_SHADER);
  LEVI_CHECK();
  gl_functions.glShaderSource(vertex_shader, 1, &gl_vertex_shader_code,
                              nullptr);
  LEVI_CHECK();
  gl_functions.glCompileShader(vertex_shader);
  LEVI_CHECK();
  GLint compile_status{};
  gl_functions.glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLint length_of_info{};
    gl_functions.glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH,
                               &length_of_info);
    std::string info_log{};
    info_log.resize(length_of_info);
    gl_functions.glGetShaderInfoLog(vertex_shader, length_of_info, nullptr,
                                    &info_log[0]);
    throw std::runtime_error{"error while compile vertex shader:\n" + info_log};
  }

  auto fragment_shader = gl_functions.glCreateShader(GL_FRAGMENT_SHADER);
  LEVI_CHECK();
  gl_functions.glShaderSource(fragment_shader, 1, &gl_fragment_shader_code,
                              nullptr);
  LEVI_CHECK();
  gl_functions.glCompileShader(fragment_shader);
  LEVI_CHECK();
  gl_functions.glGetShaderiv(fragment_shader, GL_COMPILE_STATUS,
                             &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLint length_of_info{};
    gl_functions.glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH,
                               &length_of_info);
    std::string info_log{};
    info_log.resize(length_of_info);
    gl_functions.glGetShaderInfoLog(fragment_shader, length_of_info, nullptr,
                                    &info_log[0]);
    throw std::runtime_error{"error while compile fragment shader:\n" +
                             info_log};
  }

  shader_program_ = gl_functions.glCreateProgram();
  LEVI_CHECK();
  gl_functions.glAttachShader(shader_program_, vertex_shader);
  LEVI_CHECK();
  gl_functions.glAttachShader(shader_program_, fragment_shader);
  LEVI_CHECK();
  gl_functions.glLinkProgram(shader_program_);
  LEVI_CHECK();
  GLint link_status{};
  gl_functions.glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);
  LEVI_CHECK();
  if (!link_status) {
    throw std::runtime_error{"shader program couldn't be linked"};
  }
  gl_functions.glUseProgram(shader_program_);
  LEVI_CHECK();

  // we mark shaders to delete - they will be deleted when detach of shader
  // program
  gl_functions.glDeleteShader(vertex_shader);
  LEVI_CHECK();
  gl_functions.glDeleteShader(fragment_shader);
  LEVI_CHECK();

  gl_functions.glGenVertexArrays(1, &vao_);
  LEVI_CHECK();
  gl_functions.glBindVertexArray(vao_);
  LEVI_CHECK();

  gl_functions.glGenBuffers(1, &vbo_);
  LEVI_CHECK();
  gl_functions.glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  LEVI_CHECK();

  gl_functions.glGenBuffers(1, &ebo_);
  LEVI_CHECK();
  gl_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  LEVI_CHECK();

  pos_ = gl_functions.glGetAttribLocation(shader_program_, "pos");
  LEVI_CHECK();
  if (pos_ < 0) {
    throw std::runtime_error{"couldn't locate pos in shader program"};
  }
  gl_functions.glVertexAttribPointer(pos_, 3, GL_FLOAT, GL_FALSE,
                                     sizeof(vertex) * 2, nullptr);
  LEVI_CHECK();
  gl_functions.glEnableVertexAttribArray(pos_);
  LEVI_CHECK();

  tex_pos_ = gl_functions.glGetAttribLocation(shader_program_, "tex_pos");
  LEVI_CHECK();
  if (tex_pos_ < 0) {
    throw std::runtime_error{"couldn't locate tex_pos in shader program"};
  }
  gl_functions.glVertexAttribPointer(tex_pos_, 2, GL_FLOAT, GL_FALSE,
                                     sizeof(vertex) * 2,
                                     reinterpret_cast<void *>(sizeof(vertex)));
  LEVI_CHECK();
  gl_functions.glEnableVertexAttribArray(tex_pos_);
  LEVI_CHECK();
}

levi::engine::~engine() {
  auto &gl_functions = gl_loader::instance();
  gl_functions.glDisableVertexArray(tex_pos_);
  gl_functions.glDisableVertexArray(pos_);
  gl_functions.glDeleteBuffers(1, &ebo_);
  gl_functions.glDeleteBuffers(1, &vbo_);
  gl_functions.glDeleteBuffers(1, &vao_);
  gl_functions.glDeleteProgram(shader_program_);
  ::SDL_GL_DeleteContext(gl_context_);
  ::SDL_DestroyWindow(window_);
  delete state_machine_;
  delete texture_manager_;
  ::SDL_Quit();
}

levi::state_machine &levi::engine::state_machine() { return *state_machine_; }

levi::texture_manager &levi::engine::texture_manager() {
  return *texture_manager_;
}

void levi::engine::update() { state_machine_->update(); }

void levi::engine::render() {
  ::glClear(GL_COLOR_BUFFER_BIT);

  levi::render(*this, state_machine_);

  ::SDL_GL_SwapWindow(window_);
}

levi::size levi::engine::get_window_size() const {
  int w{};
  int h{};
  ::SDL_GetWindowSize(window_, &w, &h);
  return size{w, h};
}

void levi::engine::draw(const texture &texture, const rect &src_rect,
                        const rect &dst_rect, double angle) {
  auto global_vertices = dst_rect.get_vertices();

  auto win_size = get_window_size();
  auto center = dst_rect.get_center();
  angle = to_radians(angle);

  auto texture_vertices = src_rect.get_vertices();

  std::array<vertex, 8> vertices;
  for (int i = 0; i < vertices.size() / 2; ++i) {
    vertices[i * 2] = global_vertices[i];
    vertices[i * 2 + 1] = texture_vertices[i];
  }

  auto &gl_functions = gl_loader::instance();

  ::glActiveTexture(GL_TEXTURE0);
  LEVI_CHECK();
  ::glBindTexture(GL_TEXTURE_2D, texture.gl_tex);
  LEVI_CHECK();

  gl_functions.glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  LEVI_CHECK();
  gl_functions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex),
                            &vertices[0], GL_STATIC_DRAW);
  LEVI_CHECK();

  std::array<uint32_t, default_number_of_ebo> elements{0, 1, 2, 0, 2, 3};
  gl_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  LEVI_CHECK();
  gl_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            elements.size() *
                                sizeof(decltype(elements)::value_type),
                            &elements[0], GL_STATIC_DRAW);
  LEVI_CHECK();

  auto uniform_center =
      gl_functions.glGetUniformLocation(shader_program_, "center_of_rotation");
  LEVI_CHECK();
  gl_functions.glUniform2f(uniform_center, center.x, center.y);
  LEVI_CHECK();
  auto uniform_angle =
      gl_functions.glGetUniformLocation(shader_program_, "radian_angle");
  LEVI_CHECK();
  gl_functions.glUniform1f(uniform_angle, angle);
  LEVI_CHECK();
  auto uniform_win_size =
      gl_functions.glGetUniformLocation(shader_program_, "win_size");
  LEVI_CHECK();
  gl_functions.glUniform2f(uniform_win_size, win_size.width, win_size.height);
  LEVI_CHECK();
  auto uniform_tex_size =
      gl_functions.glGetUniformLocation(shader_program_, "tex_size");
  LEVI_CHECK();
  gl_functions.glUniform2f(uniform_tex_size, texture.width, texture.height);
  LEVI_CHECK();

  ::glDrawElements(GL_TRIANGLES, default_number_of_ebo, GL_UNSIGNED_INT,
                   nullptr);
  LEVI_CHECK();
}
