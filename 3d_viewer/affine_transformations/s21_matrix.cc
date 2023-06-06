#include "s21_matrix.h"

namespace s21 {
Matrix4X4::Matrix4X4() : rows_(4), cols_(4) {
  MemoryAllocate();
  InitializeIdentityMatrix();
}

Matrix4X4::Matrix4X4(int size) : rows_(size), cols_(size) {
  if (size <= 0) throw std::invalid_argument("Size can't be less than zero");
  MemoryAllocate();
  InitializeIdentityMatrix();
}

Matrix4X4::~Matrix4X4() {
  if (matrix_) MemoryFree();
  matrix_ = nullptr;
  rows_ = cols_ = 0;
}

int Matrix4X4::GetRows() const noexcept { return rows_; }

int Matrix4X4::GetCols() const noexcept { return cols_; }

Matrix4X4 Matrix4X4::operator*(const double& num) noexcept {
  MulNumber(num);
  return *this;
}

void Matrix4X4::MulNumber(const double& num) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

double& Matrix4X4::operator()(const int& i, const int& j) {
  if (i > rows_ || i < 0 || j > cols_ || j < 0) {
    throw std::invalid_argument("Out of range");
  } else {
    return matrix_[i][j];
  }
}

void Matrix4X4::MemoryAllocate() {
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_]{};
  }
}

void Matrix4X4::MemoryFree() {
  for (int i = 0; i < rows_; ++i) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}

void Matrix4X4::InitializeIdentityMatrix() {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (i == j) matrix_[i][j] = 1;
    }
  }
}
};  // namespace s21
