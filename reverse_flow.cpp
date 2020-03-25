#include <vtkActor.h>
#include <vtkAlgorithmOutput.h>
#include <vtkConeSource.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkConeSource> cone;
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(cone->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->SetViewport(0.0, 0.0, 0.5, 1.0);
  render->AddActor(actor);

  auto ref_cone = actor->GetMapper()->GetInputConnection(0, 0)->GetProducer();
  dynamic_cast<vtkConeSource*>(ref_cone)->SetResolution(50);

  vtkNew<vtkPolyDataMapper> ref_mapper;
  ref_mapper->SetInputConnection(ref_cone->GetOutputPort());

  vtkNew<vtkActor> ref_actor;
  ref_actor->SetMapper(ref_mapper);

  vtkNew<vtkRenderer> ref_render;
  ref_render->AddActor(ref_actor);
  ref_render->SetViewport(0.5, 0.0, 1.0, 1.0);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);
  window->AddRenderer(ref_render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  window->Render();
  interactor->Start();
}