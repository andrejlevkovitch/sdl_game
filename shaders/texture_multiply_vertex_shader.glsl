#version 130

uniform vec2 win_size;
uniform vec2 tex_size;

attribute vec3 pos;
attribute vec2 tex_pos;

varying vec2 v_tex_pos;

void main() {
  mat4 to_ndc = mat4(2.0 / win_size.x, 0, 0, 0,
                     0, -2.0 / win_size.y, 0, 0,
                     0, 0, 1, 0,
                     -1, 1, 0, 1);

  gl_Position = to_ndc * vec4(pos, 1);

  mat2 tex_to_ndc = mat2(1.0 / tex_size.x, 0,
                         0, 1.0 / tex_size.y);

  v_tex_pos = tex_to_ndc * tex_pos;
}
