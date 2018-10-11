// engine.cpp

#include "engine.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "event.hpp"
#include "vertex.hpp"

#define LEVI_CHECK()                                                           \
  {                                                                            \
    auto error = ::glGetError();                                               \
    if (error != GL_NO_ERROR) {                                                \
      switch (error) {                                                         \
      case GL_INVALID_ENUM:                                                    \
        throw std::runtime_error{"gl_error: invalid enum"};                    \
      case GL_INVALID_VALUE:                                                   \
        throw std::runtime_error{"gl_error: invalid value"};                   \
      case GL_INVALID_OPERATION:                                               \
        throw std::runtime_error{"gl_error: invalid operation"};               \
      case GL_INVALID_FRAMEBUFFER_OPERATION:                                   \
        throw std::runtime_error{"gl_error: invalid framebuffer operation"};   \
      case GL_OUT_OF_MEMORY:                                                   \
        throw std::runtime_error{"gl_error: out of memory"};                   \
      default:                                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  };

::PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
::PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
::PFNGLBUFFERDATAPROC glBufferData = nullptr;
::PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;

::PFNGLCREATESHADERPROC glCreateShader = nullptr;
::PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
::PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
::PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
::PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
::PFNGLATTACHSHADERPROC glAttachShader = nullptr;
::PFNGLDETACHSHADERPROC glDetachShader = nullptr;
::PFNGLDELETESHADERPROC glDeleteShader = nullptr;

::PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
::PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
::PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
::PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
::PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
::PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;

::PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
::PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
::PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;

::PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
::PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
::PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;

template <typename T>
void load_gl_func(const std::string &func_name, T *pointer) {
  *pointer = reinterpret_cast<T>(::SDL_GL_GetProcAddress(func_name.c_str()));
}

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
  ::glClearColor(0, 0, 0, 0);

  ::load_gl_func("glGenBuffers", &glGenBuffers);
  ::load_gl_func("glBindBuffer", &glBindBuffer);
  ::load_gl_func("glBufferData", &glBufferData);
  ::load_gl_func("glDeleteBuffers", &glDeleteBuffers);

  ::load_gl_func("glCreateShader", &glCreateShader);
  ::load_gl_func("glShaderSource", &glShaderSource);
  ::load_gl_func("glCompileShader", &glCompileShader);
  ::load_gl_func("glGetShaderiv", &glGetShaderiv);
  ::load_gl_func("glGetShaderInfoLog", &glGetShaderInfoLog);
  ::load_gl_func("glAttachShader", &glAttachShader);
  ::load_gl_func("glDetachShader", &glDetachShader);
  ::load_gl_func("glDeleteShader", &glDeleteShader);

  ::load_gl_func("glCreateProgram", &glCreateProgram);
  ::load_gl_func("glLinkProgram", &glLinkProgram);
  ::load_gl_func("glGetProgramiv", &glGetProgramiv);
  ::load_gl_func("glUseProgram", &glUseProgram);
  ::load_gl_func("glValidateProgram", &glValidateProgram);
  ::load_gl_func("glDeleteProgram", &glDeleteProgram);

  ::load_gl_func("glGetAttribLocation", &glGetAttribLocation);
  ::load_gl_func("glVertexAttribPointer", &glVertexAttribPointer);
  ::load_gl_func("glEnableVertexAttribArray", &glEnableVertexAttribArray);

  ::load_gl_func("glGenVertexArrays", &glGenVertexArrays);
  ::load_gl_func("glBindVertexArray", &glBindVertexArray);
  ::load_gl_func("glDeleteVertexArrays", &glDeleteVertexArrays);

  const char *vertex_shader_code = R"(
   attribute vec2 position;
   attribute vec3 color;
   varying vec3 out_color;
   void main() {
     gl_Position = vec4(position, 0, 1);
     out_color = color;
   }
 )";

  v_shader_ = glCreateShader(GL_VERTEX_SHADER);
  LEVI_CHECK();
  glShaderSource(v_shader_, 1, &vertex_shader_code, nullptr);
  LEVI_CHECK();
  glCompileShader(v_shader_);
  LEVI_CHECK();
  GLint compile_status{};
  glGetShaderiv(v_shader_, GL_COMPILE_STATUS, &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLchar log[512];
    glGetShaderInfoLog(v_shader_, 512, nullptr, log);
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

  f_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
  LEVI_CHECK();
  glShaderSource(f_shader_, 1, &fragment_shader_code, nullptr);
  LEVI_CHECK();
  glCompileShader(f_shader_);
  LEVI_CHECK();
  glGetShaderiv(f_shader_, GL_COMPILE_STATUS, &compile_status);
  LEVI_CHECK();
  if (!compile_status) {
    GLchar log[512];
    glGetShaderInfoLog(f_shader_, 512, nullptr, log);
    throw std::runtime_error{
        std::string{"bad compile status for fragment_shader\n"} +
        reinterpret_cast<const char *>(log)};
  }

  program_ = glCreateProgram();
  LEVI_CHECK();
  if (!program_) {
    throw std::runtime_error{"program not created"};
  }
  glAttachShader(program_, v_shader_);
  LEVI_CHECK();
  glAttachShader(program_, f_shader_);
  LEVI_CHECK();

  glLinkProgram(program_);
  LEVI_CHECK();
  GLint link_status{};
  glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
  LEVI_CHECK();
  if (!link_status) {
    throw std::runtime_error{"program not linked"};
  }
  glUseProgram(program_);
  LEVI_CHECK();
}

levi::engine::~engine() {
  glDetachShader(program_, f_shader_);
  glDetachShader(program_, v_shader_);
  glDeleteShader(f_shader_);
  glDeleteShader(v_shader_);
  glDeleteProgram(program_);

  ::SDL_GL_DeleteContext(gl_context_);
  ::SDL_DestroyWindow(window_);
  ::SDL_Quit();
}

void levi::engine::update() {}

void levi::engine::render() {
  ::glClear(GL_COLOR_BUFFER_BIT);

  vertex vertices[]{vertex{.0f, .5f, 1.0f, .0f, .0f},
                    vertex{-.5f, -.5f, .0f, 1.0f, .0f},
                    vertex{.5, -.5, .0f, .0f, 1.0f}};
  GLuint vbo{};
  glGenBuffers(1, &vbo);
  LEVI_CHECK();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  LEVI_CHECK();
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  LEVI_CHECK();

  GLuint vao{};
  glGenVertexArrays(1, &vao);
  LEVI_CHECK();
  glBindVertexArray(vao);
  LEVI_CHECK();

  auto pos_attribute = glGetAttribLocation(program_, "position");
  LEVI_CHECK();
  if (pos_attribute < 0) {
    throw std::runtime_error{"undefinded attribute"};
  }
  glVertexAttribPointer(pos_attribute, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        0);
  LEVI_CHECK();
  glEnableVertexAttribArray(pos_attribute);
  LEVI_CHECK();

  auto color_attribute = glGetAttribLocation(program_, "color");
  LEVI_CHECK();
  if (pos_attribute < 0) {
    throw std::runtime_error{"undefinded attribute"};
  }
  glVertexAttribPointer(color_attribute, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        reinterpret_cast<void *>(sizeof(float) * 2));
  LEVI_CHECK();
  glEnableVertexAttribArray(color_attribute);
  LEVI_CHECK();

  ::glDrawArrays(GL_TRIANGLES, 0, 3);
  LEVI_CHECK();

  ::SDL_GL_SwapWindow(window_);

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  LEVI_CHECK();
}

levi::size levi::engine::get_window_size() const { return win_size_; }
