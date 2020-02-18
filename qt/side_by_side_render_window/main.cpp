#include "qapplication.h"
#include "side_by_side_render_window.h"

auto main(int argc, char** argv) -> int {
  QApplication app(argc, argv);
  SideBySideRenderWindow window;
  window.show();
  return app.exec();
}