#include "vtkActor.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"

#include "platform.h"

#include <filesystem>
#include <string>

auto main(int argc, char** argv) -> int {
  std::string file_name{
    fs::current_path().append("build/Data/42400-IDGH.stl").string()};

  vtkNew<vtkSTLReader> reader;
  reader->SetFileName(file_name.c_str());
  reader->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(reader->GetOutput());
  //   mapper->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->Initialize();
  interactor->Start();
}