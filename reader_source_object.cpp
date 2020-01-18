#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLODActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include "vtkSmartPointer.h"

#include "platform.h"

#include <string>

namespace {
const std::string data_root = "Data/";
} // namespace

auto main() -> int {
  const auto reader = vtkSmartPointer<vtkSTLReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/42400-IDGH.stl").c_str());

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());

  const auto actor = vtkSmartPointer<vtkLODActor>::New();
  actor->SetMapper(mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);

  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(render);

  const auto interactor_style
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetInteractorStyle(interactor_style);
  interactor->SetRenderWindow(window);

  window->Render();
  interactor->Initialize();
  interactor->Start();
  return 0;
}