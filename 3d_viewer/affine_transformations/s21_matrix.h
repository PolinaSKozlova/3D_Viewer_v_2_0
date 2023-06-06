#ifndef THREE_D_VIEWER_S21_MATRIX_H_
#define THREE_D_VIEWER_S21_MATRIX_H_

#include <array>
#include <iostream>
#include <stdexcept>

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

class Matrix4X4 {
 public:
  Matrix4X4();
  explicit Matrix4X4(int size);
  ~Matrix4X4();
  int GetRows() const noexcept;
  int GetCols() const noexcept;
  Matrix4X4 operator*(const double& num) noexcept;
  void MulNumber(const double& num) noexcept;
  double& operator()(const int& i, const int& j);

 private:
  //  protected:
  void MemoryAllocate();
  void MemoryFree();
  void InitializeIdentityMatrix();
  int rows_, cols_;
  double** matrix_;
};
};  // namespace s21

#endif  // THREE_D_VIEWER_S21_MATRIX_H_