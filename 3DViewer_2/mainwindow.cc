#include "mainwindow.h"

#include "./ui_mainwindow.h"

s21::MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  findOutBasePath(basePath);
  setState(basePath, viewer_conf_);
  showFileInfo();
  minimizeUi();
}

s21::MainWindow::~MainWindow() {
  viewer_conf_.SaveConf(basePath, viewer_conf_);
  delete ui;
}

void s21::MainWindow::findOutBasePath(std::string& basePath) {
  using namespace std;
  using namespace filesystem;

  QString QbasePath = QCoreApplication::applicationDirPath();
  basePath = QbasePath.toStdString();
  std::cout << "base path " << basePath << std::endl;
}

void s21::MainWindow::setState(std::string& basePath, UiState& uiState) {
  using namespace std;
  using namespace filesystem;
  string confPath = basePath + "/session.conf";
  ifstream fileStream;
  fileStream.open(confPath);
  if (fileStream.good()) {
    std::cout << "i found suitable config" << '\n';
    fileStream.close();
    viewer_conf_.LoadConf(confPath);
  } else {
    std::cout << "no config founded" << '\n';
    setDefaults(basePath);
    // loadConf(confPath, uiState);
    std::cout << basePath;
    viewer_conf_.LoadConf(confPath);
    basePath += "/logo.obj";
    viewer_conf_.SetPath(basePath);
  }
  syncUi();
  ui->widget->setWidgetState(uiState);
}

void s21::MainWindow::setDefaults(std::string& basePath) {
  QString defaultConfPath = QString::fromStdString(basePath) + "/session.conf";
  QString defaultModelPath = QString::fromStdString(basePath) + "/logo.obj";
  QFile::copy(":/defaults/session.conf", defaultConfPath);
  QFile::setPermissions(defaultConfPath,
                        QFileDevice::ReadOwner | QFileDevice::WriteOwner);
  QFile::copy(":/defaults/logo.obj", defaultModelPath);
  QFile::setPermissions(defaultModelPath,
                        QFileDevice::ReadOwner | QFileDevice::WriteOwner);
}

void s21::MainWindow::updateUiState(int value, std::string& valueType) {
  if (valueType == "x_rotation_deg") viewer_conf_.x_rotation_deg = value;
  if (valueType == "y_rotation_deg") viewer_conf_.y_rotation_deg = value;
  if (valueType == "z_rotation_deg") viewer_conf_.z_rotation_deg = value;
  if (valueType == "x_shift") viewer_conf_.x_shift = value;
  if (valueType == "y_shift") viewer_conf_.y_shift = value;
  if (valueType == "z_shift") viewer_conf_.z_shift = value;
  if (valueType == "user_scaler") viewer_conf_.user_scaler = value;
  if (valueType == "perspective") viewer_conf_.perspective = value;
  ui->widget->setWidgetState(viewer_conf_);
  syncUi();
}

void s21::MainWindow::syncUi() {
  ui->xRotationSlider->setValue(viewer_conf_.x_rotation_deg);
  ui->xRotationSpinBox->setValue(viewer_conf_.x_rotation_deg);
  ui->yRotationSlider->setValue(viewer_conf_.y_rotation_deg);
  ui->yRotationSpinBox->setValue(viewer_conf_.y_rotation_deg);
  ui->zRotationSlider->setValue(viewer_conf_.z_rotation_deg);
  ui->zRotationSpinBox->setValue(viewer_conf_.z_rotation_deg);
  ui->xShiftSlider->setValue(viewer_conf_.x_shift);
  ui->xShiftSpinBox->setValue(viewer_conf_.x_shift);
  ui->yShiftSlider->setValue(viewer_conf_.y_shift);
  ui->yShiftSpinBox->setValue(viewer_conf_.y_shift);
  ui->zShiftSlider->setValue(viewer_conf_.z_shift);
  ui->zShiftSpinBox->setValue(viewer_conf_.z_shift);
  ui->userScalerSlider->setValue(viewer_conf_.user_scaler);
  ui->userScalerSpinBox->setValue(viewer_conf_.user_scaler);
  ui->persperctiveComboBox->setCurrentIndex(viewer_conf_.perspective);
  ui->verticiesTypeComboBox->setCurrentIndex(viewer_conf_.v_style);
  ui->verticiesSizeSlider->setValue(viewer_conf_.v_size);
  ui->vertexSizeSpinBox->setValue(viewer_conf_.v_size);
  ui->edgesTypeComboBox->setCurrentIndex(viewer_conf_.e_style);
  ui->edgesSizeSlider->setValue(viewer_conf_.e_size);
  ui->edgesSizeSpinBox->setValue(viewer_conf_.e_size);
}

void s21::MainWindow::on_uiShowButton_clicked() {
  if (ui->uiFrame->size().height() == FRAME_H_MIN) {
    maximizeUi();
  } else {
    minimizeUi();
  }
}

void s21::MainWindow::on_uiCloseButton_clicked() {
  ui->uiFrame->setVisible(false);
  ui->actionShowUi->setChecked(false);
}

void s21::MainWindow::on_showTransformsButton_clicked() {
  int currentUiH = ui->uiFrame->size().height();
  if (ui->uiTransformsFrame->size().height() == FRAME_H_MIN) {
    maximizeUiTransforms();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH + FRAME_H_TR_CTRLS);
  } else {
    minimizeUiTransforms();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH - FRAME_H_TR_CTRLS);
  }
}

void s21::MainWindow::on_showStyleButton_clicked() {
  int currentUiH = ui->uiFrame->size().height();
  if (ui->uiStyleFrame->size().height() == FRAME_H_MIN) {
    maximizeUiStyle();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH + FRAME_H_ST_CTRLS);
  } else {
    minimizeUiStyle();
    ui->uiFrame->setFixedSize(FRAME_W, currentUiH - FRAME_H_ST_CTRLS);
  }
}

void s21::MainWindow::minimizeUi() {
  ui->uiFrame->setFixedSize(FRAME_W, FRAME_H_MIN);
  ui->uiTransformsFrame->setVisible(false);
  ui->uiStyleFrame->setVisible(false);
}

void s21::MainWindow::maximizeUi() {
  ui->uiFrame->setFixedSize(FRAME_W, FRAME_H_MIN * 3);

  minimizeUiTransforms();
  ui->uiTransformsFrame->setVisible(true);
  minimizeUiStyle();
  ui->uiStyleFrame->setVisible(true);
}

void s21::MainWindow::minimizeUiTransforms() {
  ui->uiTransformsFrame->setFixedSize(FRAME_W, FRAME_H_MIN);
  ui->uiTransformsCtrlFrame->setVisible(false);
}

void s21::MainWindow::maximizeUiTransforms() {
  ui->uiTransformsFrame->setFixedSize(FRAME_W, FRAME_H_MIN + FRAME_H_TR_CTRLS);
  ui->uiTransformsCtrlFrame->setVisible(true);
}

void s21::MainWindow::minimizeUiStyle() {
  ui->uiStyleFrame->setFixedSize(FRAME_W, FRAME_H_MIN);
  ui->uiStyleCtrlFrame->setVisible(false);
}

void s21::MainWindow::maximizeUiStyle() {
  ui->uiStyleFrame->setFixedSize(FRAME_W, FRAME_H_MIN + FRAME_H_ST_CTRLS);
  ui->uiStyleCtrlFrame->setVisible(true);
}

void s21::MainWindow::showFileInfo() {
  using namespace std;
  ui->widget->getNIndicies();
  ui->widget->getNVerticies();

  string fileInfo = ui->widget->getFilePath();
  fileInfo += " N indicies ";
  fileInfo += to_string(uiState.n_indices);
  fileInfo += " N edges ";
  fileInfo += to_string((uiState.n_indices / 3) + uiState.n_verticies - 2);

  QString message = QString::fromStdString(fileInfo);
  ui->statusBar->showMessage(message);
}

void s21::MainWindow::on_actionShowUi_triggered() {
  if (ui->uiFrame->isVisible()) {
    ui->uiFrame->setVisible(false);
  } else {
    ui->uiFrame->setVisible(true);
  }
}

void s21::MainWindow::on_xRotationSlider_valueChanged(int value) {
  std::string valueType = "x_rotation_deg";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_yRotationSlider_valueChanged(int value) {
  std::string valueType = "y_rotation_deg";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_zRotationSlider_valueChanged(int value) {
  std::string valueType = "z_rotation_deg";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_xRotationSpinBox_valueChanged(double arg1) {
  std::string valueType = "x_rotation_deg";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_yRotationSpinBox_valueChanged(double arg1) {
  std::string valueType = "y_rotation_deg";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_zRotationSpinBox_valueChanged(double arg1) {
  std::string valueType = "z_rotation_deg";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_xShiftSlider_valueChanged(int value) {
  std::string valueType = "x_shift";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_yShiftSlider_valueChanged(int value) {
  std::string valueType = "y_shift";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_zShiftSlider_valueChanged(int value) {
  std::string valueType = "z_shift";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_xShiftSpinBox_valueChanged(double arg1) {
  std::string valueType = "x_shift";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_yShiftSpinBox_valueChanged(double arg1) {
  std::string valueType = "y_shift";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_zShiftSpinBox_valueChanged(double arg1) {
  std::string valueType = "z_shift";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_userScalerSlider_valueChanged(int value) {
  std::string valueType = "user_scaler";
  updateUiState(value, valueType);
}

void s21::MainWindow::on_userScalerSpinBox_valueChanged(double arg1) {
  std::string valueType = "user_scaler";
  updateUiState((int)arg1, valueType);
}

void s21::MainWindow::on_setDefaultTransformsButton_clicked() {
  viewer_conf_.SetDefaultTransforms();
  syncUi();
  ui->widget->setWidgetState(uiState);
}

void s21::MainWindow::on_persperctiveComboBox_activated(int index) {
  std::string valueType = "perspective";
  updateUiState(index, valueType);
}

void s21::MainWindow::on_actionOpen_File_triggered() {
  QString filter = "Obj File (*.obj)";
  QString inFileName = QFileDialog::getOpenFileName(
      this, "Open file", QString::fromStdString(basePath), filter);
  if (inFileName.isNull() == false) {
    uiState.filePath = inFileName.toStdString();
    viewer_conf_.SetDefaultTransforms();
    syncUi();
    ui->widget->setWidgetState(uiState);
    ui->widget->setNewGeometry();

    uiState.n_indices = ui->widget->getNIndicies();
    uiState.n_verticies = ui->widget->getNVerticies();
    showFileInfo();
  }
}

void s21::MainWindow::on_bgColorButton_clicked() {
  QColor bgColor = QColorDialog::getColor(Qt::white, this, "Choose color");
  if (bgColor.isValid()) {
    uiState.bg_color = bgColor;
    ui->widget->setWidgetState(uiState);
  }
}

void s21::MainWindow::on_edgesColorButton_clicked() {
  QColor eColor = QColorDialog::getColor(Qt::white, this, "Choose color");
  if (eColor.isValid()) {
    uiState.e_color = eColor;
    ui->widget->setWidgetState(uiState);
  }
}

void s21::MainWindow::on_verticiesColorButton_clicked() {
  QColor vColor = QColorDialog::getColor(Qt::white, this, "Choose color");
  if (vColor.isValid()) {
    uiState.v_color = vColor;
    ui->widget->setWidgetState(uiState);
  }
}

void s21::MainWindow::on_verticiesTypeComboBox_activated(int index) {
  uiState.v_style = index;
  ui->widget->setWidgetState(uiState);
}

void s21::MainWindow::on_verticiesSizeSlider_valueChanged(int value) {
  uiState.v_size = value;
  syncUi();
  ui->widget->setWidgetState(uiState);
}

void s21::MainWindow::on_vertexSizeSpinBox_valueChanged(double arg1) {
  uiState.v_size = arg1;
  ui->widget->setWidgetState(uiState);
  syncUi();
}

void s21::MainWindow::on_edgesTypeComboBox_activated(int index) {
  uiState.e_style = index;
  ui->widget->setWidgetState(uiState);
}

void s21::MainWindow::on_edgesSizeSlider_valueChanged(int value) {
  uiState.e_size = value;
  ui->widget->setWidgetState(uiState);
  syncUi();
}

void s21::MainWindow::on_edgesSizeSpinBox_valueChanged(double arg1) {
  uiState.e_size = arg1;
  ui->widget->setWidgetState(uiState);
  syncUi();
}

void s21::MainWindow::on_setDefaultStyleButton_clicked() {
  viewer_conf_.SetDefaultStyle();
  syncUi();
  ui->widget->setWidgetState(uiState);
}

// void s21::MainWindow::on_Save_image_triggered() {
//   QString filter = "*.bmp;; *.jpeg";
//   file_name = "/0";
//   file_name = QFileDialog::getSaveFileName(this, "Сохранить файл",
//                                            QDir::homePath(), filter);
//   ui->widget->grabFramebuffer().save(file_name);
//   if (file_name.isNull() == false) {
//     QMessageBox::about(this, "Статус", "Изображение сохранено");
//   }
// }

// void s21::MainWindow::on_Save_gif_triggered() {
//     QString filter = "*.gif";
//     file_name = "/0";
//     file_name = QFileDialog::getSaveFileName(this, "Сохранить GIF",
//                                              QDir::homePath(), filter);

//    ptr_save_file = new QFile(file_name);
//    if (!ptr_save_file->open(QIODevice::WriteOnly)) {
//        QMessageBox::critical(this, "Warning", "Не удалось записать файл");
//    } else {
//        start_timer_gif();
//    }
//}

// void s21::MainWindow::start_timer_gif() {
//     gif = new QGifImage(QSize(640, 480));
//     timer = new QTimer(this);
//     connect(timer, &QTimer::timeout, this, &MainWindow::record_gif);
//     timer->start(100);
//     frame = 0;
// }

// void s21::MainWindow::record_gif() {
//     frame++;
//     gif->addFrame(ui->widget->grabFramebuffer(), 100);
//     if (frame == 50) {
//         timer->stop();
//         gif->save(ptr_save_file);
//         ptr_save_file->close();
//         delete ptr_save_file;
//         ptr_save_file = nullptr;
//         delete gif;
//         gif = nullptr;
//         QMessageBox::about(this, "Статус", "GIF сохранена");
//     }
// }
