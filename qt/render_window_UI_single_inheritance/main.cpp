#include "QVTKOpenGLNativeWidget.h"
#include "qapplication.h"
#include "qsurfaceformat.h"
#include "render_window_ui_file_single_inheritance.h"

auto main(int argc, char** argv) -> int {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
  QApplication app(argc, argv);

  RenderWindowUIFIleSingleleInheritance widget;
  widget.show();

  return QApplication::exec();
}