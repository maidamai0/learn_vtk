#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
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
  const auto cone_property = cone_actor->GetProperty();
  cone_property->SetColor(0.2, 0.63, 0.79);
  cone_property->SetDiffuse(0.7);
  cone_property->SetSpecular(0.4);
  cone_property->SetSpecularPower(20);

  const auto cone_actor1 = vtkActor::New();
  cone_actor1->SetMapper(cone_mapper);
  const auto cone_property1 = vtkProperty::New();
  cone_property1->SetColor(1.0, 0.3882, 0.2784);
  cone_property1->SetDiffuse(0.7);
  cone_property1->SetSpecular(0.4);
  cone_property1->SetSpecularPower(20);
  cone_actor1->SetProperty(cone_property1);
  cone_actor1->SetPosition(0, 2, 0);

  const auto render = vtkRenderer::New();
  render->AddActor(cone_actor);
  render->AddActor(cone_actor1);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto wind = vtkRenderWindow::New();
  wind->SetSize(300, 300);
  wind->AddRenderer(render);

  for (int i = 0; i < 36000; ++i) {
    wind->Render();
    render->GetActiveCamera()->Azimuth(1);
  }

  return 0;
}