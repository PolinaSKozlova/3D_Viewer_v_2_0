#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle("fusion");
  s21::MainWindow w;
  w.show();
  return a.exec();
}
