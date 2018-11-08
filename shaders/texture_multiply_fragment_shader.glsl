#version 130

uniform sampler2D tex;

varying vec2 v_tex_pos;

void main() {
  gl_FragColor = texture2D(tex, v_tex_pos);
  if (gl_FragColor.a == 0) discard;
}
