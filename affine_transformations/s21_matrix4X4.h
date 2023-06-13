#ifndef THREE_D_VIEWER_S21_MATRIX_H_
#define THREE_D_VIEWER_S21_MATRIX_H_

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace s21 {
struct TransformData {
  TransformData(double x_rotation = 0, double y_rotation = 0,
                double z_rotation = 0, double shift_x = 0, double shift_y = 0,
                double shift_z = 0, double scaler_user = 0,
                double scaler_model = 0, bool perspective = false)
      : x_rotation_deg(x_rotation),
        y_rotation_deg(y_rotation),
        z_rotation_deg(z_rotation),
        x_shift(shift_x),
        y_shift(shift_y),
        z_shift(shift_z),
        user_scaler(scaler_user),
        model_scaler(scaler_model),
        perspective_ortho(perspective) {}
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

class Matrix4X4 {
 public:
  Matrix4X4();
  // explicit Matrix4X4(int size);
  Matrix4X4(const Matrix4X4& other);
  ~Matrix4X4();
  Matrix4X4& operator=(const Matrix4X4& other) noexcept;
  Matrix4X4& operator*=(const Matrix4X4& other) noexcept;
  Matrix4X4& operator*=(const double& num) noexcept;
  void MakeMovement(const TransformData& other);
  void MulMatrix(const Matrix4X4& other);
  void MulNumber(const double& num) noexcept;
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

 private:
  void MemoryAllocate();
  void MemoryFree();
  void InitializeIdentityMatrix();
  void InitializeXRotationMatrix();
  void InitializeYRotationMatrix();
  void InitializeZRotationMatrix();
  void InitializeUserScalerMatrix();
  void InitializeModelScalerMatrix();
  void InitializeShiftMatrix();
  // void SetNullMatrix();
  int rows_, cols_;
  float** matrix_;
  TransformData data_;
};
};  // namespace s21

#endif  // THREE_D_VIEWER_S21_MATRIX_H_