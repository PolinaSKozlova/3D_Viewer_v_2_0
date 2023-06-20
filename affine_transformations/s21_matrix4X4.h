#ifndef THREE_D_VIEWER_S21_MATRIX_H_
#define THREE_D_VIEWER_S21_MATRIX_H_

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace s21 {
struct TransformData {
  TransformData(float x_rotation = 0, float y_rotation = 0,
                float z_rotation = 0, float shift_x = 0, float shift_y = 0,
                float shift_z = 0, float scaler_user = 0,
                float scaler_model = 0, bool perspective = false)
      : x_rotation_deg(x_rotation),
        y_rotation_deg(y_rotation),
        z_rotation_deg(z_rotation),
        x_shift(shift_x),
        y_shift(shift_y),
        z_shift(shift_z),
        user_scaler(scaler_user),
        model_scaler(scaler_model),
        perspective_ortho(perspective) {}
  float x_rotation_deg;
  float y_rotation_deg;
  float z_rotation_deg;
  float x_shift;
  float y_shift;
  float z_shift;
  float user_scaler;
  float model_scaler;
  bool perspective_ortho;
};

class Matrix4X4 {
 public:
  Matrix4X4();
  Matrix4X4(const Matrix4X4& other);
  ~Matrix4X4();
  Matrix4X4& operator=(const Matrix4X4& other) noexcept;
  void MakeMovement(const TransformData& other);
  float** GetMatrix() const noexcept;
  void SetData(const TransformData& other);
  float* CreateOneRowMatrix() const noexcept;
  /*вывод матрицы*/
  void print() {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        std::cout << matrix_[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }
  void print_transformation() {
    std::cout << data_.x_rotation_deg << std::endl;
    std::cout << data_.y_rotation_deg << std::endl;
    std::cout << data_.z_rotation_deg << std::endl;
  }

 private:
  Matrix4X4& operator*=(const Matrix4X4& other) noexcept;
  Matrix4X4& operator*=(const double& num) noexcept;
  void MulMatrix(const Matrix4X4& other);
  void MulNumber(const double& num) noexcept;
  void MemoryAllocate();
  void MemoryFree();
  void InitializeIdentityMatrix();
  void InitializeXRotationMatrix();
  void InitializeYRotationMatrix();
  void InitializeZRotationMatrix();
  void InitializeUserScalerMatrix();
  void InitializeModelScalerMatrix();
  void InitializeScalerMatrix() noexcept;
  void InitializeShiftMatrix();
  void SetNullMatrix();
  int rows_, cols_;
  float** matrix_;
  TransformData data_{};
};
};  // namespace s21

#endif  // THREE_D_VIEWER_S21_MATRIX_H_