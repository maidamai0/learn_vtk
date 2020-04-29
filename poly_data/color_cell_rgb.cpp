#include "vtkActor.h"
#include "vtkCellData.h"
#include "vtkMath.h"
#include "vtkNew.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkType.h"
#include "vtkUnsignedCharArray.h"

#include <array>

auto mian(int arc, char** argv) -> int {
  // create some geometry
  constexpr auto resolution_x = 5;
  constexpr auto resolution_y = 3;

  vtkNew<vtkPlaneSource> plane;
  plane->SetXResolution(resolution_x);
  plane->SetYResolution(resolution_y);
  plane->Update();

  // create some cell data
  vtkMath::RandomSeed(8775070);
  vtkNew<vtkUnsignedCharArray> cell_data;
  cell_data->SetNumberOfComponents(3);
  cell_data->SetNumberOfTuples(plane->GetOutput()->GetNumberOfCells());
  for (int i = 0; i < plane->GetOutput()->GetNumberOfCells(); ++i) {
    std::array<unsigned char, 3> rgb;
    rgb[0] = vtkMath::Random(64, 255);
    rgb[1] = vtkMath::Random(64, 255);
    rgb[2] = vtkMath::Random(64, 255);
    cell_data->InsertNextTypedTuple(rgb.data());
  }
  plane->GetOutput()->GetCellData()->SetScalars(cell_data);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(plane->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  window->Render();
  interactor->Start();

  return 0;
}