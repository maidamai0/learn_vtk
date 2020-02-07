#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkStructuredGridOutlineFilter.h"
#include "vtkWarpScalar.h"

#include "platform.h"
#include <filesystem>

int main() {
  vtkNew<vtkMultiBlockPLOT3DReader> reader;
  reader->SetFileName(
    (fs::canonical("Data").string() + "/combxyz.bin").c_str());
  reader->SetQFileName((fs::canonical("Data").string() + "/combq.bin").c_str());
  reader->SetScalarFunctionNumber(100);
  reader->SetVectorFunctionNumber(202);
  reader->Update();

  const auto reader_out
    = static_cast<vtkStructuredGrid*>(reader->GetOutput()->GetBlock(0));

  const auto create_plane = [&](const int i_min, const int i_max) {
    const auto plane = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
    plane->SetInputData(reader_out);
    plane->SetExtent(i_min, i_max, 1, 100, 1, 100);
    return plane;
  };

  const auto plane = create_plane(10, 10);
  const auto plane1 = create_plane(30, 30);
  const auto plane2 = create_plane(45, 45);

  vtkNew<vtkAppendPolyData> append;
  append->AddInputConnection(plane->GetOutputPort());
  append->AddInputConnection(plane1->GetOutputPort());
  append->AddInputConnection(plane2->GetOutputPort());

  vtkNew<vtkWarpScalar> warp;
  warp->SetInputConnection(append->GetOutputPort());
  warp->UseNormalOn();
  warp->SetNormal(1.0, 0.0, 0.0);
  warp->SetScaleFactor(2.5);

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(warp->GetOutputPort());
  normals->SetFeatureAngle(60);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(normals->GetOutputPort());
  mapper->SetScalarRange(reader_out->GetScalarRange());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkStructuredGridOutlineFilter> outline;
  outline->SetInputData(reader_out);

  vtkNew<vtkPolyDataMapper> outline_mapper;
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  vtkNew<vtkActor> outline_actor;
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->AddActor(outline_actor);
  render->SetBackground(1, 1, 1);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  window->Render();
  interactor->Start();
}