// gl_loader.hpp

#pragma once

#include <string>

#include <SDL2/SDL_opengl.h>

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

namespace levi {
class gl_loader {
public:
  static gl_loader &instance();

public:
  ::PFNGLGENBUFFERSPROC glGenBuffers;
  ::PFNGLBINDBUFFERPROC glBindBuffer;
  ::PFNGLBUFFERDATAPROC glBufferData;
  ::PFNGLDELETEBUFFERSPROC glDeleteBuffers;

  ::PFNGLCREATESHADERPROC glCreateShader;
  ::PFNGLSHADERSOURCEPROC glShaderSource;
  ::PFNGLCOMPILESHADERPROC glCompileShader;
  ::PFNGLGETSHADERIVPROC glGetShaderiv;
  ::PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
  ::PFNGLATTACHSHADERPROC glAttachShader;
  ::PFNGLDETACHSHADERPROC glDetachShader;
  ::PFNGLDELETESHADERPROC glDeleteShader;

  ::PFNGLCREATEPROGRAMPROC glCreateProgram;
  ::PFNGLLINKPROGRAMPROC glLinkProgram;
  ::PFNGLGETPROGRAMIVPROC glGetProgramiv;
  ::PFNGLUSEPROGRAMPROC glUseProgram;
  ::PFNGLVALIDATEPROGRAMPROC glValidateProgram;
  ::PFNGLDELETEPROGRAMPROC glDeleteProgram;

  ::PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
  ::PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
  ::PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

  ::PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
  ::PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
  ::PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

private:
  gl_loader();
  gl_loader(const gl_loader &) = delete;
  gl_loader &operator=(const gl_loader &) = delete;
};
}; // namespace levi
