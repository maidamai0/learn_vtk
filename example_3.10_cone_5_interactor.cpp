#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

auto main() -> int {
  const auto cone_source = vtkConeSource::New();
  cone_source->SetHeight(3.0);
  cone_source->SetRadius(1.0);
  cone_source->SetResolution(100);

  const auto cone_mapper = vtkPolyDataMapper::New();
  cone_mapper->SetInputConnection(cone_source->GetOutputPort());

  const auto cone_actor = vtkActor::New();
  cone_actor->SetMapper(cone_mapper);

  const auto render = vtkRenderer::New();
  render->AddActor(cone_actor);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto wind = vtkRenderWindow::New();
  wind->AddRenderer(render);
  wind->SetSize(300, 300);

  const auto interactor = vtkRenderWindowInteractor::New();
  interactor->SetRenderWindow(wind);

  const auto interactor_style = vtkInteractorStyleTrackballCamera::New();
  interactor->SetInteractorStyle(interactor_style);

  interactor->Initialize();
  interactor->Start();

  cone_source->Delete();
  cone_mapper->Delete();
  cone_actor->Delete();
  render->Delete();
  wind->Delete();

  return 0;
}