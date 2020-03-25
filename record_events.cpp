#include <vtkActor.h>
#include <vtkInteractorEventRecorder.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkSphereSource> sphere;
  sphere->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(sphere->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  vtkNew<vtkInteractorEventRecorder> recorder;
  recorder->SetInteractor(interactor);
  recorder->SetFileName("interactor_event.log");

  window->Render();
  recorder->EnabledOn();
  recorder->Record();
  interactor->Start();
}