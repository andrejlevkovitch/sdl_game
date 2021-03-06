// engine.cpp

#include "engine.hpp"
#include "event.hpp"
#include "gl_loader.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "input_handler.hpp"
#include "menu_imgui.hpp"
#include "shaders_config.hpp"
#include "state_machine.hpp"
#include "texture_manager.hpp"
#include "time.hpp"
#include "vector2d.hpp"
#include "vertex.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

// values for binding shader attributes
const GLuint position{1};
const GLuint texture_position{2};

// current update interval
static unsigned int delta_ups{};

namespace levi {
class shader_attacher {
public:
  shader_attacher(uint32_t program, uint32_t shader1, uint32_t shader2);
  ~shader_attacher();

private:
  uint32_t program_;
  uint32_t shader1_;
  uint32_t shader2_;
};

std::string read_shader_code_from_file(const std::string &file);
uint32_t create_shader(uint32_t type, const std::string &shader_code);

} // namespace levi

levi::engine &levi::engine::instance() {
  try {
    static engine retval{};
    return retval;
  } catch (std::exception) {
    throw;
  }
}

levi::engine::engine()
    : window_{nullptr}, gl_context_{}, state_machine_{new levi::state_machine},
      texture_manager_{new levi::texture_manager{}}, shader_program_{},
      general_light_{1, 1, 1, 1}, fps_{}, ups_{}, update_interval_{30},
      render_interval_{30} {
  if (::SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error{::SDL_GetError()};
  }

  window_ = ::SDL_CreateWindow("bomberman", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, 1088, 832,
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

  gl_context_ = ::SDL_GL_CreateContext(reinterpret_cast<SDL_Window *>(window_));
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

  auto &gl_functions = gl_loader::instance();

  std::string vertex_shader_code =
      read_shader_code_from_file(levi::way_to_shaders + "vertex_shader.glsl");
  std::string fragment_shader_code =
      read_shader_code_from_file(levi::way_to_shaders + "fragment_shader.glsl");
  std::string texture_multiply_vertex_shader_code = read_shader_code_from_file(
      levi::way_to_shaders + "texture_multiply_vertex_shader.glsl");
  std::string texture_multiply_fragment_shader_code =
      read_shader_code_from_file(levi::way_to_shaders +
                                 "texture_multiply_fragment_shader.glsl");
  std::string general_vertex_shader_code = read_shader_code_from_file(
      levi::way_to_shaders + "general_vertex_shader.glsl");
  std::string general_fragment_shader_code = read_shader_code_from_file(
      levi::way_to_shaders + "general_fragment_shader.glsl");

  try {
    shaders_[0] = create_shader(GL_VERTEX_SHADER, vertex_shader_code);
    shaders_[1] = create_shader(GL_FRAGMENT_SHADER, fragment_shader_code);
    shaders_[2] =
        create_shader(GL_VERTEX_SHADER, texture_multiply_vertex_shader_code);
    shaders_[3] = create_shader(GL_FRAGMENT_SHADER,
                                texture_multiply_fragment_shader_code);
    shaders_[4] = create_shader(GL_VERTEX_SHADER, general_vertex_shader_code);
    shaders_[5] =
        create_shader(GL_FRAGMENT_SHADER, general_fragment_shader_code);
  } catch (std::exception &) {
    throw;
  }

  shader_program_ = gl_functions.glCreateProgram();
  LEVI_CHECK();
  // gl_functions.glAttachShader(shader_program_, shaders_[0]);
  // LEVI_CHECK();
  // gl_functions.glAttachShader(shader_program_, shaders_[1]);
  // LEVI_CHECK();

  // binding location of attributes
  // have effect only past call linkprogram, and have to set past shader attach
  // gl_functions.glDetachShader(shader_program_, shaders_[0]);
  // LEVI_CHECK();
  // gl_functions.glDetachShader(shader_program_, shaders_[1]);
  // LEVI_CHECK();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ::ImGui_ImplSDL2_InitForOpenGL(reinterpret_cast<SDL_Window *>(window_),
                                 gl_context_);
  ::ImGui_ImplOpenGL3_Init("#version 130");
  ::ImGui::StyleColorsClassic();
}

levi::engine::~engine() {
  ::ImGui_ImplOpenGL3_Shutdown();
  ::ImGui_ImplSDL2_Shutdown();
  ::ImGui::DestroyContext();

  auto &gl_functions = gl_loader::instance();
  gl_functions.glDeleteProgram(shader_program_);
  for (auto &i : shaders_) {
    gl_functions.glDeleteShader(i);
  }

  ::SDL_GL_DeleteContext(gl_context_);
  ::SDL_DestroyWindow(reinterpret_cast<SDL_Window *>(window_));

  delete state_machine_;
  delete texture_manager_;

  ::SDL_Quit();
}

levi::state_machine &levi::engine::state_machine() { return *state_machine_; }

levi::texture_manager &levi::engine::texture_manager() {
  return *texture_manager_;
}

void levi::engine::update(unsigned int delta_t_ms) {
  ups_ += delta_t_ms;
  if (ups_ >= update_interval_) {
    static auto last_ups_time = levi::get_time();

    input_handler::instance().update();
    for (auto &i : input_handler::instance().get_event_list()) {
      if (i.type == event_type::button_event &&
          i.button.code == button_code::show_info &&
          i.button.state == button_state::pressed) {
        menu_imgui::instance() = !menu_imgui::instance();
      }
    }
    state_machine_->update();

    delta_ups = levi::get_time() - last_ups_time;
    last_ups_time = levi::get_time();
    ups_ = 0;
  }
}

void levi::engine::render(unsigned int delta_t_ms) {
  fps_ += delta_t_ms;
  if (fps_ >= render_interval_) {
    auto win_size = get_window_size();

    ::ImGui_ImplOpenGL3_NewFrame();
    ::ImGui_ImplSDL2_NewFrame(reinterpret_cast<SDL_Window *>(window_));
    ImGui::NewFrame();

    if (menu_imgui::instance()) {
      ImGui::Begin("Game Menu");
      static auto last_fps_time{get_time()};
      ImGui::Text("time form start:\n %i", get_time() / 1000);
      ImGui::Separator();
      ImGui::Text("ups:\n %.1f", 1000. / delta_ups);
      ImGui::Text("fps:\n %.1f", 1000. / (get_time() - last_fps_time));
      last_fps_time = get_time();
      ImGui::ColorEdit4("light color", &general_light_[0]);
      ImGui::End();
    }

    auto &gl_functions = gl_loader::instance();

    GLuint light_frame{};
    GLuint light_texture{};
    {
      shader_attacher attacher{shader_program_, shaders_[2], shaders_[3]};

      gl_functions.glGenFramebuffers(1, &light_frame);
      LEVI_CHECK();
      gl_functions.glBindFramebuffer(GL_FRAMEBUFFER, light_frame);
      LEVI_CHECK();

      ::glGenTextures(1, &light_texture);
      LEVI_CHECK();
      ::glBindTexture(GL_TEXTURE_2D, light_texture);
      LEVI_CHECK();
      ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, win_size.width, win_size.height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      LEVI_CHECK();
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      LEVI_CHECK();
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      LEVI_CHECK();
      ::glBindTexture(GL_TEXTURE_2D, 0);
      LEVI_CHECK();

      gl_functions.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                          GL_TEXTURE_2D, light_texture, 0);
      LEVI_CHECK();

      if (gl_functions.glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
          GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error{"framebuffer for light not complete"};
      }
      LEVI_CHECK();

      ::glViewport(0, 0, win_size.width, win_size.height);
      ::glEnable(GL_BLEND);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      ::glClearColor(general_light_[0], general_light_[1], general_light_[2],
                     general_light_[3]);
      ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      state_machine_->calculate_light(*this);

      ::glDisable(GL_BLEND);

      gl_functions.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint render_frame{};
    GLuint render_texture{};
    {
      shader_attacher attacher{shader_program_, shaders_[0], shaders_[1]};

      gl_functions.glGenFramebuffers(1, &render_frame);
      LEVI_CHECK();
      gl_functions.glBindFramebuffer(GL_FRAMEBUFFER, render_frame);
      LEVI_CHECK();

      ::glGenTextures(1, &render_texture);
      LEVI_CHECK();
      ::glBindTexture(GL_TEXTURE_2D, render_texture);
      LEVI_CHECK();
      ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, win_size.width, win_size.height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      LEVI_CHECK();
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      LEVI_CHECK();
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      LEVI_CHECK();
      ::glBindTexture(GL_TEXTURE_2D, 0);
      LEVI_CHECK();

      // renderbuffer for depth
      GLuint render_buffer{};
      gl_functions.glGenRenderBuffers(1, &render_buffer);
      LEVI_CHECK();
      gl_functions.glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
      LEVI_CHECK();
      gl_functions.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                                         win_size.width, win_size.height);
      LEVI_CHECK();
      gl_functions.glBindRenderbuffer(GL_RENDERBUFFER, 0);
      LEVI_CHECK();

      gl_functions.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                          GL_TEXTURE_2D, render_texture, 0);
      LEVI_CHECK();
      gl_functions.glFramebufferRenderbuffer(
          GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

      if (gl_functions.glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
          GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error{"framebuffer for light not complete"};
      }
      LEVI_CHECK();

      ::glViewport(0, 0, win_size.width, win_size.height);
      ::glEnable(GL_BLEND);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      ::glEnable(GL_DEPTH_TEST);
      ::glDepthFunc(GL_LEQUAL);

      ::glClearColor(0, 0, 0, 1);
      ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      state_machine_->render(*this);

      ::glDisable(GL_BLEND);
      ::glDisable(GL_DEPTH_TEST);

      gl_functions.glDeleteRenderbuffers(1, &render_buffer);

      gl_functions.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    {
      shader_attacher attacher{shader_program_, shaders_[4], shaders_[5]};

      gl_functions.glBindFramebuffer(GL_FRAMEBUFFER, 0);

      GLuint vbo{};
      GLuint ebo{};

      std::array<vertex, 8> vertices{vertex{-1, 1, 0},  vertex{0, 1, 0},
                                     vertex{-1, -1, 0}, vertex{0, 0, 0},
                                     vertex(1, -1, 0),  vertex{1, 0, 0},
                                     vertex{1, 1, 0},   vertex{1, 1, 0}};

      gl_functions.glGenBuffers(1, &vbo);
      LEVI_CHECK();
      gl_functions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
      LEVI_CHECK();
      gl_functions.glBufferData(GL_ARRAY_BUFFER,
                                vertices.size() * sizeof(vertex), &vertices[0],
                                GL_STATIC_DRAW);
      LEVI_CHECK();

      // always looks like this, because we draw rectangles
      std::array<uint32_t, 4> elements{1, 0, 2, 3};
      gl_functions.glGenBuffers(1, &ebo);
      LEVI_CHECK();
      gl_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      LEVI_CHECK();
      gl_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                elements.size() *
                                    sizeof(decltype(elements)::value_type),
                                &elements[0], GL_STATIC_DRAW);
      LEVI_CHECK();

      gl_functions.glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE,
                                         sizeof(vertex) * 2, nullptr);
      LEVI_CHECK();
      gl_functions.glEnableVertexAttribArray(position);
      LEVI_CHECK();

      gl_functions.glGetAttribLocation(shader_program_, "pos");
      gl_functions.glVertexAttribPointer(
          texture_position, 2, GL_FLOAT, GL_FALSE, sizeof(vertex) * 2,
          reinterpret_cast<void *>(sizeof(vertex)));
      LEVI_CHECK();
      gl_functions.glEnableVertexAttribArray(texture_position);
      LEVI_CHECK();

      gl_functions.glActiveTexture(GL_TEXTURE0);
      LEVI_CHECK();
      ::glBindTexture(GL_TEXTURE_2D, render_texture);
      LEVI_CHECK();
      gl_functions.glUniform1i(
          gl_functions.glGetUniformLocation(shader_program_, "render_tex"), 0);
      LEVI_CHECK();

      gl_functions.glActiveTexture(GL_TEXTURE1);
      LEVI_CHECK();
      ::glBindTexture(GL_TEXTURE_2D, light_texture);
      LEVI_CHECK();
      gl_functions.glUniform1i(
          gl_functions.glGetUniformLocation(shader_program_, "light_tex"), 1);
      LEVI_CHECK();

      ::glViewport(0, 0, win_size.width, win_size.height);
      ::glClearColor(0, 0, 0, 1);
      ::glClear(GL_COLOR_BUFFER_BIT);

      glDrawElements(GL_TRIANGLE_STRIP, elements.size(), GL_UNSIGNED_INT,
                     nullptr);

      gl_functions.glDisableVertexAttribArray(texture_position);
      LEVI_CHECK();
      gl_functions.glDisableVertexAttribArray(position);
      LEVI_CHECK();
      gl_functions.glDeleteBuffers(1, &ebo);
      LEVI_CHECK();
      gl_functions.glDeleteBuffers(1, &vbo);
      LEVI_CHECK();
    }

    ImGui::Render();

    ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());

    ::SDL_GL_SwapWindow(reinterpret_cast<SDL_Window *>(window_));

    ::glDeleteTextures(1, &light_texture);
    gl_functions.glDeleteFramebuffers(1, &light_frame);
    ::glDeleteTextures(1, &render_texture);
    gl_functions.glDeleteFramebuffers(1, &render_frame);

    fps_ = 0;
  }
}

levi::size levi::engine::get_window_size() const {
  int w{};
  int h{};
  ::SDL_GetWindowSize(reinterpret_cast<SDL_Window *>(window_), &w, &h);
  return size{w, h};
}

void levi::engine::draw(const texture &texture, const rect &src_rect,
                        const rect &dst_rect, float angle, flip flip_,
                        depth depth_) {
  auto global_vertices = dst_rect.get_vertices(depth_ / 10.);

  auto win_size = get_window_size();
  auto center = dst_rect.get_center();

  auto texture_vertices = src_rect.get_vertices();

  if (flip_ & flip::horizontal) {
    std::swap(texture_vertices[0], texture_vertices[3]);
    std::swap(texture_vertices[1], texture_vertices[2]);
  }
  if (flip_ & flip::vertical) {
    std::swap(texture_vertices[0], texture_vertices[1]);
    std::swap(texture_vertices[3], texture_vertices[2]);
  }

  std::array<vertex, 8> vertices;
  for (unsigned i = 0; i < vertices.size() / 2; ++i) {
    vertices[i * 2] = global_vertices[i];
    vertices[i * 2 + 1] = texture_vertices[i];
  }

  auto &gl_functions = gl_loader::instance();

  GLuint vbo{};
  GLuint ebo{};

  gl_functions.glGenBuffers(1, &vbo);
  LEVI_CHECK();
  gl_functions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
  LEVI_CHECK();
  gl_functions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex),
                            &vertices[0], GL_STATIC_DRAW);
  LEVI_CHECK();

  // always looks like this, because we draw rectangles
  std::array<uint32_t, 4> elements{1, 0, 2, 3};
  gl_functions.glGenBuffers(1, &ebo);
  LEVI_CHECK();
  gl_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  LEVI_CHECK();
  gl_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            elements.size() *
                                sizeof(decltype(elements)::value_type),
                            &elements[0], GL_STATIC_DRAW);
  LEVI_CHECK();

  gl_functions.glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
                                     sizeof(vertex) * 2, nullptr);
  LEVI_CHECK();
  gl_functions.glEnableVertexAttribArray(position);
  LEVI_CHECK();

  gl_functions.glVertexAttribPointer(texture_position, 2, GL_FLOAT, GL_FALSE,
                                     sizeof(vertex) * 2,
                                     reinterpret_cast<void *>(sizeof(vertex)));
  LEVI_CHECK();
  gl_functions.glEnableVertexAttribArray(texture_position);
  LEVI_CHECK();

  // we use only one texture in shaders, so it always be GL_TEXTURE0
  gl_functions.glActiveTexture(GL_TEXTURE0);
  LEVI_CHECK();
  ::glBindTexture(GL_TEXTURE_2D, texture.gl_tex);
  LEVI_CHECK();
  gl_functions.glUniform1i(
      gl_functions.glGetUniformLocation(shader_program_, "tex"), 0);
  LEVI_CHECK();

  auto uniform_center =
      gl_functions.glGetUniformLocation(shader_program_, "center_of_rotation");
  LEVI_CHECK();
  gl_functions.glUniform2f(uniform_center, center.x, center.y);
  LEVI_CHECK();
  auto uniform_angle =
      gl_functions.glGetUniformLocation(shader_program_, "degrees_angle");
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

  ::glDrawElements(GL_TRIANGLE_STRIP, elements.size(), GL_UNSIGNED_INT,
                   nullptr);
  LEVI_CHECK();

  gl_functions.glDisableVertexAttribArray(texture_position);
  LEVI_CHECK();
  gl_functions.glDisableVertexAttribArray(position);
  LEVI_CHECK();
  gl_functions.glDeleteBuffers(1, &ebo);
  LEVI_CHECK();
  gl_functions.glDeleteBuffers(1, &vbo);
  LEVI_CHECK();
}

void levi::engine::draw_light(const texture &texture, const rect &src_rect,
                              const rect &dst_rect, vertex color) {
  auto global_vertices = dst_rect.get_vertices(0);

  auto win_size = get_window_size();

  auto texture_vertices = src_rect.get_vertices();

  std::array<vertex, 8> vertices;
  for (unsigned i = 0; i < vertices.size() / 2; ++i) {
    vertices[i * 2] = global_vertices[i];
    vertices[i * 2 + 1] = texture_vertices[i];
  }

  auto &gl_functions = gl_loader::instance();

  GLuint vbo{};
  GLuint ebo{};

  gl_functions.glGenBuffers(1, &vbo);
  LEVI_CHECK();
  gl_functions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
  LEVI_CHECK();
  gl_functions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex),
                            &vertices[0], GL_STATIC_DRAW);
  LEVI_CHECK();

  // always looks like this, because we draw rectangles
  std::array<uint32_t, 4> elements{1, 0, 2, 3};
  gl_functions.glGenBuffers(1, &ebo);
  LEVI_CHECK();
  gl_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  LEVI_CHECK();
  gl_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            elements.size() *
                                sizeof(decltype(elements)::value_type),
                            &elements[0], GL_STATIC_DRAW);
  LEVI_CHECK();

  gl_functions.glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
                                     sizeof(vertex) * 2, nullptr);
  LEVI_CHECK();
  gl_functions.glEnableVertexAttribArray(position);
  LEVI_CHECK();

  gl_functions.glVertexAttribPointer(texture_position, 2, GL_FLOAT, GL_FALSE,
                                     sizeof(vertex) * 2,
                                     reinterpret_cast<void *>(sizeof(vertex)));
  LEVI_CHECK();
  gl_functions.glEnableVertexAttribArray(texture_position);
  LEVI_CHECK();

  // we use only one texture in shaders, so it always be GL_TEXTURE0
  gl_functions.glActiveTexture(GL_TEXTURE0);
  LEVI_CHECK();
  ::glBindTexture(GL_TEXTURE_2D, texture.gl_tex);
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
  auto uniform_color =
      gl_functions.glGetUniformLocation(shader_program_, "color");
  LEVI_CHECK();
  gl_functions.glUniform3f(uniform_color, color.x, color.y, color.z);
  LEVI_CHECK();

  ::glDrawElements(GL_TRIANGLE_STRIP, elements.size(), GL_UNSIGNED_INT,
                   nullptr);
  LEVI_CHECK();

  gl_functions.glDisableVertexAttribArray(texture_position);
  LEVI_CHECK();
  gl_functions.glDisableVertexAttribArray(position);
  LEVI_CHECK();
  gl_functions.glDeleteBuffers(1, &ebo);
  LEVI_CHECK();
  gl_functions.glDeleteBuffers(1, &vbo);
  LEVI_CHECK();
}

void levi::engine::set_light(unsigned char r, unsigned char g, unsigned char b,
                             unsigned char a) {
  general_light_[0] = r;
  general_light_[1] = g;
  general_light_[2] = b;
  general_light_[3] = a;
}

namespace levi {
shader_attacher::shader_attacher(uint32_t program, uint32_t shader1,
                                 uint32_t shader2)
    : program_{program}, shader1_{shader1}, shader2_{shader2} {

  auto &gl_functions = gl_loader::instance();
  gl_functions.glAttachShader(program_, shader1_);
  LEVI_CHECK();
  gl_functions.glAttachShader(program_, shader2_);
  LEVI_CHECK();
  gl_functions.glBindAttribLocation(program_, position, "pos");
  LEVI_CHECK();
  gl_functions.glBindAttribLocation(program_, texture_position, "tex_pos");
  LEVI_CHECK();

  gl_functions.glLinkProgram(program_);
  LEVI_CHECK();
  GLint link_status{};
  gl_functions.glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
  LEVI_CHECK();
  if (!link_status) {
    GLint lenght;
    gl_functions.glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &lenght);
    std::string log;
    log.resize(lenght);
    gl_functions.glGetProgramInfoLog(program_, log.size(), nullptr, &log[0]);
    throw std::runtime_error{"shader program couldn't be linked:\n" + log};
  }
  gl_functions.glValidateProgram(program_);
  GLint validate_status{};
  gl_functions.glGetProgramiv(program_, GL_VALIDATE_STATUS, &validate_status);
  if (!validate_status) {
    GLint lenght;
    gl_functions.glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &lenght);
    std::string log;
    log.resize(lenght);
    gl_functions.glGetProgramInfoLog(program_, log.size(), nullptr, &log[0]);
    throw std::runtime_error{"shader program couldn't be validate:\n" + log};
  }
  gl_functions.glUseProgram(program_);
  LEVI_CHECK();
};

shader_attacher::~shader_attacher() {
  auto &gl_functions = gl_loader::instance();
  gl_functions.glDetachShader(program_, shader1_);
  gl_functions.glDetachShader(program_, shader2_);
}

std::string read_shader_code_from_file(const std::string &file) {
  std::ifstream fin;
  fin.open(file);
  if (fin.is_open()) {
    fin.seekg(0, std::ios_base::end);
    auto size = fin.tellg();
    fin.seekg(0, std::ios_base::beg);
    std::string retval;
    retval.resize(size);
    fin.read(&retval[0], retval.size());
    fin.close();
    return retval;
  } else {
    return "";
  }
}

uint32_t create_shader(uint32_t type, const std::string &shader_code) {
  const char *code{nullptr};
  auto &gl_functions = levi::gl_loader::instance();
  auto shader = gl_functions.glCreateShader(type);
  LEVI_CHECK();
  code = &shader_code[0];
  gl_functions.glShaderSource(shader, 1, &code, nullptr);
  LEVI_CHECK();
  gl_functions.glCompileShader(shader);
  LEVI_CHECK();
  GLint compile_status{};
  gl_functions.glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLint length_of_info{};
    gl_functions.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length_of_info);
    std::string info_log{};
    info_log.resize(length_of_info);
    gl_functions.glGetShaderInfoLog(shader, length_of_info, nullptr,
                                    &info_log[0]);
    gl_functions.glDeleteShader(shader);
    throw std::runtime_error{"error while compile shader:\n" + info_log};
  }
  return shader;
}
} // namespace levi
