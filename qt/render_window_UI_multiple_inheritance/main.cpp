#include "QVTKOpenGLNativeWidget.h"
#include "qapplication.h"
#include "qsurfaceformat.h"
#include "render_window_ui_file_multiple_inheritance.h"

auto main(int argc, char** argv) -> int {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
  QApplication app(argc, argv);

  RenderWindowUIFIleMultipleInheritance widget;
  widget.show();

  return QApplication::exec();
}