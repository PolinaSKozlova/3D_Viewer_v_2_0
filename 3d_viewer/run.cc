#include "affine_transformations/s21_matrix4X4.h"

int main() {
  s21::Matrix4X4 m(4);
  s21::TransformData transformations;
  transformations.x_shift = -26.67;
  transformations.y_shift = 37.89;
  transformations.z_shift = -10.54;
  transformations.user_scaler = 50;
  transformations.x_rotation_deg = 45;
  transformations.y_rotation_deg = 60;
  transformations.z_rotation_deg = 35;
  m.MakeMovement(transformations);
  m.print();
  double result[][4] = {{0.4095764 * 50, -0.2867879 * 50, 0.8660255 * 50,
                         transformations.x_shift},
                        {0.9072061 * 50, 0.2279859 * 50, -0.3535533 * 50,
                         transformations.y_shift},
                        {-0.09604633 * 50, 0.930474 * 50, 0.3535533 * 50,
                         -5 + transformations.z_shift},
                        {0, 0, 0, 1}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j) {
      std::cout << result[i][j] << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}
