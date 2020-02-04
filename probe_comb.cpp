#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkContourFilter.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkOutlineFilter.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProbeFilter.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridOutlineFilter.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"

#include "platform.h"
#include <filesystem>

int main() {
  const auto reader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/combxyz.bin").c_str());
  reader->SetQFileName((fs::canonical("Data").string() + "/combq.bin").c_str());
  reader->SetScalarFunctionNumber(100);
  reader->SetVectorFunctionNumber(202);
  reader->Update();
  const auto reader_out
    = dynamic_cast<vtkStructuredGrid*>(reader->GetOutput()->GetBlock(0));

  const auto plane = vtkSmartPointer<vtkPlaneSource>::New();
  plane->SetResolution(50, 50);

  const auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->Translate(3.7, 0.0, 28.37);
  transform->Scale(5, 5, 5);
  transform->RotateY(90);
  const auto filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  filter->SetInputConnection(plane->GetOutputPort());
  filter->SetTransform(transform);
  const auto outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outline->SetInputConnection(filter->GetOutputPort());
  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(outline->GetOutputPort());
  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0, 0, 0);

  const auto transform1 = vtkSmartPointer<vtkTransform>::New();
  transform1->Translate(9.2, 0.0, 31.2);
  transform1->Scale(5, 5, 5);
  transform1->RotateY(90);
  const auto filter1 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  filter1->SetInputConnection(plane->GetOutputPort());
  filter1->SetTransform(transform1);
  const auto outline1 = vtkSmartPointer<vtkOutlineFilter>::New();
  outline1->SetInputConnection(filter1->GetOutputPort());
  const auto mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper1->SetInputConnection(outline1->GetOutputPort());
  const auto actor1 = vtkSmartPointer<vtkActor>::New();
  actor1->SetMapper(mapper1);
  actor1->GetProperty()->SetColor(0, 0, 0);

  const auto transform2 = vtkSmartPointer<vtkTransform>::New();
  transform2->Translate(13.27, 0.0, 33.30);
  transform2->Scale(5, 5, 5);
  transform2->RotateY(90);
  const auto filter2 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  filter2->SetInputConnection(plane->GetOutputPort());
  filter2->SetTransform(transform2);
  const auto outline2 = vtkSmartPointer<vtkOutlineFilter>::New();
  outline2->SetInputConnection(filter2->GetOutputPort());
  const auto mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper2->SetInputConnection(outline2->GetOutputPort());
  const auto actor2 = vtkSmartPointer<vtkActor>::New();
  actor2->SetMapper(mapper2);
  actor2->GetProperty()->SetColor(0, 0, 0);

  const auto append = vtkSmartPointer<vtkAppendPolyData>::New();
  append->AddInputConnection(filter->GetOutputPort());
  append->AddInputConnection(filter1->GetOutputPort());
  append->AddInputConnection(filter2->GetOutputPort());

  const auto probe_filter = vtkSmartPointer<vtkProbeFilter>::New();
  probe_filter->SetInputConnection(append->GetOutputPort());
  probe_filter->SetSourceData(reader_out);

  const auto contour = vtkSmartPointer<vtkContourFilter>::New();
  contour->SetInputConnection(probe_filter->GetOutputPort());
  contour->GenerateValues(50, reader_out->GetScalarRange());
  const auto contour_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  contour_mapper->SetInputConnection(contour->GetOutputPort());
  contour_mapper->SetScalarRange(reader_out->GetScalarRange());

  const auto plane_acotr = vtkSmartPointer<vtkActor>::New();
  plane_acotr->SetMapper(contour_mapper);

  const auto outline_source
    = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline_source->SetInputData(reader_out);

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline_source->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->AddActor(actor1);
  render->AddActor(actor2);
  render->AddActor(outline_actor);
  render->AddActor(plane_acotr);
  render->SetBackground(1, 1, 1);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);
  render_window->Render();
  interactor->Initialize();
  interactor->Start();
}