#include "viewersettings.h"

namespace s21 {
ViewerSettings::ViewerSettings() {}

void ViewerSettings::SaveConf(std::string &basePath, UiState &uiState)
{

}

void ViewerSettings::SetDefaultTransforms() {
  conf_settings_.x_rotation_deg = 0;
  conf_settings_.y_rotation_deg = 0;
  conf_settings_.z_rotation_deg = 0;
  conf_settings_.x_shift = 0;
  conf_settings_.y_shift = 0;
  conf_settings_.z_shift = 0;
  conf_settings_.user_scaler = 1;
  conf_settings_.perspective = false;
}

void ViewerSettings::SetDefaultStyle() {
  conf_settings_.e_style = 1;
  conf_settings_.e_size = 0;
  conf_settings_.v_style = 0;
  conf_settings_.v_size = 0;
  conf_settings_.bg_color = QColor(25, 39, 52);
  conf_settings_.e_color = QColor(136, 153, 166);
  conf_settings_.v_color = QColor(255, 255, 255);
}
};  // namespace s21
