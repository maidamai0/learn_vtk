
#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkLODActor.h"
#include "vtkPlane.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"

namespace {
vtkSmartPointer<vtkPlane> plane;
vtkSmartPointer<vtkLODActor> select_actor;
vtkSmartPointer<vtkImplicitPlaneWidget> implicit_plane;
} // namespace

class MyCallback : public vtkCommand {
public:
  static MyCallback* New() {
    return new MyCallback();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    implicit_plane->GetPlane(plane);
    select_actor->VisibilityOn();
  }
};

int main() {
  const auto sphere = vtkSmartPointer<vtkSphereSource>::New();
  const auto cone = vtkSmartPointer<vtkConeSource>::New();

  const auto glyph = vtkSmartPointer<vtkGlyph3D>::New();
  glyph->SetInputConnection(sphere->GetOutputPort());
  glyph->SetSourceConnection(cone->GetOutputPort());
  glyph->SetVectorModeToUseNormal();
  glyph->SetScaleModeToScaleByVector();
  glyph->SetScaleFactor(0.25);

  const auto append = vtkSmartPointer<vtkAppendPolyData>::New();
  append->AddInputConnection(glyph->GetOutputPort());
  append->AddInputConnection(sphere->GetOutputPort());

  const auto mace_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mace_mapper->SetInputConnection(append->GetOutputPort());

  const auto mace_actor = vtkSmartPointer<vtkActor>::New();
  mace_actor->SetMapper(mace_mapper);
  mace_actor->VisibilityOn();

  plane = vtkSmartPointer<vtkPlane>::New();
  const auto clipper = vtkSmartPointer<vtkClipPolyData>::New();
  clipper->SetInputConnection(append->GetOutputPort());
  clipper->SetClipFunction(plane);
  clipper->InsideOutOn();

  const auto select_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  select_mapper->SetInputConnection(clipper->GetOutputPort());

  select_actor = vtkSmartPointer<vtkLODActor>::New();
  select_actor->SetMapper(select_mapper);
  select_actor->GetProperty()->SetColor(0, 1, 0);
  select_actor->VisibilityOff();
  select_actor->SetScale(1.01, 1.01, 1.01);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(mace_actor);
  render->AddActor(select_actor);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);
  const auto callback = vtkSmartPointer<MyCallback>::New();
  interactor->AddObserver(vtkCommand::InteractionEvent, callback);

  implicit_plane = vtkSmartPointer<vtkImplicitPlaneWidget>::New();
  implicit_plane->SetInteractor(interactor);
  implicit_plane->SetPlaceFactor(1.25);
  implicit_plane->SetInputConnection(glyph->GetOutputPort());
  implicit_plane->PlaceWidget();
  implicit_plane->EnabledOn();

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}