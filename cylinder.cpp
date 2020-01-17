#include <vtkActor.h>
#include <vtkCylinderSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

auto main() -> int {
  const auto cylinder = vtkSmartPointer<vtkCylinderSource>::New();
  cylinder->SetResolution(100);

  auto cylinder_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  cylinder_mapper->SetInputConnection(cylinder->GetOutputPort());

  const auto cylinder_actor = vtkSmartPointer<vtkActor>::New();
  cylinder_actor->SetMapper(cylinder_mapper);

  const auto cylinder_render = vtkSmartPointer<vtkRenderer>::New();
  cylinder_render->AddActor(cylinder_actor);

  const auto cylinder_render_window = vtkSmartPointer<vtkRenderWindow>::New();
  cylinder_render_window->AddRenderer(cylinder_render);

  const auto cylinder_interactor_style
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  const auto cylinder_interator
    = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  cylinder_interator->SetRenderWindow(cylinder_render_window);
  cylinder_interator->SetInteractorStyle(cylinder_interactor_style);

  cylinder_render_window->Render();
  cylinder_interator->Initialize();
  cylinder_interator->Start();

  return 0;
}