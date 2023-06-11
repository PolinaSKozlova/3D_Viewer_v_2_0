#ifndef OGLWIDGET_H
#define OGLWIDGET_H

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

#include "3d_viewer/affine_transformations/s21_matrix4X4.h"
#include "mainwindow.h"

namespace s21 {
//==================from parser===================
typedef struct model_data {
  int n_vertices;
  int n_indices;
  void* p_to_vertices;
  void* p_to_indices;
  double model_to_world_scaler;
} model_data_t;

// Colors, linetypes etc.
typedef struct style_data {
  int v_style;
  int v_size;

  int e_style;
  int e_size;

  QColor v_color;
  QColor e_color;
  QColor bg_color;
} style_data_t;

typedef enum { PERSPECTIVE, ORTHO } projection_t;

#define DEFAULT_ASPECT_RATIO 1.865889

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OGLWidget(QWidget* parent = nullptr);
  ~OGLWidget();
  void setWidgetState(s21::ui_state_t& uiState);
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
  void initializeGL() override;  // TODO(weldonfe): Разобраться с override
  void paintGL() override;
  void resizeGL(int width, int height) override;
  // Эта функция компилирует и линкует шейдеры,
  // Если шейдеры не компилируются, всё приложение закрывается
  void initShaders();
  // Функция ниже загружает геометрию модели
  void loadGeometry(std::string& file_path);
  void cleanUp();

 private:
  std::string filePath;

  float aspect = DEFAULT_ASPECT_RATIO;

  model_data_t modelData;  // Модель
  // default member initialisation, google it!
  TransformData transformations;
  style_data_t style;

  QOpenGLShaderProgram
      program;  // Шейдерная программа, сюда собираются все шейдеры
  QOpenGLShaderProgram program_P;  // Шейдерная программа для вершин
  QOpenGLShaderProgram
      program_dash;  // Шейдерная программа для модификации начертания линий

  QMatrix4x4 projection;  // Матрица проекций, используется при отображении,
                          // разобраться как это работает

  Matrix4X4 affine_transformation_matrix_{};
  QOpenGLBuffer arrayBuf;  // Вершинный буффер
  QOpenGLBuffer indexBuf;  // Индексный буффер

  void calculateProjection();
};
};      // namespace s21
#endif  // OGLWIDGET_H
