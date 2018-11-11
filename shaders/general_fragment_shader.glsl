#version 130

uniform sampler2D render_tex;
uniform sampler2D light_tex;

varying vec2 v_tex_pos;

void main() {
  gl_FragColor = texture2D(render_tex, v_tex_pos) * texture2D(light_tex, v_tex_pos);
}
