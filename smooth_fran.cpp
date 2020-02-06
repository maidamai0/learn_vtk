#include "vtkActor.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataReader.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmoothPolyDataFilter.h"

#include "platform.h"
#include <filesystem>

int main() {
  vtkNew<vtkPolyDataReader> reader;
  reader->SetFileName(
    (fs::canonical("Data").string() + "/fran_cut.vtk").c_str());

  vtkNew<vtkSmoothPolyDataFilter> smooth;
  smooth->SetInputConnection(reader->GetOutputPort());
  smooth->SetNumberOfIterations(100);

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(smooth->GetOutputPort());
  normals->FlipNormalsOn();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(normals->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkPolyDataMapper> original_mapper;
  original_mapper->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkActor> original_actor;
  original_actor->SetMapper(original_mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(0.1, 0.2, 0.4);
  render->SetViewport(0.0, 0.0, 0.5, 1.0);

  vtkNew<vtkRenderer> original_render;
  original_render->AddActor(original_actor);
  original_render->SetBackground(1, 1, 1);
  original_render->SetViewport(0.5, 0.0, 1.0, 1.0);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);
  window->AddRenderer(original_render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  window->Render();
  interactor->Start();
}