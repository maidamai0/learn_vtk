#include "vtkActor.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkVertexGlyphFilter.h"

#include "fmt/core.h"
#include "platform.h"

#include <fstream>
#include <sstream>
#include <string>

auto main(int argc, char** argv) -> int {
  std::string file_name{
    fs::current_path().append("build/Data/mbwavelet_ascii.xyz").string()};

  std::ifstream file(file_name);
  if (!file.is_open()) {
    fmt::print("open {} failed\n", file_name);
    return 1;
  }

  std::string line;
  vtkNew<vtkPoints> points;
  double x = 0.0;
  double y = x;
  double z = x;
  std::stringstream ss;
  while (std::getline(file, line)) {
    ss << line;
    ss >> x >> y >> z;

    fmt::print("point:[{},{},{}]\n", x, y, z);

    points->InsertNextPoint(x, y, z);
    x = 0;
    y = 0;
    z = 0;
    ss.clear();
  }
  file.close();

  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points);

  vtkNew<vtkVertexGlyphFilter> filter;
  filter->SetInputData(polydata);
  filter->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(filter->GetOutputPort());

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