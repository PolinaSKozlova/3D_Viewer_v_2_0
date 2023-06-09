#include "oglwidget.h"

s21::OGLWidget::OGLWidget(QWidget* parent)
    // Список инициализации, индексный буффер должен быть проинициализирован
    // явно
    : QOpenGLWidget(parent), indexBuf(QOpenGLBuffer::IndexBuffer) {}

s21::OGLWidget::~OGLWidget() { cleanUp(); }

void s21::OGLWidget::cleanUp() {
  arrayBuf.destroy();
  indexBuf.destroy();
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
  //  loadGeometry(filePath);
}

// Вызывается каждый раз при перерисовке
void s21::OGLWidget::paintGL() {
  calculateProjection();
  // цвет должен меняться в paintGl чтоб его можно было выбрать
  glClearColor(
      style.bg_color.red() / 255., style.bg_color.green() / 255.,
      style.bg_color.blue() / 255.,
      1);  // Здесь указывается цвет, с помощью которого будет очищаться
           // изображение (он же фон рендера), 4 цвет - альфа канал
           // (прозрачность, 0 ... 1) В glClear передаются флаги, указывающие,
           // какие буфферы должны быть очищены при перерисовке
  glClear(GL_COLOR_BUFFER_BIT);  // Очистка буфферов цвета при перерисовке
  /*
  - В шейдер нам нужно подать QMatrix4x4,
  - У класса QMatrix4x4 есть конструктор от const float*
  Код ниже делает следующее:
  - создает переменную float[16], инициализирует её 0
  - адрес первого значения массива передается в вызов функции афинных
  трансформаций как указатель float* (по усл. задачи модуль выполнен на С,
  заголовочный файл transformations.h). После выполнения всех трансформаций в
  исходном массиве float[16] окажутся модифицированные трансформациями значения.
  Для создания модифицированной модельно-видовой матрицы:
  - адрес массива float[16] помещается во вспомогательную переменную (const
  float*)
  - эта переменная передается в конструктор QMatrix4x4
  */
  float transform[16] = {0};  // В этом массиве будет содержаться композиция
                              // афинных преобразований модели.
  float* p_to_data = (float*)transform;
  //  transform_matrix(p_to_data, &transformations);

  const float* p_to_transform = (const float*)p_to_data;

  QMatrix4x4 matrix(
      p_to_transform);  // Инициируем модельно - видовую матрицу матрицей
                        // трансформаций, вычисленной из афинных преобразований

  program.bind();  // Снова биндим шейдерную программу
  program_P.bind();

  // Set modelview-projection matrix
  program.setUniformValue(
      "mvp_matrix",
      projection *
          matrix);  // Тут происходит связывание переменной внутри вершинного
                    // шейдера с переменной matrix из виджета
  program.setUniformValue("line_color", style.e_color.red() / 255.,
                          style.e_color.green() / 255.,
                          style.e_color.blue() / 255., 1.0);

  // Тут мы снова привязываем вершинный и индексный буферы
  arrayBuf.bind();
  indexBuf.bind();
  // Offset for position
  quintptr offset = 0;

  int vertexLocation = program.attributeLocation("a_position");
  program.enableAttributeArray(vertexLocation);
  program_P.enableAttributeArray(vertexLocation);

  program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, 0);
  program_P.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, 0);

  //    glEnable(GL_CULL_FACE);
  //    glCullFace(GL_BACK);

  glLineWidth(style.e_size);
  if (style.e_style == 2) {
    glLineStipple(4, 0x0C0F);
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  if (style.e_style != 0) {
    glDrawElements(GL_TRIANGLES, modelData.n_indices, GL_UNSIGNED_INT, nullptr);
  }

  // That part not so good, need to refactor it
  float pointSize = (style.v_size != 0) ? style.v_size : 0;
  program_P.setUniformValue("point_size", pointSize);

  if (style.v_style != 0) {
    if (style.v_style == 1) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }
    program_P.setUniformValue("mvp_matrix", projection * matrix);
    program_P.setUniformValue("dot_color", style.v_color.red() / 255.,
                              style.v_color.green() / 255.,
                              style.v_color.blue() / 255., 1.0);
    glDrawElements(GL_POINTS, modelData.n_indices, GL_UNSIGNED_INT, nullptr);
  }
  // Here the broken part is ended))

  program.release();  // Освобождаем шейдерную программу после отрисовки
  program_P.release();
}
// Этот метод вызывается один раз, при изменении размеров виджета
void s21::OGLWidget::resizeGL(int w, int h) {
  aspect =
      w / float(h ? h : 1);  // не очень понятно, зачем тут тернарный оператор
  calculateProjection();
  // update() // Работает и без этого
}

void s21::OGLWidget::calculateProjection() {
  projection
      .setToIdentity();  // Инициализируем матрицу проекции единичной матрицей
  if (transformations.perspective == 0) {
    projection.perspective(
        22.5, aspect, 0.1,
        10.0);  // C помощью метода perspective формируем матрицу, необходимую
                // для задания перспективы
  } else {
    double scaler = 1.;
    projection.ortho(-scaler * aspect, scaler * aspect, -scaler, scaler, 0.1,
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
void s21::OGLWidget::loadGeometry(std::string& file_path) {
  //  int return_code = load_model(file_path.c_str(), &modelData);
  int return_code = 0;
  transformations.model_scaler = modelData.model_to_world_scaler;

  if (return_code == 0) {
    arrayBuf.create();  // Создаем буффер
    arrayBuf.bind();    // Tell OpenGL which VBOs to use

    float* p_to_data =
        *(float**)(modelData.p_to_vertices);  // Вот тут получаем адрес с первой
                                              // вершиной

    arrayBuf.allocate(p_to_data,
                      modelData.n_vertices * 3 *
                          sizeof(float));  // Тут allocate - это одновременно и
                                           // выделение памяти и загрузка
    arrayBuf.release();  // Отвязываем буффер до момента отрисовки

    // очистка памяти на указателе vertices_matrix
    // после инициализации вершинного буфера, память, захваченную парсером,
    // можно освобождать
    //    s21_free_vertices_matrix((float**)modelData.p_to_vertices);

    indexBuf.create();  // Создаем буффер
    indexBuf.bind();    // Tell OpenGL which VBOs to use

    indexBuf.allocate(
        modelData.p_to_indices,
        modelData.n_indices *
            sizeof(unsigned int));  // Тут allocate - это одновременно и
                                    // выделение памяти и загрузка

    // очистка памяти на указателе indices_array
    // после инициализации индексного буфера, память, захваченную парсером,
    // можно освобождать
    //    s21_free_indices_array((unsigned int*)modelData.p_to_indices);

    arrayBuf.release();  // Отвязываем буффер до момента отрисовки
  } else {
    QMessageBox::critical(
        this, "Warning",
        "Ошибка чтения файла: " + QString::fromStdString(filePath));
  }
}

void s21::OGLWidget::setNewGeometry() {
  loadGeometry(filePath);
  update();
}

void s21::OGLWidget::setWidgetState(s21::ui_state_t& uiState) {
  transform_data_t& T = transformations;
  s21::ui_state_t& S = uiState;

  filePath = S.filePath;

  T.x_rotation_deg = S.x_rotation_deg;
  T.y_rotation_deg = S.y_rotation_deg;
  T.z_rotation_deg = S.z_rotation_deg;

  T.x_shift = (S.x_shift / (double)100) * aspect;
  T.y_shift = S.y_shift / (double)100;
  T.z_shift = S.z_shift / (double)100;

  T.user_scaler = 1 + S.user_scaler / (double)100.;
  T.perspective = (char)S.perspective;

  style.bg_color = S.bg_color;
  style.e_color = S.e_color;
  style.v_color = S.v_color;

  style.e_style = S.e_style;
  style.e_size = S.e_size;

  style.v_style = S.v_style;
  style.v_size = S.v_size;

  update();
}

int s21::OGLWidget::getNVerticies() { return modelData.n_indices; }

int s21::OGLWidget::getNIndicies() { return modelData.n_vertices; }

std::string s21::OGLWidget::getFilePath() {
  using namespace std;
  string modelFilePath = "Model file: ";
  modelFilePath += filePath;

  return modelFilePath;
}
