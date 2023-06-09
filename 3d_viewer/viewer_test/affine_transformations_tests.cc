#include <gtest/gtest.h>

#include "s21_matrix4X4.h"

constexpr double ACCURACY = 1e-07;

TEST(calculation_src_test, test_1) {
  float data[16] = {0};
  s21::Matrix4X4 m;
  s21::TransformData transformations;
  transformations.x_shift = -12.67;
  transformations.y_shift = 34.21;
  transformations.z_shift = 10.01;
  m.MakeMovement();
  float result[16] = {
      0, 0, 0, transformations.x_shift,      0, 0, 0, transformations.y_shift,
      0, 0, 0, -5 + transformations.z_shift, 0, 0, 0, 1};

  for (int i = 0; i < 16; i++) {
    EXPECT_NEAR(data[i], result[i], ACCURACY);
  }
}
