#ifndef THREE_D_VIEWER_CONTROLLER_H_
#define THREE_D_VIEWER_CONTROLLER_H_

#include "3DViewer_2/viewersettings.h"
#include "affine_transformations/s21_matrix4X4.h"
#include "parser/s21_parser.h"

namespace s21 {
class Controller {
 public:
  Controller() {
    std::cout << "Controller constructor" << std::endl;
    matrix4X4_.print();
    matrix4X4_.print_transformation();
  }
  ~Controller() = default;

  ModelObj StartParsingFile(const std::string& filename) {
    return parser_.Parse(filename);
  }

  float* CreateMatrixForMovements(TransformData& settings) {
    // matrix4X4_.print();
    matrix4X4_.MakeMovement(settings);
    std::cout << "CreateMatrixForMovements in controller" << std::endl;
    // matrix4X4_.print();
    return matrix4X4_.CreateOneRowMatrix();
    // return matrix4X4_;
  }

 private:
  s21::ModelObj model_obj_;
  s21::ObjParser parser_;
  s21::Matrix4X4 matrix4X4_{};
};

};  // namespace s21

#endif  // THREE_D_VIEWER_CONTROLLER_H_
