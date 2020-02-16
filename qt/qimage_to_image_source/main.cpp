#include <QApplication>
#include <QPixmap>

#include "vtkActor.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkQImageToImageSource.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

auto main(int argc, char** argv) -> int {
  QApplication app(argc, argv);
  QPixmap pixmap(1000, 1000);
  QColor color(10, 20, 30);
  pixmap.fill(color);

  vtkNew<vtkQImageToImageSource> image_source;
  auto qimage = pixmap.toImage();
  image_source->SetQImage(&qimage);
  image_source->Update();

  // TODO how to visialize image?

  return 0;
}