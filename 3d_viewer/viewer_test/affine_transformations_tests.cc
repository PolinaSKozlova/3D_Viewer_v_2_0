#include <gtest/gtest.h>

#include "../affine_transformations/s21_matrix4X4.h"

constexpr double ACCURACY = 1e-07;

TEST(affine_transformations_test_shift, test_1) {
  s21::Matrix4X4 m;
  s21::TransformData transformations;
  transformations.x_shift = -12.67;
  transformations.y_shift = 34.21;
  transformations.z_shift = 10.01;
  m.MakeMovement(transformations);
  double result[][4] = {{0, 0, 0, transformations.x_shift},
                        {0, 0, 0, transformations.y_shift},
                        {0, 0, 0, -5 + transformations.z_shift},
                        {0, 0, 0, 1}};
  double **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}

TEST(affine_transformations_test_scaler, test_1) {
  s21::Matrix4X4 m;
  s21::TransformData transformations;
  transformations.user_scaler = 123.45;
  m.MakeMovement(transformations);
  double result[][4] = {{0, 0, 0, transformations.user_scaler},
                        {0, 0, 0, transformations.user_scaler},
                        {0, 0, 0, -5 + transformations.user_scaler},
                        {0, 0, 0, 1}};

  double **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}

TEST(affine_transformations_test_rotation, test_1) {
  s21::Matrix4X4 m;
  s21::TransformData transformations;
  transformations.x_rotation_deg = 45;
  transformations.y_rotation_deg = 60;
  transformations.z_rotation_deg = 35;
  m.MakeMovement(transformations);
  double result[][4] = {{cos(transformations.z_rotation_deg) *
                             cos(transformations.y_rotation_deg),
                         -sin(transformations.z_rotation_deg),
                         sin(transformations.y_rotation_deg), 0},
                        {sin(transformations.z_rotation_deg),
                         cos(transformations.z_rotation_deg) *
                             cos(transformations.x_rotation_deg),
                         -sin(transformations.x_rotation_deg), 0},
                        {-sin(transformations.y_rotation_deg),
                         cos(transformations.x_rotation_deg),
                         cos(transformations.y_rotation_deg) *
                             cos(transformations.x_rotation_deg),
                         -5 + transformations.z_shift},
                        {0, 0, 0, 1}};
  double **result_matrix = m.GetMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result_matrix[i][j], result[i][j], ACCURACY);
  }
}

// TEST(affine_transformations_test_all, test_1) {
//   float data[16] = {0};
//   s21::Matrix4X4 m;
//   s21::TransformData transformations;
//   transformations.x_shift = -26.67;
//   transformations.y_shift = 37.89;
//   transformations.z_shift = -10.54;
//   transformations.user_scaler = 50;
//   m.MakeMovement(transformations);
//   float result[16] = {
//       0, 0, 0, transformations.x_shift,      0, 0, 0,
//       transformations.y_shift, 0, 0, 0, -5 + transformations.z_shift, 0, 0,
//       0, 1};

//   for (int i = 0; i < 16; i++) {
//     EXPECT_NEAR(data[i], result[i], ACCURACY);
//   }
// }
