#version 130

uniform sampler2D tex;
uniform sampler2D light;

varying vec2 v_tex_pos;
varying vec2 light_tex_pos;

void main() {
  gl_FragColor = texture2D(tex, v_tex_pos) * texture2D(light, light_tex_pos);
  if (gl_FragColor.a == 0) discard;
}
