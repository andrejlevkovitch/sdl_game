#version 130

attribute vec4 pos;
attribute vec2 tex_pos;
uniform vec2 center_of_rotation;
uniform float radian_angle;
uniform vec2 win_size;
uniform vec2 tex_size;

varying vec2 v_tex_pos;

void main() {
  mat4 transfer = mat4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       -center_of_rotation, 0, 1);

  float sin_angle = sin(radian_angle);
  float cos_angle = cos(radian_angle);
  mat4 rotade = mat4(cos_angle, sin_angle, 0, 0,
                     -sin_angle, cos_angle, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1);

  mat4 to_ndc = mat4(2.0 / win_size.x, 0, 0, 0,
                     0, -2.0 / win_size.y, 0, 0,
                     0, 0, 1, 0,
                     -1, 1, 0, 1);

  mat4 rezult = rotade * transfer;
  transfer[3] = vec4(center_of_rotation, 0, 1);
  rezult = transfer * rezult;
  rezult = to_ndc * rezult;

  gl_Position = rezult * pos;

  mat2 tex_to_ndc = mat2(1.0 / tex_size.x, 0,
                         0, 1.0 / tex_size.y);

  v_tex_pos = tex_to_ndc * tex_pos;
}
