// gl_loader.hpp

#pragma once

#include <SDL2/SDL_opengl.h>
#include <string>

#include <sstream>
#define LEVI_CHECK()                                                           \
  {                                                                            \
    auto some_error = ::glGetError();                                          \
    if (some_error != GL_NO_ERROR) {                                           \
      std::string error;                                                       \
      switch (some_error) {                                                    \
      case GL_INVALID_ENUM:                                                    \
        error = "gl_error: invalid enum\n";                                    \
      case GL_INVALID_VALUE:                                                   \
        error = "gl_error: invalid value\n";                                   \
      case GL_INVALID_OPERATION:                                               \
        error = "gl_error: invalid operation\n";                               \
      case GL_INVALID_FRAMEBUFFER_OPERATION:                                   \
        error = "gl_error: invalid framebuffer operation\n";                   \
      case GL_OUT_OF_MEMORY:                                                   \
        error = "gl_error: out of memory\n";                                   \
      default:                                                                 \
        break;                                                                 \
      }                                                                        \
      std::stringstream ss;                                                    \
      const int max_len{512};                                                  \
      char buf[max_len]{};                                                     \
      ss << __FILE__ << ':' << __LINE__ << ' ' << __FUNCTION__;                \
      ss.getline(buf, max_len);                                                \
      error += buf;                                                            \
      throw std::runtime_error{error};                                         \
    }                                                                          \
  }

namespace levi {
/**\brief this class load all needed gl function. Singleton*/
class gl_loader {
public:
  static gl_loader &instance();

public:
  ::PFNGLGENBUFFERSPROC glGenBuffers;
  ::PFNGLBINDBUFFERPROC glBindBuffer;
  ::PFNGLBUFFERDATAPROC glBufferData;
  ::PFNGLDELETEBUFFERSPROC glDeleteBuffers;

  ::PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
  ::PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
  ::PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
  ::PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
  ::PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
  ::PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

  ::PFNGLGENRENDERBUFFERSPROC glGenRenderBuffers;
  ::PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
  ::PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
  ::PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;

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
  ::PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
  ::PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;

  ::PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
  ::PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
  ::PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
  ::PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
  ::PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

  ::PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
  ::PFNGLUNIFORM1FPROC glUniform1f;
  ::PFNGLUNIFORM2FPROC glUniform2f;
  ::PFNGLUNIFORM3FPROC glUniform3f;
  ::PFNGLUNIFORM1IPROC glUniform1i;

  ::PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;

  ::PFNGLACTIVETEXTUREPROC glActiveTexture;

private:
  gl_loader();
  gl_loader(const gl_loader &) = delete;
  gl_loader &operator=(const gl_loader &) = delete;
};
}; // namespace levi
