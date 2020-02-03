#include "platform.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCutter.h"
#include "vtkDataObject.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkPlane.h"
#include "vtkPointData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProbeFilter.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkStructuredGridOutlineFilter.h"
#include <filesystem>

int main() {
  const auto reader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/combxyz.bin").c_str());
  reader->SetQFileName((fs::canonical("data").string() + "/combq.bin").c_str());
  reader->SetScalarFunctionNumber(100);
  reader->SetVectorFunctionNumber(202);
  reader->Update();
  const auto reader_out
    = static_cast<vtkStructuredGrid*>(reader->GetOutput()->GetBlock(0));

  const auto plane = vtkSmartPointer<vtkPlane>::New();
  plane->SetOrigin(reader_out->GetCenter());
  plane->SetNormal(-0.287, 0, 0.9579);

  const auto plane_cut = vtkSmartPointer<vtkCutter>::New();
  plane_cut->SetInputData(reader_out);
  plane_cut->SetCutFunction(plane);

  const auto filter = vtkSmartPointer<vtkProbeFilter>::New();
  filter->SetInputConnection(plane_cut->GetOutputPort());
  filter->SetSourceData(reader_out);

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(filter->GetOutputPort());
  mapper->SetScalarRange(reader_out->GetPointData()->GetScalars()->GetRange());

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  const auto extract_plane
    = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
  extract_plane->SetInputData(reader_out);
  extract_plane->SetExtent(0, 100, 0, 100, 9, 9);

  const auto extract_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  extract_mapper->SetInputConnection(extract_plane->GetOutputPort());
  extract_mapper->ScalarVisibilityOff();

  const auto extract_actor = vtkSmartPointer<vtkActor>::New();
  extract_actor->SetMapper(extract_mapper);
  extract_actor->GetProperty()->SetRepresentationToWireframe();
  extract_actor->GetProperty()->SetColor(0, 0, 0);

  const auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline->SetInputData(reader_out);

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->AddActor(extract_actor);
  render->AddActor(outline_actor);
  render->SetBackground(1, 1, 1);

  const auto camera = render->GetActiveCamera();
  camera->SetClippingRange(11.1034, 59.5328);
  camera->SetFocalPoint(9.71821, 0.458166, 29.3999);
  camera->SetPosition(-2.95748, -26, 44);
  camera->SetViewUp(0.002, 0.5, 0.9);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render_window->Render();
  interactor->Initialize();
  interactor->Start();
}