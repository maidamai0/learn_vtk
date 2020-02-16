#include "QVTKOpenGLNativeWidget.h"
#include "event_qt_slot_connect.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char** argv) {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
  QApplication app(argc, argv);

  EventQtSlotConnect connect;
  connect.show();

  return app.exec();
}