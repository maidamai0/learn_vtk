#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSize>

#include "qnamespace.h"
#include "vtkActor.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkQImageToImageSource.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include <vtkActor2D.h>
#include <vtkImageMapper.h>
#include <vtkSmartPointer.h>

auto CreateImageActor(QString&& text) -> vtkSmartPointer<vtkActor2D> {
  QFont font("Lato", 18, QFont::Bold);

  QFontMetrics font_metrics(font);
  const auto text_width = font_metrics.horizontalAdvance(text) + 20;
  const int text_height = font_metrics.height() + 20;
  const auto border_width = 2;
  const auto border_radius = 10;

  QImage qimage(QSize(text_width + border_width * 2,
                      text_height + border_width * 2),
                QImage::Format_ARGB32);
  QPainter p(&qimage);
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  const auto border
    = QRectF(border_width, border_width, text_width, text_height);
  path.addRoundedRect(border, border_radius, border_radius);
  QPen pen(QColor("#4A4A4A"), border_width);
  p.setPen(pen);
  p.fillPath(path, Qt::white);
  p.drawPath(path);

  p.setPen(QPen("#4A4A4A"));
  p.setFont(font);
  p.drawText(border, Qt::AlignCenter, text);

  p.end();

  vtkNew<vtkQImageToImageSource> image_source;
  image_source->SetQImage(&qimage);
  image_source->Update();

  vtkNew<vtkImageMapper> mapper;
  mapper->SetInputData(image_source->GetOutput());
  mapper->SetColorWindow(255);  // width of the color range to map to
  mapper->SetColorLevel(127.5); // center of the color range to map to

  vtkNew<vtkActor2D> image;
  image->SetMapper(mapper);

  return image;
}

auto main(int argc, char** argv) -> int {
  QApplication app(argc, argv);

  auto short_string = CreateImageActor("Short");
  auto long_string = CreateImageActor("Longggg Stringggg");

  vtkNew<vtkRenderer> render;
  short_string->SetPosition(10, 100);
  render->AddActor(short_string);

  long_string->SetPosition(10, 10);
  render->AddActor(long_string);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->Start();
}