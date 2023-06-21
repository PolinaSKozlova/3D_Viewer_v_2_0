#ifndef THREE_D_VIEWER_CONTROLLER_H_
#define THREE_D_VIEWER_CONTROLLER_H_

#include "3DViewer_2/viewersettings.h"
#include "affine_transformations/s21_matrix4X4.h"
#include "parser/s21_parser.h"

namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  ModelObj StartParsingFile(const std::string& filename) {
    return parser_.Parse(filename);
  }

  float* CreateMatrixForMovements(ViewerSettings& settings) {
    matrix4X4_.MakeMovement(settings.GetTransformDataFromSettings());
    std::cout << "CreateMatrixForMovements in controller" << std::endl;
    return matrix4X4_.CreateOneRowMatrix();
  }

 private:
  s21::ModelObj model_obj_;
  s21::ObjParser parser_;
  s21::Matrix4X4 matrix4X4_{};
};

};  // namespace s21

#endif  // THREE_D_VIEWER_CONTROLLER_H_
