#include "qapplication.h"
#include "qsurfaceformat.h"
#include "share_camera.h"

int main(int argc, char** argv) {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
  QApplication app(argc, argv);
  ShareCamera window;
  window.show();
  return app.exec();
}