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

 private:
  s21::ModelObj model_obj_;
  s21::ObjParser parser_;
};

};  // namespace s21

#endif  // THREE_D_VIEWER_CONTROLLER_H_
