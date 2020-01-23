#include "vtkActor.h"
#include "vtkAxes.h"
#include "vtkCamera.h"
#include "vtkFollower.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkVectorText.h"
#include "vtkfollower.h"
#include "vtkvectortext.h"
#include <iostream>

int main() {
  const auto axes = vtkSmartPointer<vtkAxes>::New();
  axes->SetOrigin(0, 0, 0);

  const auto axes_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  axes_mapper->SetInputConnection(axes->GetOutputPort());

  const auto axes_actor = vtkSmartPointer<vtkActor>::New();
  axes_actor->SetMapper(axes_mapper);

  const auto text = vtkSmartPointer<vtkVectorText>::New();
  text->SetText("Origin");

  const auto text_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  text_mapper->SetInputConnection(text->GetOutputPort());

  const auto text_actor = vtkSmartPointer<vtkFollower>::New();
  text_actor->SetMapper(text_mapper);
  text_actor->SetScale(0.2, 0.2, 0.2);
  text_actor->AddPosition(0, -0.1, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(text_actor);
  render->AddActor(axes_actor);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render->ResetCamera();
  render->GetActiveCamera()->Zoom(1.6);
  render->ResetCameraClippingRange();

  text_actor->SetCamera(render->GetActiveCamera());

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}