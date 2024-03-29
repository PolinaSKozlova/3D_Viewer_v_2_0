#include "s21_matrix4X4.h"

namespace s21 {
Matrix4X4::Matrix4X4() : rows_(4), cols_(4) {
  MemoryAllocate();
  // InitializeIdentityMatrix();
}

Matrix4X4::Matrix4X4(int size) : rows_(size), cols_(size) {
  if (size <= 0) throw std::invalid_argument("Size can't be less than zero");
  MemoryAllocate();
  InitializeIdentityMatrix();
}

Matrix4X4::Matrix4X4(const Matrix4X4& other) { *this = other; }

Matrix4X4::~Matrix4X4() {
  if (matrix_) MemoryFree();
  matrix_ = nullptr;
  rows_ = cols_ = 0;
}

void Matrix4X4::MulMatrix(const Matrix4X4& other) { *this *= other; }

Matrix4X4& Matrix4X4::operator=(const Matrix4X4& other) noexcept {
  if (matrix_ != nullptr) {
    MemoryFree();
  }
  rows_ = other.rows_;
  cols_ = other.cols_;
  MemoryAllocate();
  for (int i = 0; i < other.rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
  return *this;
}

Matrix4X4& Matrix4X4::operator*=(const Matrix4X4& other) noexcept {
  Matrix4X4 result_matrix;
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      for (int k = 0; k < cols_; ++k) {
        result_matrix.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = result_matrix;
  return *this;
}

Matrix4X4& Matrix4X4::operator*=(const double& num) noexcept {
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

double** Matrix4X4::GetMatrix() const noexcept { return matrix_; }

void Matrix4X4::SetData(const TransformData& other) {
  data_.x_rotation_deg = other.x_rotation_deg;
  data_.y_rotation_deg = other.y_rotation_deg;
  data_.z_rotation_deg = other.z_rotation_deg;
  data_.x_shift = other.x_shift;
  data_.y_shift = other.y_shift;
  data_.z_shift = other.z_shift;
  data_.user_scaler = other.user_scaler;
  data_.model_scaler = other.model_scaler;
  data_.perspective_ortho = other.perspective_ortho;
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
