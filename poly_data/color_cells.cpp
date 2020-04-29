#include "fmt/core.h"
#include "vtkActor.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkLookupTable.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkPlaneSource> plane;
  constexpr auto resolution = 3;
  plane->SetXResolution(resolution);
  plane->SetYResolution(resolution);

  vtkNew<vtkFloatArray> cell_data;
  for (auto i = 0; i < resolution; ++i) {
    cell_data->InsertNextValue(i + 1);
  }

  vtkNew<vtkLookupTable> lookup_table;
  constexpr auto table_size = std::max(resolution * resolution + 1, 10);
  lookup_table->SetNumberOfTableValues(table_size);
  lookup_table->Build();

  vtkNew<vtkNamedColors> named_colors;
  auto index = 0;
  for (const auto name : {"Black", "Banana", "Tomato", "Wheat", "Lavender",
                          "Flesh", "Raspberry", "Salmon", "Mint", "Peacock"}) {
    fmt::print("{} color is {}\n", index, name);
    lookup_table->SetTableValue(index++,
                                named_colors->GetColor4d(name).GetData());
  };
  std::flush(std::cout);

  plane->Update();
  plane->GetOutput()->GetCellData()->SetScalars(cell_data);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(plane->GetOutputPort());
  mapper->SetScalarRange(0, table_size - 1);
  mapper->SetLookupTable(lookup_table);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->SetBackground(named_colors->GetColor3d("black").GetData());
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  window->Render();
  interactor->Start();
}