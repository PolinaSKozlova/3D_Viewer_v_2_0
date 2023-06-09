#include "s21_matrix4X4.h"

namespace s21 {
static double DegreesToRadian(double degrees) { return degrees * (M_PI / 180); }

void Matrix4X4::MakeMovement() {
  InitializeXRotationMatrix();
  InitializeYRotationMatrix();
  InitializeZRotationMatrix();
  InitializeUserScalerMatrix();
  InitializeModelScalerMatrix();
  InitializeShiftMatrix();
}

void Matrix4X4::InitializeXRotationMatrix() {
  data_.x_rotation_deg = DegreesToRadian(data_.x_rotation_deg);
  Matrix4X4 x_rotation_matrix;
  x_rotation_matrix.InitializeIdentityMatrix();
  x_rotation_matrix.matrix_[1][1] = cos(data_.x_rotation_deg);
  x_rotation_matrix.matrix_[1][2] = -sin(data_.x_rotation_deg);
  x_rotation_matrix.matrix_[2][1] = sin(data_.x_rotation_deg);
  x_rotation_matrix.matrix_[2][2] = cos(data_.x_rotation_deg);
  *this *= x_rotation_matrix;
}

void Matrix4X4::InitializeYRotationMatrix() {
  data_.y_rotation_deg = DegreesToRadian(data_.y_rotation_deg);
  Matrix4X4 y_rotation_matrix;
  y_rotation_matrix.InitializeIdentityMatrix();
  y_rotation_matrix.matrix_[0][0] = cos(data_.y_rotation_deg);
  y_rotation_matrix.matrix_[0][2] = sin(data_.y_rotation_deg);
  y_rotation_matrix.matrix_[2][0] = -sin(data_.y_rotation_deg);
  y_rotation_matrix.matrix_[2][2] = cos(data_.y_rotation_deg);
  MulMatrix(y_rotation_matrix);
}

void Matrix4X4::InitializeZRotationMatrix() {
  data_.z_rotation_deg = DegreesToRadian(data_.z_rotation_deg);
  Matrix4X4 z_rotation_matrix;
  z_rotation_matrix.InitializeIdentityMatrix();
  z_rotation_matrix.matrix_[0][0] = cos(data_.z_rotation_deg);
  z_rotation_matrix.matrix_[0][1] = -sin(data_.z_rotation_deg);
  z_rotation_matrix.matrix_[1][0] = sin(data_.z_rotation_deg);
  z_rotation_matrix.matrix_[1][1] = cos(data_.z_rotation_deg);
  MulMatrix(z_rotation_matrix);
}

void Matrix4X4::InitializeUserScalerMatrix() {
  if (data_.user_scaler) MulNumber(data_.user_scaler);
  matrix_[3][3] = 1;
}

void Matrix4X4::InitializeModelScalerMatrix() { matrix_[2][3] += -5; }

void Matrix4X4::InitializeShiftMatrix() {
  matrix_[0][3] += data_.x_shift;
  matrix_[1][3] += data_.y_shift;
  matrix_[2][3] += data_.z_shift;
}
};  //  namespace s21