#define GL_SILENCE_DEPRECATION
#include "oglwidget.h"

s21::OGLWidget::OGLWidget(QWidget* parent)
    // Список инициализации, индексный буффер должен быть проинициализирован
    // явно
    : QOpenGLWidget(parent), index_buf_(QOpenGLBuffer::IndexBuffer) {}

s21::OGLWidget::~OGLWidget() { cleanUp(); }

void s21::OGLWidget::cleanUp() {
  array_buf_.destroy();
  index_buf_.destroy();
}

// Вызывается один раз при создании нового OpenGL контекста, т.е. при
// инициализации виджета
void s21::OGLWidget::initializeGL() {
  /*
  After calling this function,
  the QOpenGLFunctions object can only be used with the current context
  and other contexts that share with it.
  Call initializeOpenGLFunctions() again to change the object's context
  association.
  */
  initializeOpenGLFunctions();

  // Вызов ниже переопределяет режим отрисовки примитивов,
  // Мы переопределяем стандартный сплошной рендер на рендер только граней
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  //    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  initShaders();
  try {
    //    loadGeometry(file_path_);
    loadGeometry();
  } catch (std::runtime_error& e) {
    QMessageBox::information(this, "Warning", e.what());
    file_path_ =
        QCoreApplication::applicationDirPath().toStdString() + "/logo.obj";
    std::cout << "initializeGL " << file_path_ << std::endl;
  }
}

// Вызывается каждый раз при перерисовке
void s21::OGLWidget::paintGL() {
  CalculateProjection();
  // цвет должен меняться в paintGL чтоб его можно было выбрать
  glClearColor(
      style_.bg_color.red() / 255., style_.bg_color.green() / 255.,
      style_.bg_color.blue() / 255.,
      1);  // Здесь указывается цвет, с помощью которого будет очищаться
           // изображение (он же фон рендера), 4 цвет - альфа канал
           // (прозрачность, 0 ... 1) В glClear передаются флаги, указывающие,
           // какие буфферы должны быть очищены при перерисовке
  glClear(GL_COLOR_BUFFER_BIT);  // Очистка буфферов цвета при перерисовке

  affine_transformation_matrix_.MakeMovement(transformations_);

  QMatrix4x4 matrix(affine_transformation_matrix_.CreateOneRowMatrix());
  //  float* matrixOne_row =
  //      controller_->CreateMatrixForMovements(transformations_);

  //  affine_transformation_matrix_ =
  //      controller_->CreateMatrixForMovements(transformations_);

  //  QMatrix4x4 matrix(matrixOne_row);

  program.bind();  // Снова биндим шейдерную программу
  program_P.bind();

  // Set modelview-projection matrix
  program.setUniformValue(
      "mvp_matrix",
      projection_ *
          matrix);  // Тут происходит связывание переменной внутри вершинного
                    // шейдера с переменной matrix из виджета
  program.setUniformValue("line_color", style_.e_color.red() / 255.,
                          style_.e_color.green() / 255.,
                          style_.e_color.blue() / 255., 1.0);

  // Тут мы снова привязываем вершинный и индексный буферы
  array_buf_.bind();
  index_buf_.bind();

  // Offset for position
  quintptr offset = 0;

  int vertexLocation = program.attributeLocation("a_position");
  program.enableAttributeArray(vertexLocation);
  program_P.enableAttributeArray(vertexLocation);

  program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, 0);
  program_P.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, 0);

  //    glEnable(GL_CULL_FACE);
  //    glCullFace(GL_BACK);

  glLineWidth(style_.e_size);
  if (style_.e_style == 2) {
    glLineStipple(4, 0x0C0F);
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  if (style_.e_style != 0) {
    glDrawElements(GL_TRIANGLES, model_obj_.faces.size(), GL_UNSIGNED_INT,
                   nullptr);
  }

  // That part not so good, need to refactor it
  float pointSize = (style_.v_size != 0) ? style_.v_size : 0;
  program_P.setUniformValue("point_size", pointSize);

  if (style_.v_style != 0) {
    if (style_.v_style == 1) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }
    program_P.setUniformValue("mvp_matrix", projection_ * matrix);
    program_P.setUniformValue("dot_color", style_.v_color.red() / 255.,
                              style_.v_color.green() / 255.,
                              style_.v_color.blue() / 255., 1.0);
    glDrawElements(GL_POINTS, model_obj_.faces.size(), GL_UNSIGNED_INT,
                   nullptr);
  }
  // Here the broken part is ended))

  program.release();  // Освобождаем шейдерную программу после отрисовки
  program_P.release();
}
// Этот метод вызывается один раз, при изменении размеров виджета
void s21::OGLWidget::resizeGL(int w, int h) {
  aspect = w / float(h ? h : 1);
  CalculateProjection();
}

void s21::OGLWidget::CalculateProjection() {
  projection_
      .setToIdentity();  // Инициализируем матрицу проекции единичной матрицей
  if (transformations_.perspective_ortho == false) {
    projection_.perspective(
        22.5, aspect, 0.1,
        10.0);  // C помощью метода perspective формируем матрицу, необходимую
                // для задания перспективы
  } else {
    double scaler = 1.;
    projection_.ortho(-scaler * aspect, scaler * aspect, -scaler, scaler, 0.1,
                      10.0);
  }
  // в вызове perspective:
  // 1 - угол наклона камеры
  // 2 - aspect ratio (соотношение сторон)
  // 3, 4 - передняя и задняя плоскости отсечения

  // в вызове ortho:
  // 1, 2 - левая и правая границы
  // 3, 4 - нижняя и верхняя границы
  // 5, 6 - передняя и задняя плоскости отсечения
}

// Метод для компиляции и сборки шейдеров
void s21::OGLWidget::initShaders() {
  // Compile vertex shader
  if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    close();
  if (!program_P.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/vshader.glsl"))
    // Compile geometry shader
    if (!program_P.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                           ":/gshader.glsl"))
      close();
  // Compile fragment shader
  if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":/fshader.glsl"))
    close();
  // Compile second fragment shader for vertex color handling
  if (!program_P.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/fshaderP.glsl"))
    close();
  // Link shader pipeline
  if (!program.link()) close();
  if (!program_P.link()) close();
  // Bind shader pipeline for use
  if (!program.bind()) close();
  if (!program_P.bind()) close();
  program.release();  // Освобождаем шейдерную программу до момента отрисовки
  program_P.release();
}

// Загрузка модели
// void s21::OGLWidget::loadGeometry(std::string& file_path) {
void s21::OGLWidget::loadGeometry() {
  try {
    // std::cout << file_path_ << std::endl;
    // ObjParser parser{};
    // model_obj_ = parser.Parse(file_path);
    //    model_obj_ = controller_->StartParsingFile(file_path);

    transformations_.model_scaler = model_obj_.scaler;

    array_buf_.create();  // Создаем буффер
    array_buf_.bind();    // Tell OpenGL which VBOs to use

    float* p_to_data = model_obj_.vertices.data();

    array_buf_.allocate(p_to_data,
                        model_obj_.vertices.size() *
                            sizeof(float));  // Тут allocate - это  одновременно
                                             // и выделение памяти и загрузка
    array_buf_.release();  // Отвязываем буффер до момента отрисовки

    index_buf_.create();  // Создаем буффер
    index_buf_.bind();    // Tell OpenGL which VBOs to use

    index_buf_.allocate(
        model_obj_.faces.data(),
        model_obj_.faces.size() *
            sizeof(unsigned int));  // Тут allocate - это одновременно и
                                    // выделение памяти и загрузка

    index_buf_.release();  // Отвязываем буффер до момента отрисовки

  } catch (const std::invalid_argument& e) {
    // придумать решение получше
    std::cout << "i'm in loadgeometry" << std::endl;
    std::cout << file_path_ << std::endl;
    file_path_ =
        QCoreApplication::applicationDirPath().toStdString() + "/logo.obj";
    QMessageBox::critical(this, "Warning", e.what());
  }
}

void s21::OGLWidget::setNewGeometry(ModelObj&& other) {
  model_obj_ = std::move(other);
  loadGeometry();
  // loadGeometry(file_path_);
  update();
}

void s21::OGLWidget::setWidgetState(ViewerSettings& uiState) {
  file_path_ = uiState.GetUiState().filePath;
  transformations_.x_rotation_deg = uiState.GetUiState().x_rotation_deg;
  transformations_.y_rotation_deg = uiState.GetUiState().y_rotation_deg;
  transformations_.z_rotation_deg = uiState.GetUiState().z_rotation_deg;
  transformations_.x_shift = (uiState.GetUiState().x_shift / 50.0) * aspect;
  transformations_.y_shift = uiState.GetUiState().y_shift / 50.0;
  transformations_.z_shift = uiState.GetUiState().z_shift / 50.0;
  transformations_.user_scaler = 1 + uiState.GetUiState().user_scaler / 101.0;
  transformations_.perspective_ortho = uiState.GetUiState().perspective;
  style_.bg_color = uiState.GetUiState().bg_color;
  style_.e_color = uiState.GetUiState().e_color;
  style_.v_color = uiState.GetUiState().v_color;
  style_.e_style = uiState.GetUiState().e_style;
  style_.e_size = uiState.GetUiState().e_size;
  style_.v_style = uiState.GetUiState().v_style;
  style_.v_size = uiState.GetUiState().v_size;
  update();
}

int s21::OGLWidget::getNVerticies() { return model_obj_.vertices.size(); }

int s21::OGLWidget::getNIndicies() { return model_obj_.faces.size(); }

std::string s21::OGLWidget::getFilePath() {
  using namespace std;
  string model_file_path = "Model file: ";
  model_file_path += file_path_;
  return model_file_path;
}
