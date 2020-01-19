
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkCylinderSource.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"

int main() {
  const auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
  sphere_source->SetPhiResolution(100);
  sphere_source->SetThetaResolution(100);

  const auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

  const auto sphere_actor = vtkSmartPointer<vtkActor>::New();
  sphere_actor->SetMapper(sphere_mapper);
  sphere_actor->SetOrigin(2, 1, 3);
  sphere_actor->RotateY(6.0);
  sphere_actor->SetPosition(2.25, 0, 0);
  sphere_actor->GetProperty()->SetColor(1, 0, 1);

  const auto cube_source = vtkSmartPointer<vtkCubeSource>::New();

  const auto cube_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  cube_mapper->SetInputConnection(cube_source->GetOutputPort());

  const auto cube_actor = vtkSmartPointer<vtkActor>::New();
  cube_actor->SetMapper(cube_mapper);
  cube_actor->SetPosition(0.0, 0.25, 0.0);
  cube_actor->GetProperty()->SetColor(0, 0, 1);

  const auto cone_source = vtkSmartPointer<vtkConeSource>::New();
  cone_source->SetResolution(100);

  const auto cone_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  cone_mapper->SetInputConnection(cone_source->GetOutputPort());

  const auto cone_actor = vtkSmartPointer<vtkActor>::New();
  cone_actor->SetMapper(cone_mapper);
  cone_actor->GetProperty()->SetColor(0, 1, 0);

  const auto cylinder_source = vtkSmartPointer<vtkCylinderSource>::New();
  cylinder_source->SetResolution(100);

  const auto cylinder_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  cylinder_mapper->SetInputConnection(cylinder_source->GetOutputPort());

  const auto cylinder_actor = vtkSmartPointer<vtkActor>::New();
  cylinder_actor->SetMapper(cylinder_mapper);
  cylinder_actor->GetProperty()->SetColor(1, 0, 0);

  const auto assembly = vtkSmartPointer<vtkAssembly>::New();
  assembly->AddPart(sphere_actor);
  assembly->AddPart(cube_actor);
  assembly->AddPart(cylinder_actor);
  assembly->AddPart(cone_actor);
  assembly->SetOrigin(5, 10, 15);
  assembly->AddPosition(5, 0, 0);
  assembly->RotateX(15);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(assembly);
  render->AddActor(cone_actor);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  const auto interactor_style
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  interactor->SetInteractorStyle(interactor_style);
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}