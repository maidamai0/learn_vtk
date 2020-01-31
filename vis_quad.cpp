#include "vtkActor.h"
#include "vtkContourFilter.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkQuadric.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSampleFunction.h"
#include "vtkSmartPointer.h"

int main() {
  const auto quadric = vtkSmartPointer<vtkQuadric>::New();
  quadric->SetCoefficients(0.5, 1, 0.2, 0, 0.1, 0, 0, 0.2, 0, 0);

  const auto sample = vtkSmartPointer<vtkSampleFunction>::New();
  sample->SetSampleDimensions(30, 30, 30);
  sample->SetImplicitFunction(quadric);

  const auto contour = vtkSmartPointer<vtkContourFilter>::New();
  contour->SetInputConnection(sample->GetOutputPort());
  contour->GenerateValues(10, 0.0, 1.5);

  const auto contour_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  contour_mapper->SetInputConnection(contour->GetOutputPort());
  contour_mapper->SetScalarRange(0.0, 1.1);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(contour_mapper);

  const auto outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outline->SetInputConnection(sample->GetOutputPort());

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->AddActor(outline_actor);
  render->SetBackground(1, 1, 1);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render_window->Render();
  interactor->Initialize();
  interactor->Start();
}