#include "event_qt_slot_connect.h"

#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkCubeSource.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include <iostream>

EventQtSlotConnect::EventQtSlotConnect() {
  setupUi(this);

  vtkNew<vtkCubeSource> source;
  source->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(source->GetOutputPort());

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

  connections_ = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  connections_->Connect(
    qvtkWidget->GetRenderWindow()->GetInteractor(),
    vtkCommand::LeftButtonPressEvent, this,
    SLOT(slot_clicked(vtkObject*, unsigned long, void*, void*)));
}

void EventQtSlotConnect::slot_clicked(vtkObject*, unsigned long, void*, void*) {
  std::cout << __FUNCTION__ << std::endl;
}