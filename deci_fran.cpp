#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkDecimatePro.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataReader.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "platform.h"
#include <filesystem>

int main() {
  vtkNew<vtkPolyDataReader> reader;
  reader->SetFileName(
    (fs::canonical("Data").string() + "/fran_cut.vtk").c_str());

  vtkNew<vtkDecimatePro> decimate;
  decimate->SetInputConnection(reader->GetOutputPort());
  decimate->SetTargetReduction(0.9);
  decimate->PreserveTopologyOn();

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(reader->GetOutputPort());
  normals->FlipNormalsOn();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(normals->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(1, 1, 1);

  vtkNew<vtkCamera> camera;
  camera->SetClippingRange(0.04, 2.4);
  camera->SetFocalPoint(0.05, -0.129, -0.057);
  camera->SetPosition(0.33, -0.12, -0.26);
  camera->SetViewUp(-0.022, 0.999, 0.035);
  render->SetActiveCamera(camera);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  window->Render();
  interactor->Start();
}