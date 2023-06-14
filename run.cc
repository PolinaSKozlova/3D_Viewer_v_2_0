#include "affine_transformations/s21_matrix4X4.h"
#include "parser/s21_parser.h"

int main() {
  s21::ObjParser pars;
  s21::Model model = pars.Parse("examplecube.obj");
  std::cout << model;
  // s21::Matrix4X4 m;
  // s21::TransformData transformations;
  // transformations.x_shift = 65;
  // transformations.y_rotation_deg = 45;
  // m.MakeMovement(transformations);
  // float *res = m.CreateOneRowMatrix();
  // m.print();
  // for (int i = 0; i < 4; i++) {
  //   for (int j = 0; j < 4; ++j) {
  //     std::cout << result[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  return 0;
}
