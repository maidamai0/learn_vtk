#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkType.h"
#include <array>

#include "viewport_borders.hpp"

int main() {
  std::array<std::array<double, 3>, 8> vertices{{{0, 0, 0},
                                                 {1, 0, 0},
                                                 {1, 1, 0},
                                                 {0, 1, 0},
                                                 {0, 0, 1},
                                                 {1, 0, 1},
                                                 {1, 1, 1},
                                                 {0, 1, 1}}};

  std::array<std::array<vtkIdType, 4>, 6> vertices_ids{{{0, 1, 2, 3},
                                                        {4, 5, 6, 7},
                                                        {0, 1, 5, 4},
                                                        {1, 2, 6, 5},
                                                        {2, 3, 7, 6},
                                                        {3, 0, 4, 7}}};

  vtkNew<vtkPolyData> cube;
  vtkNew<vtkPoints> points;
  vtkNew<vtkCellArray> polys;
  vtkNew<vtkFloatArray> scalars;

  for (auto i = 0UL; i < vertices.size(); ++i) {
    points->InsertPoint(i, vertices[i].data());
    scalars->InsertTuple1(i, i);
  }

  for (auto&& i : vertices_ids) {
    polys->InsertNextCell(i.size(), i.data());
  }

  cube->SetPoints(points);
  cube->SetPolys(polys);
  cube->GetPointData()->SetScalars(scalars);

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(cube);
  mapper->SetScalarRange(cube->GetScalarRange());
  mapper->ScalarVisibilityOff();

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0.1, 0.2, 0.4);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);

  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(render);

  std::array<double, 3> red_color{1, 0, 0};
  viewport_borders(render, red_color.data(), true);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(window);

  interactor->Initialize();
  window->Render();
  interactor->Start();
}