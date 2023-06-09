#include "affine_transformations/s21_matrix4X4.h"

int main() {
  s21::Matrix4X4 m(4);
  s21::TransformData transformations;
  transformations.x_rotation_deg = -12;
  transformations.y_rotation_deg = 34;
  transformations.user_scaler = 0;
  transformations.z_shift = 20;
  m.SetData(transformations);
  m.MakeMovement();
  m.print();
  transformations.x_rotation_deg = 0;
  transformations.x_shift = -18;
  transformations.z_rotation_deg = 10;
  m.SetData(transformations);
  m.MakeMovement();
  m.print();
  return 0;
}
