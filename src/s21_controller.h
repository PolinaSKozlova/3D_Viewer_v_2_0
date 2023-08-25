#ifndef THREE_D_VIEWER_CONTROLLER_H_
#define THREE_D_VIEWER_CONTROLLER_H_

#include "imagesaver.h"
#include "parser/s21_parser.h"

namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  s21::ModelObj StartParsingFile(const std::string& filename) {
    s21::ModelObj result;
    try {
      output_ = "";
      result = parser_.Parse(filename);
    } catch (std::exception& e) {
      output_ = e.what();
    }
    return result;
  }
  std::string GetOutput() const { return output_; }

  void SaveImage(QMainWindow* w, QOpenGLWidget* ogl) {
    saver_.SetOGLImage(ogl);
    saver_.SaveImage(w);
  }

  void SaveGif(QMainWindow* w, QOpenGLWidget* ogl) {
    saver_.SetOGLImage(ogl);
    saver_.SaveGif(w);
  }

 private:
  s21::ObjParser parser_;
  std::string output_{};
  s21::ImageSaver saver_;
};
};  // namespace s21

#endif  // THREE_D_VIEWER_CONTROLLER_H_
