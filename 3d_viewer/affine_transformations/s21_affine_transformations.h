#ifndef THREE_D_VIEWER_S21_AFFINE_TRANSFORMATIONS_H_
#define THREE_D_VIEWER_S21_AFFINE_TRANSFORMATIONS_H_

#include "../parser/s21_parser.h"
#include "s21_matrix.h"

namespace s21 {
struct TransformData {
  // Углы поворота
  double x_rotation_deg;
  double y_rotation_deg;
  double z_rotation_deg;
  // Смещения вдоль осей
  double x_shift;
  double y_shift;
  double z_shift;
  // Коэффициент масштабирования
  double user_scaler;
  double model_scaler;
  bool perspective_ortho;
};

class AffineTransformations : public Matrix {
 public:
  AffineTransformations();
  ~AffineTransformations();
  void SetTransformData(const TransformData& data) noexcept;
  void MakeMovement();
  void print() {
    for (auto& it : transform_matrix_) {
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
          std::cout << it(i, j) << " ";
        }
        std::cout << std::endl;
      }
    }
  }

 private:
  void InitializeTransformMatrix();
  void InitializeXRotationMatrix();
  void InitializeYRotationMatrix();
  void InitializeZRotationMatrix();
  std::array<Matrix, 6> transform_matrix_{};
  TransformData transform_data_;
};
};  // namespace s21

#endif  // THREE_D_VIEWER_S21_AFFINE_TRANSFORMATIONS_H_