#version 130

uniform sampler2D tex;
uniform vec3 color;

varying vec2 v_tex_pos;

void main() {
  gl_FragColor = texture2D(tex, v_tex_pos) * vec4(color, 1);
  if (gl_FragColor.a == 0) discard;
}
