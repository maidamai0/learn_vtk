
#include "vtkActor.h"
#include "vtkContourFilter.h"
#include "vtkLODActor.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"

#include "platform.h"
#include "vtkStructuredGridOutlineFilter.h"
#include <filesystem>

int main() {
  const auto reader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/combxyz.bin").c_str());
  reader->SetQFileName((fs::canonical("Data").string() + "/combq.bin").c_str());
  reader->SetScalarFunctionNumber(100);
  reader->SetVectorFunctionNumber(202);
  reader->AddFunction(153);
  reader->Update();
  reader->DebugOn();
  const auto reader_out
    = static_cast<vtkStructuredGrid*>(reader->GetOutput()->GetBlock(0));

  const auto iso = vtkSmartPointer<vtkContourFilter>::New();
  iso->SetInputData(reader_out);
  iso->SetValue(0, 0.24);

  const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(iso->GetOutputPort());
  normals->SetFeatureAngle(45);

  const auto iso_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  iso_mapper->SetInputConnection(normals->GetOutputPort());
  iso_mapper->ScalarVisibilityOn();
  iso_mapper->SetScalarRange(0, 1500);
  iso_mapper->SetScalarModeToUsePointFieldData();
  iso_mapper->ColorByArrayComponent("VelocityMagnitude", 0);

  const auto iso_actor = vtkSmartPointer<vtkLODActor>::New();
  iso_actor->SetMapper(iso_mapper);
  iso_actor->SetNumberOfCloudPoints(1000);

  const auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline->SetInputData(reader_out);
  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());
  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(outline_actor);
  render->AddActor(iso_actor);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);
  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}