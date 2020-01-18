#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLODActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include "vtkShrinkPolyData.h"
#include "vtkSmartPointer.h"

#include "platform.h"
#include <filesystem>

static constexpr auto shrink_factor = 0.8;

int main() {
  const auto reader = vtkSmartPointer<vtkSTLReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/42400-IDGH.stl").c_str());

  const auto shrink_filter = vtkSmartPointer<vtkShrinkPolyData>::New();
  shrink_filter->SetInputConnection(reader->GetOutputPort());
  shrink_filter->SetShrinkFactor(shrink_factor);

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(shrink_filter->GetOutputPort());

  const auto actor = vtkSmartPointer<vtkLODActor>::New();
  actor->SetMapper(mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor_style
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetInteractorStyle(interactor_style);
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}