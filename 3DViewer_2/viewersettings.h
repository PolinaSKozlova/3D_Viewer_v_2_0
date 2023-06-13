#ifndef VIEWERSETTINGS_H
#define VIEWERSETTINGS_H

#include <QColor>
#include <QColorDialog>
#include <string>

#include "QtWidgets/qapplication.h"

namespace s21 {
std::string FindOutBasePath() {
  using namespace std;
  using namespace filesystem;

  QString QbasePath = QCoreApplication::applicationDirPath();
  string basePath = QbasePath.toStdString();
  return basePath;
}

struct UiState {
  UiState()
      : filePath(FindOutBasePath() + "/logo.obj"),
        n_verticies(0),
        n_indices(0),
        x_rotation_deg(0),
        y_rotation_deg(0),
        z_rotation_deg(0),
        x_shift(0),
        y_shift(0),
        z_shift(0),
        user_scaler(1),
        perspective(false),
        v_style(0),
        v_size(0),
        e_style(1),
        e_size(0),
        v_color(QColor(255, 255, 255)),
        e_color(QColor(136, 153, 166)),
        bg_color(QColor(25, 39, 52)) {}
  std::string filePath;
  int n_verticies;
  int n_indices;
  int x_rotation_deg;
  int y_rotation_deg;
  int z_rotation_deg;
  int x_shift;
  int y_shift;
  int z_shift;
  int user_scaler;
  bool perspective;
  int v_style;
  int v_size;
  int e_style;
  int e_size;
  QColor v_color;
  QColor e_color;
  QColor bg_color;
};

class ViewerSettings {
 public:
  ViewerSettings();
  void SetState(UiState& uiState);
  void LoadConf(std::string& confPath, UiState& uiState);
  void SaveConf(std::string& basePath, UiState& uiState);
  void SetDefaultTransforms();
  void SetDefaultStyle();

 private:
  UiState conf_settings_{};
};

};      // namespace s21
#endif  // VIEWERSETTINGS_H
