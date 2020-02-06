#include "vtkActor.h"
#include "vtkFloatArray.h"
#include "vtkHedgeHog.h"
#include "vtkMath.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkStructuredGrid.h"

#include <array>

int main() {
  const auto r_min = 0.5;
  const auto r_max = 1.0;

  std::array<int, 3> dim{13, 11, 11};
  vtkNew<vtkStructuredGrid> structured_grid;
  structured_grid->SetDimensions(dim.data());

  vtkNew<vtkFloatArray> vectors;
  vectors->SetNumberOfComponents(3);
  vectors->SetNumberOfTuples(dim[0] * dim[1] * dim[2]);

  vtkNew<vtkPoints> points;
  points->Allocate(dim[0] * dim[1] * dim[2]);

  const auto delta_z = 2.0 / (dim[2] - 1);
  const auto delta_rad = (r_max - r_min) / (dim[1] - 1);

  std::array<float, 3> x;
  std::array<float, 3> v;
  v[2] = 0.0;

  for (auto k = 0; k < dim[2]; ++k) {
    x[2] = -1.0 + k * delta_z;
    int k_offset = k * dim[0] * dim[1];

    for (auto j = 0; j < dim[1]; ++j) {
      auto radius = r_min + j * delta_rad;
      int j_offset = j * dim[0];
      for (auto i = 0; i < dim[0]; ++i) {
        auto theta = i * vtkMath::RadiansFromDegrees(15.0);
        x[0] = radius * cos(theta);
        x[1] = radius * sin(theta);
        v[0] = -x[1];
        v[1] = x[0];

        auto offset = i + k_offset + j_offset;
        points->InsertPoint(offset, x.data());
        vectors->InsertTuple(offset, v.data());
      }
    }
  }

  structured_grid->SetPoints(points);
  structured_grid->GetPointData()->SetVectors(vectors);

  vtkNew<vtkHedgeHog> hedgehog;
  hedgehog->SetInputData(structured_grid);
  hedgehog->SetScaleFactor(0.1);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(hedgehog->GetOutputPort());

  vtkNew<vtkNamedColors> color;

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(color->GetColor3d("Indigo").GetData());

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(color->GetColor3d("Cornsilk").GetData());

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->Initialize();
  interactor->Start();
}