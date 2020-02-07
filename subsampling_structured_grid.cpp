#include "vtkActor.h"
#include "vtkExtractGrid.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridOutlineFilter.h"

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

  const auto reader_out = (reader->GetOutput()->GetBlock(0));

  vtkNew<vtkExtractGrid> extract;
  extract->SetInputData(reader_out);
  extract->SetVOI(30, 30, -1000, 1000, -1000, 1000);
  extract->SetSampleRate(1, 2, 3);
  extract->IncludeBoundaryOn();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(extract->GetOutputPort());
  mapper->SetScalarRange(0.18, 0.7);

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

  vtkNew<vtkRenderWindowInteractor> interacotr;
  interacotr->SetRenderWindow(window);
  window->Render();
  interacotr->Initialize();
  interacotr->Start();
}