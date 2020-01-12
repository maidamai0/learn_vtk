#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

auto main() -> int {
  const auto sphere_source = vtkSphereSource::New();
  sphere_source->SetRadius(1.0);

  const auto sphere_smapper = vtkPolyDataMapper::New();
  sphere_smapper->SetInputConnection(sphere_source->GetOutputPort());

  const auto sphere_actor = vtkActor::New();
  sphere_actor->SetMapper(sphere_smapper);
  sphere_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
  sphere_actor->GetProperty()->SetAmbient(0.5);
  sphere_actor->GetProperty()->SetDiffuse(0.5);
  sphere_actor->GetProperty()->SetDiffuseColor(1.0, 0.0, 0.0);
  sphere_actor->GetProperty()->SetAmbientColor(0.0, 0.0, 1.0);

  const auto render = vtkRenderer::New();
  render->AddActor(sphere_actor);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto wind = vtkRenderWindow::New();
  wind->AddRenderer(render);
  wind->SetSize(300, 300);

  for (int i = 0; i < 10000; ++i) {
    wind->Render();
    sphere_actor->GetProperty()->GetDiffuseColor()[0] -= 0.001;
    sphere_actor->GetProperty()->GetDiffuseColor()[2] += 0.001;
    sphere_actor->GetProperty()->GetDiffuseColor()[2] -= 0.001;
    sphere_actor->GetProperty()->GetDiffuseColor()[1] += 0.001;
  }

  sphere_source->Delete();
  sphere_smapper->Delete();
  sphere_actor->Delete();
  render->Delete();
  wind->Delete();

  return 0;
}