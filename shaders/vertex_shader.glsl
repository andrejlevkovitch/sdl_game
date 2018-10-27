attribute vec3 pos;
attribute vec2 tex_pos;
uniform vec2 center_of_rotation;
uniform float radian_angle;
uniform vec2 win_size;
uniform vec2 tex_size;

varying vec2 v_tex_pos;

void main() {
  mat3 transfer;
  transfer[0] = vec3(1, 0, 0);
  transfer[1] = vec3(0, 1, 0);
  transfer[2] = vec3(-center_of_rotation.x, -center_of_rotation.y, 1);

  mat3 rotade;
  float sin_angle = sin(radian_angle);
  float cos_angle = cos(radian_angle);
  rotade[0] = vec3(cos_angle, sin_angle, 0);
  rotade[1] = vec3(-sin_angle, cos_angle, 0);
  rotade[2] = vec3(0, 0, 1);

  mat3 to_ndc;
  to_ndc[0] = vec3(2.0 / win_size.x, 0, 0);
  to_ndc[1] = vec3(0, -2.0 / win_size.y, 0);
  to_ndc[2] = vec3(-1, 1, 1);

  mat3 rezult = rotade * transfer;
  transfer[2][0] = center_of_rotation.x;
  transfer[2][1] = center_of_rotation.y;
  rezult = transfer * rezult;
  rezult = to_ndc * rezult;

  gl_Position = vec4(rezult * pos, 1);

  mat2 tex_to_ndc;
  tex_to_ndc[0] = vec2(1.0 / tex_size.x, 0);
  tex_to_ndc[1] = vec2(0, 1.0 / tex_size.y);

  v_tex_pos = tex_to_ndc * tex_pos;
}
