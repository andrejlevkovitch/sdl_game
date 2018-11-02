// gl_loader.cpp

#include "gl_loader.hpp"
#include <SDL2/SDL.h>
#include <string>

levi::gl_loader &levi::gl_loader::instance() {
  static gl_loader retval{};
  return retval;
}

template <typename T>
void load_gl_func(const std::string &func_name, T &pointer) {
  pointer = reinterpret_cast<T>(::SDL_GL_GetProcAddress(func_name.c_str()));
}

levi::gl_loader::gl_loader() {
  ::load_gl_func("glGenBuffers", glGenBuffers);
  ::load_gl_func("glBindBuffer", glBindBuffer);
  ::load_gl_func("glBufferData", glBufferData);
  ::load_gl_func("glDeleteBuffers", glDeleteBuffers);

  ::load_gl_func("glCreateShader", glCreateShader);
  ::load_gl_func("glShaderSource", glShaderSource);
  ::load_gl_func("glCompileShader", glCompileShader);
  ::load_gl_func("glGetShaderiv", glGetShaderiv);
  ::load_gl_func("glGetShaderInfoLog", glGetShaderInfoLog);
  ::load_gl_func("glAttachShader", glAttachShader);
  ::load_gl_func("glDetachShader", glDetachShader);
  ::load_gl_func("glDeleteShader", glDeleteShader);

  ::load_gl_func("glCreateProgram", glCreateProgram);
  ::load_gl_func("glLinkProgram", glLinkProgram);
  ::load_gl_func("glGetProgramiv", glGetProgramiv);
  ::load_gl_func("glUseProgram", glUseProgram);
  ::load_gl_func("glValidateProgram", glValidateProgram);
  ::load_gl_func("glDeleteProgram", glDeleteProgram);
  ::load_gl_func("glGetProgramInfoLog", glGetProgramInfoLog);

  ::load_gl_func("glGetAttribLocation", glGetAttribLocation);
  ::load_gl_func("glBindAttribLocation", glBindAttribLocation);
  ::load_gl_func("glVertexAttribPointer", glVertexAttribPointer);
  ::load_gl_func("glEnableVertexAttribArray", glEnableVertexAttribArray);
  ::load_gl_func("glDisableVertexAttribArray", glDisableVertexAttribArray);

  ::load_gl_func("glGetUniformLocation", glGetUniformLocation);
  ::load_gl_func("glUniform2f", glUniform2f);
  ::load_gl_func("glUniform1f", glUniform1f);
}
