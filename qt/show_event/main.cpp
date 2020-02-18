#include "qapplication.h"
#include "qsurfaceformat.h"
#include "show_event.h"

auto main(int argc, char** argv) -> int {
  QApplication app(argc, argv);

  ShowEvent window;
  window.show();
  return app.exec();
}