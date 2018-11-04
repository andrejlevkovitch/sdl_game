#version 130

varying vec2 v_tex_pos;
uniform sampler2D tex;
void main() {
  gl_FragColor = texture2D(tex, v_tex_pos);
}
