#ifndef THREE_D_VIEWER_OGLWIDGET_H
#define THREE_D_VIEWER_OGLWIDGET_H

// QT includes
#include <QMatrix4x4>
#include <QOpenGLWidget>
// OpenGL imports
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
// QT imports
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "mainwindow.h"

namespace s21 {
// Colors, linetypes etc.
struct StyleData {
  int v_style;
  int v_size;
  int e_style;
  int e_size;
  QColor v_color;
  QColor e_color;
  QColor bg_color;
};

typedef enum { PERSPECTIVE, ORTHO } projection_t;

constexpr double DEFAULT_ASPECT_RATIO = 1.865889;

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OGLWidget(QWidget* parent = nullptr);
  ~OGLWidget();
  void setWidgetState(ViewerSettings& uiState);
  void setNewGeometry();
  int getNVerticies();
  int getNIndicies();
  std::string getFilePath();

  /* Для реализации функционала мы должны переопределить 3 метода:
   * 1. initializeGL() - вызывается один раз при инициализации контекста OpenGL
   * 2. paintGL - вызывается при каждом изменении изображения, пересчитывает
   * изображение
   * 3. resizeGL - вызывается при изменении размеров виджета, пересчитывает
   * изображение?
   */
 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  // Эта функция компилирует и линкует шейдеры,
  // Если шейдеры не компилируются, всё приложение закрывается
  void initShaders();
  // Функция ниже загружает геометрию модели
  void loadGeometry(std::string& file_path);
  void cleanUp();

 private:
  //  s21::Controller* controller_;
  std::string file_path_;
  float aspect = DEFAULT_ASPECT_RATIO;
  //  model_data_t modelData;  // Модель
  ModelObj model_obj_;
  // default member initialisation, google it!
  TransformData transformations_;
  StyleData style_;
  //  ViewerSettings ui_conf_;
  QOpenGLShaderProgram
      program;  // Шейдерная программа, сюда собираются все шейдеры
  QOpenGLShaderProgram program_P;  // Шейдерная программа для вершин
  QOpenGLShaderProgram
      program_dash;  // Шейдерная программа для модификации начертания линий
  QMatrix4x4 projection_;  // Матрица проекций, используется при отображении,
                           // разобраться как это работает
  Matrix4X4 affine_transformation_matrix_{};
  QOpenGLBuffer array_buf_;  // Вершинный буффер
  QOpenGLBuffer index_buf_;  // Индексный буффер
  void CalculateProjection();
};
};      // namespace s21
#endif  // THREE_D_VIEWER_OGLWIDGET_H
