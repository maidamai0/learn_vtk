#include "vtkActor.h"
#include "vtkBYUReader.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkCubeAxesActor2D.h"
#include "vtkInteractorStyle.h"
#include "vtkLODActor.h"
#include "vtkLight.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTextProperty.h"

#include "platform.h"
#include <filesystem>
#include <iostream>

class AbortCommand : public vtkCommand {
public:
  static AbortCommand* New() {
    return new AbortCommand();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    const auto render_window = dynamic_cast<vtkRenderWindow*>(caller);

    if (render_window->GetEventPending() > 0) {
      render_window->SetAbortRender(1);
    }
  }

private:
  AbortCommand() = default;
  ~AbortCommand() final = default;
};

int main() {
  const auto reader = vtkSmartPointer<vtkBYUReader>::New();
  reader->SetGeometryFileName(
    (fs::canonical("Data").string() + "/teapot.g").c_str());
  const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(reader->GetOutputPort());

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(normals->GetOutputPort());

  const auto actor = vtkSmartPointer<vtkLODActor>::New();
  actor->SetMapper(mapper);

  const auto outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outline->SetInputConnection(normals->GetOutputPort());

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto camera = vtkSmartPointer<vtkCamera>::New();
  camera->SetClippingRange(1.60187, 20.0842);
  camera->SetFocalPoint(0.21406, 1.5, 0);
  camera->SetPosition(8.3761, 4.94858, 4.12505);
  camera->SetViewUp(0.180325, 0.549245, -0.815974);

  const auto light = vtkSmartPointer<vtkLight>::New();
  light->SetFocalPoint(0.21406, 1.5, 0);
  light->SetPosition(8.3761, 4.94858, 4.12505);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->SetViewport(0, 0, 0.5, 1.0);
  render->SetActiveCamera(camera);
  render->AddLight(light);
  render->AddViewProp(actor);
  render->AddViewProp(outline_actor);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto render_1 = vtkSmartPointer<vtkRenderer>::New();
  render_1->SetViewport(0.5, 0, 1.0, 1.0);
  render_1->SetActiveCamera(camera);
  render_1->AddLight(light);
  render_1->AddViewProp(actor);
  render_1->AddViewProp(outline_actor);
  render_1->SetBackground(0.1, 0.2, 0.4);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->AddRenderer(render_1);
  render_window->SetWindowName("VTK - cube axes");

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  const auto text_property = vtkSmartPointer<vtkTextProperty>::New();
  text_property->SetColor(1, 1, 1);
  text_property->ShadowOn();

  const auto cube_axes = vtkSmartPointer<vtkCubeAxesActor2D>::New();
  cube_axes->SetInputConnection(normals->GetOutputPort());
  cube_axes->SetCamera(camera);
  cube_axes->SetLabelFormat("%6.4g");
  cube_axes->SetFlyModeToOuterEdges();
  cube_axes->SetFontFactor(0.8);
  cube_axes->SetAxisLabelTextProperty(text_property);
  cube_axes->SetAxisTitleTextProperty(text_property);

  render->AddViewProp(cube_axes);

  const auto cube_axes_1 = vtkSmartPointer<vtkCubeAxesActor2D>::New();
  cube_axes_1->SetViewProp(actor);
  cube_axes_1->SetCamera(render_1->GetActiveCamera());
  cube_axes_1->SetLabelFormat("%6.4g");
  cube_axes_1->SetFlyModeToOuterEdges();
  cube_axes_1->SetFontFactor(0.8);
  cube_axes_1->ScalingOff();
  cube_axes_1->SetAxisTitleTextProperty(text_property);
  cube_axes_1->SetAxisLabelTextProperty(text_property);
  render_1->AddViewProp(cube_axes_1);

  const auto abort_command = vtkSmartPointer<AbortCommand>::New();
  render_window->AddObserver(vtkCommand::AbortCheckEvent, abort_command);

  std::cout << "reference count:" << render_window->GetReferenceCount()
            << std::endl;

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}