#include <cmath>
#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkDoubleArray.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkStructuredPoints.h>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkRenderer> render;
  vtkNew<vtkRenderWindow> window;
  vtkNew<vtkRenderWindowInteractor> interactor;

  window->AddRenderer(render);
  interactor->SetRenderWindow(window);

  vtkNew<vtkStructuredPoints> volume;
  constexpr auto vol_dim = 64;
  constexpr auto spacing = 1.0 / (vol_dim - 1);
  volume->SetDimensions(vol_dim, vol_dim, vol_dim);
  volume->SetOrigin(0, 0, 0);
  volume->SetSpacing(spacing, spacing, spacing);

  vtkNew<vtkDoubleArray> scalars;
  scalars->SetNumberOfComponents(1);
  scalars->SetNumberOfTuples(vol_dim * vol_dim * vol_dim);

  for (auto k = 0; k < vol_dim; ++k) {
    auto z = -0.5 + k * spacing;
    auto k_offset = k * vol_dim * vol_dim;
    for (auto j = 0; j < vol_dim; ++j) {
      auto y = -0.5 + j * spacing;
      auto j_offset = j * vol_dim;
      for (auto i = 0; i < vol_dim; ++i) {
        auto x = -0.5 + i * spacing;
        auto index = k_offset + j_offset + i;
        auto value = std::sqrt(x * x + y * y + z * z);
        scalars->SetTuple1(index, value);
      }
    }
  }
  volume->GetPointData()->SetScalars(scalars);

  vtkNew<vtkContourFilter> contour;
  contour->SetInputData(volume);
  // contour->SetValue(0, -0.2);
  contour->SetValue(1, 0.6);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(contour->GetOutputPort());
  mapper->ScalarVisibilityOff();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
  actor->GetProperty()->EdgeVisibilityOn();
  actor->GetProperty()->SetEdgeColor(colors->GetColor3d("Banana").GetData());
  render->AddActor(actor);
  render->SetBackground(colors->GetColor3d("SlateGray").GetData());

  window->SetSize(640, 480);
  window->SetWindowName("ContourFilter");
  window->Render();

  interactor->Start();

  return EXIT_SUCCESS;
}