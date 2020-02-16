#include <QApplication>
#include <QSurfaceFormat>

#include "QVTKOpenGLNativeWidget.h"
#include "vtkActor.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"

auto main(int argc, char** argv) -> int {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
  QApplication app(argc, argv);

  QVTKOpenGLNativeWidget widget;

  vtkNew<vtkRenderWindow> render_window;
  widget.SetRenderWindow(render_window);
  widget.resize(600, 600);

  vtkNew<vtkSphereSource> source;
  source->SetThetaResolution(200);
  source->SetPhiResolution(200);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(source->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  widget.GetRenderWindow()->AddRenderer(render);
  widget.GetRenderWindow()->SetWindowName("RenderWindowNoUIFile");

  vtkNew<vtkNamedColors> colors;
  actor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());
  render->SetBackground(colors->GetColor4d("SteelBlue").GetData());

  widget.show();

  return QApplication::exec();
}