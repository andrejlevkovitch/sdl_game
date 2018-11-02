attribute vec3 pos;
attribute vec2 tex_pos;
uniform vec2 center_of_rotation;
uniform float radian_angle;
uniform vec2 win_size;
uniform vec2 tex_size;

varying vec2 v_tex_pos;

void main() {
  mat3 transfer = mat3(1, 0, 0,
                       0, 1, 0,
                       -center_of_rotation, 1);

  float sin_angle = sin(radian_angle);
  float cos_angle = cos(radian_angle);
  mat3 rotade = mat3(cos_angle, sin_angle, 0,
                     -sin_angle, cos_angle, 0,
                     0, 0, 1);

  mat3 to_ndc = mat3(2.0 / win_size.x, 0, 0,
                     0, -2.0 / win_size.y, 0,
                     -1, 1, 1);

  mat3 rezult = rotade * transfer;
  transfer[2] = vec3(center_of_rotation, 1);
  rezult = transfer * rezult;
  rezult = to_ndc * rezult;

  gl_Position = vec4(rezult * pos, 1);

  mat2 tex_to_ndc = mat2(1.0 / tex_size.x, 0,
                         0, 1.0 / tex_size.y);

  v_tex_pos = tex_to_ndc * tex_pos;
}
