#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSize>

#include "qcolor.h"
#include "qnamespace.h"
#include "qpainterpath.h"
#include "qpen.h"
#include "qrgb.h"
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
  qimage.fill(qRgba(0, 0, 0, 0));
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

auto CreateAnothorImageActor(QString&& text, const int number)
  -> vtkSmartPointer<vtkActor2D> {
  QFont font("Lato", 18, QFont::Bold);

  const int number_width = 50;
  QFontMetrics font_metrics(font);
  const auto text_width
    = font_metrics.horizontalAdvance(text) + 20 + number_width;
  const int text_height = font_metrics.height() + 20;
  const auto border_width = 2;
  const auto border_radius = 10;

  QImage qimage(QSize(text_width + border_width * 2,
                      text_height + border_width * 2),
                QImage::Format_ARGB32);
  qimage.fill(qRgba(0, 0, 0, 0));
  QPainter p(&qimage);
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  const auto border
    = QRectF(border_width, border_width, text_width, text_height);
  path.addRoundedRect(border, border_radius, border_radius);
  QPen pen(QColor("#4A4A4A"), border_width);
  auto blue_pen = pen;
  blue_pen.setWidth(0);
  p.setPen(blue_pen);
  p.fillPath(path, Qt::white);
  p.drawPath(path);

  // number with color background
  // counter-clockwise
  QPainterPath number_area;
  number_area.moveTo(border_radius + border_width,
                     border_width); // about top-left start point
  number_area.arcTo(
    QRectF(border_width, border_width, border_radius, border_radius), 90, 90);
  number_area.lineTo(border_width, text_height - border_radius - border_width);
  number_area.arcTo(QRectF(border_width,
                           text_height - border_radius + border_width,
                           border_radius, border_radius),
                    180, 90);
  number_area.lineTo(100, text_height + border_width);
  number_area.lineTo(100, border_width);
  number_area.closeSubpath();
  p.fillPath(number_area, QColor("#0078FF"));
  p.drawPath(number_area);

  p.setPen(QPen("#4A4A4A"));
  p.setFont(font);
  p.drawText(QRectF(border_width, border_width, number_width, text_height),
             QString("#") + number);
  p.drawText(QRectF(number_width, border_width, text_width - number_width,
                    text_height),
             Qt::AlignCenter, text);

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

auto CreateThirdImageActor(QString&& text, const int number)
  -> vtkSmartPointer<vtkActor2D> {
  QFont font("Lato", 18, QFont::Bold);

  const int number_width = 50;
  QFontMetrics font_metrics(font);
  const auto text_width
    = font_metrics.horizontalAdvance(text) + 20 + number_width;
  const int text_height = font_metrics.height() + 20;
  const auto border_width = 2;
  const auto border_radius = 10;

  QImage qimage(QSize(text_width + border_width * 2,
                      text_height + border_width * 2),
                QImage::Format_ARGB32);
  qimage.fill(qRgba(0, 0, 0, 0));
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

  // number with color background
  // counter-clockwise
  QPainterPath number_area_left;
  const auto number_rect_left
    = QRectF(border_width, border_width, 100, text_height);
  number_area_left.addRoundedRect(number_rect_left, border_radius,
                                  border_radius);

  p.setPen(pen);
  p.fillPath(number_area_left, QColor("#0078FF"));
  p.drawPath(number_area_left);

  QPainterPath number_area_right;
  auto number_rect_right = number_rect_left;
  number_rect_right.setX(number_rect_right.x() + border_radius);
  number_rect_right.setY(number_rect_right.y() + border_width);
  number_rect_right.setHeight(number_rect_right.height() - border_width);
  number_area_right.addRect(number_rect_right);
  auto blue_pen = QPen(QColor("#0078FF"), border_width);
  p.setPen(blue_pen);
  p.fillPath(number_area_right, QColor("#0078FF"));
  p.drawPath(number_area_right);

  p.setPen(QPen("#4A4A4A"));
  p.setFont(font);
  p.drawText(QRectF(border_width, border_width, number_width, text_height),
             QString("#22"));
  p.drawText(QRectF(number_width, border_width, text_width - number_width,
                    text_height),
             Qt::AlignCenter, text);

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
  auto long_string = CreateThirdImageActor("Longggg Stringggg", 22);

  vtkNew<vtkRenderer> render;
  render->SetBackground(255, 255, 255);
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