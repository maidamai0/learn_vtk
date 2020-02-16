#include "render_window_ui_file_multiple_inheritance.h"
#include "qapplication.h"
#include "qcoreapplication.h"
#include "qobjectdefs.h"
#include "vtkActor.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"

RenderWindowUIFIleMultipleInheritance::RenderWindowUIFIleMultipleInheritance() {
  setupUi(this);
  vtkNew<vtkGenericOpenGLRenderWindow> render_window;
  qvtkWidget->SetRenderWindow(render_window);

  vtkNew<vtkSphereSource> source;
  source->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(source->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkNamedColors> colors;
  actor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());
  render->SetBackground(colors->GetColor4d("SteelBlue").GetData());

  qvtkWidget->GetRenderWindow()->AddRenderer(render);
  qvtkWidget->GetRenderWindow()->SetWindowName(
    "Render window UI file Multiple Inheritance");
  connect(actionExit, SIGNAL(triggered()), this, SLOT(slot_exit()));
}

void RenderWindowUIFIleMultipleInheritance::slot_exit() {
  (void) this;
  qApp->exit();
}