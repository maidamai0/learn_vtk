#include "fmt/core.h"

#include "vtkBYUReader.h"
#include "vtkOBJReader.h"
#include "vtkPLYReader.h"
#include "vtkPolyDataReader.h"
#include "vtkSTLReader.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTimerLog.h"
#include "vtksys/SystemTools.hxx"

#include <algorithm>
#include <array>
#include <filesystem>
#include <random>
#include <string>
#include <xutility>

#include "platform.h"

template <typename READER>
auto get_polydata(const READER& reader, std::string&& file_name)
  -> vtkSmartPointer<vtkPolyData> {
  reader->SetFileName(file_name.c_str());
  return reader->GetOutput();
}

auto read_polydata(std::string file_name) -> vtkSmartPointer<vtkPolyData> {
  auto dot = file_name.find_last_of('.');
  if (dot == std::string::npos) {
    fmt::print("Not recognised file:{}", file_name);
    return nullptr;
  }

  const auto ext = file_name.substr(dot);
  vtkSmartPointer<vtkPolyData> polydata;
  if (ext == ".ply") {
    vtkNew<vtkPLYReader> reader;
    polydata = get_polydata(reader, std::move(file_name));
  } else if (ext == ".vtp") {
    vtkNew<vtkXMLPolyDataReader> reader;
    polydata = get_polydata(reader, std::move(file_name));
  } else if (ext == ".obj") {
    vtkNew<vtkOBJReader> reader;
    polydata = get_polydata(reader, std::move(file_name));
  } else if (ext == ".stl") {
    vtkNew<vtkSTLReader> reader;
    polydata = get_polydata(reader, std::move(file_name));
  } else if (ext == ".vtk") {
    vtkNew<vtkPolyDataReader> reader;
    polydata = get_polydata(reader, std::move(file_name));
  } else if (ext == ".g") {
    vtkNew<vtkBYUReader> reader;
    reader->SetGeometryFileName(file_name.c_str());
    reader->Update();
    polydata = reader->GetOutput();
  } else {
    vtkNew<vtkSphereSource> source;
    source->Update();
    polydata = source->GetOutput();
  }

  return polydata;
}

auto str = R"(fsfs)";

auto main(int argc, char** argv) -> int {
  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkRenderer> render;
  render->SetBackground(colors->GetColor3d("Wheat").GetData());
  render->UseHiddenLineRemovalOn();

  std::mt19937 mt(time(nullptr));
  std::uniform_real_distribution<double> distribution(0.6, 1.0);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);

  std::string file_name = "test.sphere";
  if (argc > 1) {
    file_name = argv[1];
  }

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(read_polydata(file_name));

  std::array<double, 3> random_color;
  random_color[0] = distribution(mt);
  random_color[1] = distribution(mt);
  random_color[2] = distribution(mt);

  vtkNew<vtkProperty> back_property;
  //   back_property->SetDiffuseColor(colors->GetColor3d("Banana").GetData());
  back_property->SetSpecular(0.6);
  back_property->SetSpecularPower(30);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->SetBackfaceProperty(back_property);
  actor->GetProperty()->SetDiffuseColor(random_color.data());
  actor->GetProperty()->SetSpecular(0.3);
  actor->GetProperty()->SetSpecularPower(30);
  render->AddActor(actor);

  window->Render();
  interactor->Initialize();
  interactor->Start();
}