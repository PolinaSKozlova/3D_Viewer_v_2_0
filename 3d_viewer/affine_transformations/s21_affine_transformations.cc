#include "s21_affine_transformations.h"

#include <cmath>

namespace s21 {
// static double DegreesToRadian(double degrees) { return degrees * (M_PI /
// 180); }

AffineTransformations::AffineTransformations() {}

AffineTransformations::~AffineTransformations() {}

void AffineTransformations::SetTransformData(
    const TransformData& data) noexcept {
  transform_data_.x_rotation_deg = data.x_rotation_deg;
  transform_data_.y_rotation_deg = data.y_rotation_deg;
  transform_data_.z_rotation_deg = data.z_rotation_deg;
  transform_data_.x_shift = data.x_shift;
  transform_data_.y_shift = data.y_shift;
  transform_data_.z_shift = data.z_shift;
  transform_data_.user_scaler = data.user_scaler;
  transform_data_.model_scaler = data.model_scaler;
  transform_data_.perspective_ortho = data.perspective_ortho;
}

void AffineTransformations::MakeMovement() {
  // InitializeTransformMatrix();
  transform_matrix_ = CreateMatrixArray();
  print();
}

void AffineTransformations::InitializeTransformMatrix() {}

void AffineTransformations::InitializeXRotationMatrix() {}

void AffineTransformations::InitializeYRotationMatrix() {}

void AffineTransformations::InitializeZRotationMatrix() {}

};  // namespace s21