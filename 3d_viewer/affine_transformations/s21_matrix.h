#ifndef THREE_D_VIEWER_S21_MATRIX_H_
#define THREE_D_VIEWER_S21_MATRIX_H_

#include <array>
#include <iostream>
#include <stdexcept>

namespace s21 {
class Matrix {
 public:
  Matrix();
  explicit Matrix(int size);
  ~Matrix();
  int GetRows() const noexcept;
  int GetCols() const noexcept;
  Matrix operator*(const double& num) noexcept;
  void MulNumber(const double& num) noexcept;
  double& operator()(const int& i, const int& j);
  std::array<Matrix, 6> CreateMatrixArray();

  //  private:
 protected:
  void MemoryAllocate();
  void MemoryFree();
  void InitializeIdentityMatrix();
  int rows_, cols_;
  double** matrix_;
};
};  // namespace s21

#endif  // THREE_D_VIEWER_S21_MATRIX_H_