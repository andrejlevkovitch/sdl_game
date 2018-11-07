#version 130

uniform sampler2D tex;
uniform sampler2D light;
uniform vec3 color;

varying vec2 v_tex_pos;
varying vec2 global_pos;

void main() {
  gl_FragColor = texture2D(tex, v_tex_pos) * vec4(color, 1) * texture2D(light, global_pos);
//vec4 alfa = texture2D(light, global_pos);
  if (gl_FragColor.a == 0) discard;
}
