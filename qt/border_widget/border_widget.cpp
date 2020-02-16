#include "border_widget.h"

#include <iostream>

#include "QVTKOpenGLNativeWidget.h"
#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"

class BorderCallback : public vtkCommand {
public:
  static BorderCallback* New() {
    return new BorderCallback();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    std::cout << __FUNCTION__ << "\n";
  }

private:
  BorderCallback() = default;
};

BorderWidget::BorderWidget() {
  setupUi(this);

  vtkNew<vtkSphereSource> sphere_source;
  sphere_source->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(sphere_source->GetOutputPort());

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(colors->GetColor4d("SteelBlue").GetData());

  vtkNew<vtkGenericOpenGLRenderWindow> render_window;
  render_window->AddRenderer(render);
  qvtkWidget->SetRenderWindow(render_window);

  border_widget_ = vtkSmartPointer<vtkBorderWidget>::New();
  border_widget_->SetInteractor(qvtkWidget->GetInteractor());
  border_widget_->On();
}