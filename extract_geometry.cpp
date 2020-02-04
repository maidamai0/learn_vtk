#include "vtkActor.h"
#include "vtkDataSetMapper.h"
#include "vtkExtractGeometry.h"
#include "vtkImplicitBoolean.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkQuadric.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSampleFunction.h"
#include "vtkShrinkFilter.h"
#include "vtkSmartPointer.h"
#include "vtkSphere.h"
#include "vtkTransform.h"

int main() {
  const auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->Scale(1, 0.5, 0.333);
  const auto sphere = vtkSmartPointer<vtkSphere>::New();
  sphere->SetRadius(0.25);
  sphere->SetTransform(transform);

  const auto transform1 = vtkSmartPointer<vtkTransform>::New();
  transform1->Scale(0.25, 0.5, 1.0);
  const auto sphere1 = vtkSmartPointer<vtkSphere>::New();
  sphere1->SetRadius(0.25);
  sphere1->SetTransform(transform1);

  const auto implicit_function = vtkSmartPointer<vtkImplicitBoolean>::New();
  implicit_function->AddFunction(sphere);
  implicit_function->AddFunction(sphere1);
  implicit_function->SetOperationType(0);

  const auto quadric = vtkSmartPointer<vtkQuadric>::New();
  quadric->SetCoefficients(0.5, 1, 0.2, 0, 0.1, 0, 0, 0.2, 0, 0);
  const auto sample = vtkSmartPointer<vtkSampleFunction>::New();
  sample->SetSampleDimensions(50, 50, 50);
  sample->SetImplicitFunction(quadric);

  const auto extract = vtkSmartPointer<vtkExtractGeometry>::New();
  extract->SetInputConnection(sample->GetOutputPort());
  extract->SetImplicitFunction(implicit_function);
  const auto shrink = vtkSmartPointer<vtkShrinkFilter>::New();
  shrink->SetInputConnection(extract->GetOutputPort());
  shrink->SetShrinkFactor(0.5);

  const auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(shrink->GetOutputPort());
  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

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

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}