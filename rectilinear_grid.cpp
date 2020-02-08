#include "vtkActor.h"
#include "vtkDataSetAlgorithm.h"
#include "vtkFloatArray.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRectilinearGrid.h"
#include "vtkRectilinearGridGeometryFilter.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include <array>

int main() {
  std::array<double, 47> x{
    {-1.22396,  -1.17188,  -1.11979,  -1.06771,  -1.01562,  -0.963542,
     -0.911458, -0.859375, -0.807292, -0.755208, -0.703125, -0.651042,
     -0.598958, -0.546875, -0.494792, -0.442708, -0.390625, -0.338542,
     -0.286458, -0.234375, -0.182292, -0.130209, -0.078125, -0.026042,
     0.0260415, 0.078125,  0.130208,  0.182291,  0.234375,  0.286458,
     0.338542,  0.390625,  0.442708,  0.494792,  0.546875,  0.598958,
     0.651042,  0.703125,  0.755208,  0.807292,  0.859375,  0.911458,
     0.963542,  1.01562,   1.06771,   1.11979,   1.17188}};

  std::array<double, 33> y = {
    {-1.25,     -1.17188,  -1.09375,  -1.01562, -0.9375,   -0.859375, -0.78125,
     -0.703125, -0.625,    -0.546875, -0.46875, -0.390625, -0.3125,   -0.234375,
     -0.15625,  -0.078125, 0,         0.078125, 0.15625,   0.234375,  0.3125,
     0.390625,  0.46875,   0.546875,  0.625,    0.703125,  0.78125,   0.859375,
     0.9375,    1.01562,   1.09375,   1.17188,  1.25}};

  std::array<double, 44> z
    = {{0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.75, 0.8, 0.9,
        1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.75, 1.8, 1.9,
        2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.75, 2.8, 2.9,
        3, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.75, 3.8, 3.9}};

  vtkNew<vtkFloatArray> x_coordinates;
  for (auto& i : x) {
    x_coordinates->InsertNextValue(i);
  }
  vtkNew<vtkFloatArray> y_coordinates;
  for (auto& i : y) {
    y_coordinates->InsertNextValue(i);
  }

  vtkNew<vtkFloatArray> z_coordinates;
  for (auto& i : z) {
    z_coordinates->InsertNextValue(i);
  }

  vtkNew<vtkRectilinearGrid> rectilinear_grid;
  rectilinear_grid->SetDimensions(static_cast<int>(x.size()),
                                  static_cast<int>(y.size()),
                                  static_cast<int>(z.size()));
  rectilinear_grid->SetXCoordinates(x_coordinates);
  rectilinear_grid->SetYCoordinates(y_coordinates);
  rectilinear_grid->SetZCoordinates(z_coordinates);

  vtkNew<vtkRectilinearGridGeometryFilter> plane;
  plane->SetInputData(rectilinear_grid);
  plane->SetExtent(0, 46, 16, 16, 0, 43);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(plane->GetOutputPort());

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetRepresentationToWireframe();
  actor->GetProperty()->SetColor(colors->GetColor3d("Indigo").GetData());

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(colors->GetColor3d("Cornsilk").GetData());

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  window->Render();
  interactor->Start();
}