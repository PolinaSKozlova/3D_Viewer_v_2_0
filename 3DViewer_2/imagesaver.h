#ifndef IMAGESAVER_H
#define IMAGESAVER_H

#include <QBuffer>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLWidget>
#include <QTimer>

#include "gif_image/gifimage/qgifimage.h"
//#include "mainwindow.h"

namespace s21 {

class ImageSaver : public QMainWindow {  //, public QOpenGLWidget
  Q_OBJECT
 public:
  ImageSaver() = default;
  ~ImageSaver() = default;

  void SaveImage(QWidget* w);
  void SaveGif(QWidget* w);
  void SetOGLImage(QOpenGLWidget* image);

 private slots:
  void RecordGif();

 private:
  void StartTimerForGif(QWidget* w);
  QOpenGLWidget* image_;
  QString image_file_name_;
  QTimer* timer_;  // идентификатор таймера
  int frame_;
  QGifImage* gif_;
  QFile* ptr_save_file_;
};
};  // namespace s21

#endif  // IMAGESAVER_H
