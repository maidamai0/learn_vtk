#include "vtkActor.h"
#include "vtkDataObject.h"
#include "vtkLineSource.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkRuledSurfaceFilter.h"
#include "vtkRungeKutta4.h"
#include "vtkSmartPointer.h"
#include "vtkStreamTracer.h"
#include "vtkStructuredGridOutlineFilter.h"

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

  const auto reader_out = reader->GetOutput()->GetBlock(0);

  const auto rake = vtkSmartPointer<vtkLineSource>::New();
  rake->SetPoint1(15, -5, 32);
  rake->SetPoint2(15, 5, 32);
  rake->SetResolution(21);

  const auto rake_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  rake_mapper->SetInputConnection(rake->GetOutputPort());

  const auto rake_actor = vtkSmartPointer<vtkActor>::New();
  rake_actor->SetMapper(rake_mapper);
  rake_actor->GetProperty()->SetColor(1, 0, 0);

  const auto integrate = vtkSmartPointer<vtkRungeKutta4>::New();

  const auto stream_tracer = vtkSmartPointer<vtkStreamTracer>::New();
  stream_tracer->SetInputData(reader_out);
  stream_tracer->SetSourceConnection(rake->GetOutputPort());
  stream_tracer->SetIntegrator(integrate);
  stream_tracer->SetMaximumPropagation(10);
  stream_tracer->SetInitialIntegrationStep(0.1);
  stream_tracer->SetIntegrationDirectionToBackward();

  // debug
  const auto stream_line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  stream_line_mapper->SetInputConnection(stream_tracer->GetOutputPort());

  const auto stream_line_actor = vtkSmartPointer<vtkActor>::New();
  stream_line_actor->SetMapper(stream_line_mapper);
  // debug

  const auto surface = vtkSmartPointer<vtkRuledSurfaceFilter>::New();
  surface->SetInputConnection(stream_tracer->GetOutputPort());
  surface->SetOffset(0);
  surface->SetOnRatio(2);
  surface->PassLinesOn();
  surface->SetRuledModeToPointWalk();
  surface->SetDistanceFactor(30);

  const auto surface_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  surface->SetInputConnection(surface->GetOutputPort());
  //   surface_mapper->SetScalarRange(0.197813, 0.710419);

  const auto surface_actor = vtkSmartPointer<vtkActor>::New();
  surface_actor->SetMapper(surface_mapper);

  const auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline->SetInputData(reader_out);

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(rake_actor);
  render->AddActor(surface_actor);
  render->AddActor(outline_actor);
  render->AddActor(stream_line_actor);
  render->SetBackground(1, 1, 1);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}