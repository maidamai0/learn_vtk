#include "render_window_ui_file_single_inheritance.h"
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

RenderWindowUIFIleSingleleInheritance::RenderWindowUIFIleSingleleInheritance() {
  ui_ = new Ui_RenderWindowUISingleInheritance();
  ui_->setupUi(this);
  vtkNew<vtkGenericOpenGLRenderWindow> render_window;
  ui_->qvtkWidget->SetRenderWindow(render_window);

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

  ui_->qvtkWidget->GetRenderWindow()->AddRenderer(render);
  ui_->qvtkWidget->GetRenderWindow()->SetWindowName(
    "Render window UI file Multiple Inheritance");
  connect(ui_->actionExit, SIGNAL(triggered()), this, SLOT(slot_exit()));
}

void RenderWindowUIFIleSingleleInheritance::slot_exit() {
  (void) this;
  qApp->exit();
}