#version 130

attribute vec2 pos;
attribute vec2 tex_pos;

varying vec2 v_tex_pos;

void main() {
  gl_Position = vec4(pos, 0, 1);
  v_tex_pos = tex_pos;
}
