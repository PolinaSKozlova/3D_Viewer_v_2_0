#include "s21_matrix.h"

namespace s21 {
Matrix::Matrix() : rows_(4), cols_(4) { MemoryAllocate(); }

Matrix::Matrix(int size) : rows_(size), cols_(size) {
  if (size <= 0) throw std::invalid_argument("Size can't be less than zero");
  MemoryAllocate();
}

Matrix::~Matrix() {
  if (matrix_) MemoryFree();
  matrix_ = nullptr;
  rows_ = cols_ = 0;
}

int Matrix::GetRows() const noexcept { return rows_; }

int Matrix::GetCols() const noexcept { return cols_; }

Matrix Matrix::operator*(const double& num) noexcept {
  MulNumber(num);
  return *this;
}

void Matrix::MulNumber(const double& num) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

double& Matrix::operator()(const int& i, const int& j) {
  if (i > rows_ || i < 0 || j > cols_ || j < 0) {
    throw std::invalid_argument("Out of range");
  } else {
    return matrix_[i][j];
  }
}

std::array<Matrix, 6> Matrix::CreateMatrixArray() {
  std::array<Matrix, 6> result_array;
  // static int num = 0;
  for (int i = 0; i < 6; ++i) {
    result_array[i].MemoryAllocate();
    result_array[i].InitializeIdentityMatrix();
    // std::cout << "create matrix array " << num++ << std::endl;
    // for (int j = 0; j < 4; ++j) {
    //   for (int k = 0; k < 4; ++k) {
    //     std::cout << result_array[i].matrix_[j][k] << " ";
    //   }
    //   std::cout << std::endl;
    // }
  }
  return result_array;
}

void Matrix::MemoryAllocate() {
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_]{};
  }
}

void Matrix::MemoryFree() {
  for (int i = 0; i < rows_; ++i) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}

void Matrix::InitializeIdentityMatrix() {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (i == j) matrix_[i][j] = 1;
    }
  }
}
};  // namespace s21
