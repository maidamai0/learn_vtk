#include "vtkABI.h"
#include "vtkActor.h"
#include "vtkBMPReader.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTexture.h"

#include "platform.h"

int main() {
  const auto bmp_reader = vtkSmartPointer<vtkBMPReader>::New();
  bmp_reader->SetFileName(
    (fs::canonical("Data").string() + "/masonry.bmp").c_str());

  const auto texture = vtkSmartPointer<vtkTexture>::New();
  texture->SetInputConnection(bmp_reader->GetOutputPort());
  texture->SetInterpolate(VTK_TRUE);

  const auto plane_source = vtkSmartPointer<vtkPlaneSource>::New();

  const auto plane_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  plane_mapper->SetInputConnection(plane_source->GetOutputPort());

  const auto plane_actor = vtkSmartPointer<vtkActor>::New();
  plane_actor->SetMapper(plane_mapper);
  plane_actor->SetTexture(texture);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(plane_actor);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  const auto interactor_style
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  interactor->SetInteractorStyle(interactor_style);
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}