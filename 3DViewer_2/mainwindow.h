#ifndef THREE_D_VIEWER_MAINWINDOW_H
#define THREE_D_VIEWER_MAINWINDOW_H

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>

#include "affine_transformations/s21_matrix4X4.h"
#include "parser/s21_parser.h"
#include "viewersettings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
#define FRAME_W 256
#define FRAME_H_MIN 30
#define FRAME_H_TR_CTRLS 284
#define FRAME_H_ST_CTRLS 284

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_xRotationSlider_valueChanged(int value);
  void on_yRotationSlider_valueChanged(int value);
  void on_zRotationSlider_valueChanged(int value);
  void on_uiShowButton_clicked();
  void on_showTransformsButton_clicked();
  void on_showStyleButton_clicked();
  void on_uiCloseButton_clicked();
  void on_actionShowUi_triggered();
  void on_xShiftSlider_valueChanged(int value);
  void on_yShiftSlider_valueChanged(int value);
  void on_zShiftSlider_valueChanged(int value);
  void on_userScalerSlider_valueChanged(int value);
  void on_setDefaultTransformsButton_clicked();
  void on_persperctiveComboBox_activated(int index);
  void on_actionOpen_File_triggered();
  void on_bgColorButton_clicked();
  void on_edgesColorButton_clicked();
  void on_verticiesColorButton_clicked();
  void on_verticiesTypeComboBox_activated(int index);
  void on_edgesTypeComboBox_activated(int index);
  void on_verticiesSizeSlider_valueChanged(int value);
  void on_edgesSizeSlider_valueChanged(int value);
  void on_xRotationSpinBox_valueChanged(double arg1);
  void on_yRotationSpinBox_valueChanged(double arg1);
  void on_zRotationSpinBox_valueChanged(double arg1);
  void on_xShiftSpinBox_valueChanged(double arg1);
  void on_yShiftSpinBox_valueChanged(double arg1);
  void on_zShiftSpinBox_valueChanged(double arg1);
  void on_userScalerSpinBox_valueChanged(double arg1);
  void on_setDefaultStyleButton_clicked();
  void on_vertexSizeSpinBox_valueChanged(double arg1);
  void on_edgesSizeSpinBox_valueChanged(double arg1);

  //  void on_Save_image_triggered();
  //  void on_Save_gif_triggered();
  //  void record_gif();
  //  void start_timer_gif();

 private:
  Ui::MainWindow* ui;
  std::string basePath;
  void findOutBasePath(std::string& basePath);
  void setState(std::string& basePath, ViewerSettings& uiState);
  void setDefaults(std::string& basePath);
  void updateUiState(int value, std::string& valueType);
  void syncUi();
  void minimizeUi();
  void maximizeUi();
  void minimizeUiTransforms();
  void maximizeUiTransforms();
  void minimizeUiStyle();
  void maximizeUiStyle();
  void showFileInfo();
  ViewerSettings viewer_conf_;
  QString file_name;
  QTimer* timer;  // идентификатор таймера
  int frame;
  //     QGifImage* gif;
  //     QFile* ptr_save_file;
};
};      // namespace s21
#endif  // THREE_D_VIEWER_MAINWINDOW_H
