#include "border_widget.h"
#include "qapplication.h"
#include "qsurfaceformat.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char** argv) {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
  QApplication app(argc, argv);

  BorderWidget widget;
  widget.show();
  return app.exec();
}