// texture.cpp

#include "texture.hpp"

levi::texture::texture() : gl_tex{}, width{}, height{} {}

levi::texture::texture(uint32_t gl_tex_, size_t width_, size_t height_)
    : gl_tex{gl_tex_}, width{width_}, height{height_} {};
