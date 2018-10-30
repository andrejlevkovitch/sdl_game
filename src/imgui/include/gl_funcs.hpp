// gl_funcs.hpp

/* load all neded funcs by SDL2*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

::PFNGLGENBUFFERSPROC glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(
    ::SDL_GL_GetProcAddress("glGenBuffers"));
::PFNGLBINDBUFFERPROC glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(
    ::SDL_GL_GetProcAddress("glBindBuffers"));
::PFNGLBUFFERDATAPROC glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(
    ::SDL_GL_GetProcAddress("glBufferData"));
::PFNGLDELETEBUFFERSPROC glDeleteBuffers =
    reinterpret_cast<PFNGLDELETEBUFFERSPROC>(
        ::SDL_GL_GetProcAddress("glDeleteBuffers"));

::PFNGLCREATESHADERPROC glCreateShader =
    reinterpret_cast<PFNGLCREATESHADERPROC>(
        ::SDL_GL_GetProcAddress("glCreateShader"));
::PFNGLSHADERSOURCEPROC glShaderSource =
    reinterpret_cast<PFNGLSHADERSOURCEPROC>(
        ::SDL_GL_GetProcAddress("glShaderSource"));
::PFNGLCOMPILESHADERPROC glCompileShader =
    reinterpret_cast<PFNGLCOMPILESHADERPROC>(
        ::SDL_GL_GetProcAddress("glCompileShader"));
::PFNGLGETSHADERIVPROC glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(
    ::SDL_GL_GetProcAddress("glGetShaderiv"));
::PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog =
    reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(
        ::SDL_GL_GetProcAddress("glGetShaderInfoLog"));
::PFNGLATTACHSHADERPROC glAttachShader =
    reinterpret_cast<PFNGLATTACHSHADERPROC>(
        ::SDL_GL_GetProcAddress("glAttachShader"));
::PFNGLDETACHSHADERPROC glDetachShader =
    reinterpret_cast<PFNGLDETACHSHADERPROC>(
        ::SDL_GL_GetProcAddress("glDetachShader"));
::PFNGLDELETESHADERPROC glDeleteShader =
    reinterpret_cast<PFNGLDELETESHADERPROC>(
        ::SDL_GL_GetProcAddress("glDeleteShader"));

::PFNGLCREATEPROGRAMPROC glCreateProgram =
    reinterpret_cast<PFNGLCREATEPROGRAMPROC>(
        ::SDL_GL_GetProcAddress("glCreateProgram"));
::PFNGLLINKPROGRAMPROC glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(
    ::SDL_GL_GetProcAddress("glLinkProgram"));
::PFNGLGETPROGRAMIVPROC glGetProgramiv =
    reinterpret_cast<PFNGLGETPROGRAMIVPROC>(
        ::SDL_GL_GetProcAddress("glGetProgramiv"));
::PFNGLUSEPROGRAMPROC glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(
    ::SDL_GL_GetProcAddress("glUseProgram"));
::PFNGLVALIDATEPROGRAMPROC glValidateProgram =
    reinterpret_cast<PFNGLVALIDATEPROGRAMPROC>(
        ::SDL_GL_GetProcAddress("glValidateProgram"));
::PFNGLDELETEPROGRAMPROC glDeleteProgram =
    reinterpret_cast<PFNGLDELETEPROGRAMPROC>(
        ::SDL_GL_GetProcAddress("glDeleteProgram"));
::PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog =
    reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(
        ::SDL_GL_GetProcAddress("glGetProgramInfoLog"));

::PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation =
    reinterpret_cast<PFNGLGETATTRIBLOCATIONPROC>(
        ::SDL_GL_GetProcAddress("glGetAttribLocation"));
::PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer =
    reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(
        ::SDL_GL_GetProcAddress("glVertexAttribPointer"));
::PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray =
    reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(
        ::SDL_GL_GetProcAddress("glEnableVertexAttribArray"));
::PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray =
    reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(
        ::SDL_GL_GetProcAddress("glDisableVertexAttribArray"));

::PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation =
    reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(
        ::SDL_GL_GetProcAddress("glGetUniformLocation"));
::PFNGLUNIFORM2FPROC glUniform2f = reinterpret_cast<PFNGLUNIFORM2FPROC>(
    ::SDL_GL_GetProcAddress("glUniform2f"));
::PFNGLUNIFORM1FPROC glUniform1f = reinterpret_cast<PFNGLUNIFORM1FPROC>(
    ::SDL_GL_GetProcAddress("glUniform1f"));

::PFNGLUNIFORM1IPROC glUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>(
    ::SDL_GL_GetProcAddress("glUniform1i"));
::PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv =
    reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(
        ::SDL_GL_GetProcAddress("glUniformMatrix4fv"));

::PFNGLBINDSAMPLERPROC glBindSampler = reinterpret_cast<PFNGLBINDSAMPLERPROC>(
    ::SDL_GL_GetProcAddress("glBindSampler"));

::PFNGLGENVERTEXARRAYSPROC glGenVertexArrays =
    reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(
        ::SDL_GL_GetProcAddress("glGenVertexArrays"));
::PFNGLBINDVERTEXARRAYPROC glBindVertexArray =
    reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(
        ::SDL_GL_GetProcAddress("glBindVertexArray"));
::PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays =
    reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(
        ::SDL_GL_GetProcAddress("glDeleteVertexArrays"));

::PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate =
    reinterpret_cast<PFNGLBLENDEQUATIONSEPARATEPROC>(
        ::SDL_GL_GetProcAddress("glBlendEquationSeparate"));
::PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate =
    reinterpret_cast<PFNGLBLENDFUNCSEPARATEPROC>(
        ::SDL_GL_GetProcAddress("glBlendFuncSeparate"));
