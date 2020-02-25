#include "vtkActor.h"
#include "vtkNew.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include "vtkSTLWriter.h"
#include "vtkSphereSource.h"

#include "platform.h"

#include <string>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkSphereSource> sphere;

  const std::string file_name{"stl_sphere_test.stl"};
  vtkNew<vtkSTLWriter> stl_writer;
  stl_writer->SetFileName(file_name.c_str());
  stl_writer->SetInputConnection(sphere->GetOutputPort());
  stl_writer->Write();

  vtkNew<vtkSTLReader> reader;
  reader->SetFileName(file_name.c_str());
  reader->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(reader->GetOutput());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interacotr;
  interacotr->SetRenderWindow(window);
  window->Render();
  interacotr->Initialize();
  interacotr->Start();

  fs::remove(file_name);
}