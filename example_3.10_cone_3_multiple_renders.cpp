#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

auto main() -> int {
  const auto cone_source = vtkConeSource::New();
  cone_source->SetResolution(10);
  cone_source->SetRadius(1.0);
  cone_source->SetHeight(3.0);

  const auto cone_mapper = vtkPolyDataMapper::New();
  cone_mapper->SetInputConnection(cone_source->GetOutputPort());

  const auto cone_actor = vtkActor::New();
  cone_actor->SetMapper(cone_mapper);

  const auto render = vtkRenderer::New();
  render->AddActor(cone_actor);
  render->SetBackground(0.1, 0.2, 0.4);
  render->SetViewport(0.0, 0.0, 0.5, 1.0);
  render->GetActiveCamera()->Azimuth(90);

  const auto render1 = vtkRenderer::New();
  render1->AddActor(cone_actor);
  render1->SetBackground(0.2, 0.3, 0.5);
  render1->SetViewport(0.5, 0.0, 1.0, 1.0);

  const auto wind = vtkRenderWindow::New();
  wind->SetSize(600, 300);
  wind->AddRenderer(render);
  wind->AddRenderer(render1);

  render->ResetCamera();
  render->GetActiveCamera()->Azimuth(90);

  for (int i = 0; i < 360; ++i) {
    wind->Render();
    render->GetActiveCamera()->Azimuth(1);
    render1->GetActiveCamera()->Azimuth(1);
  }

  return 0;
}