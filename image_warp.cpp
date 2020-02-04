#include "vtkActor.h"
#include "vtkBMPReader.h"
#include "vtkCamera.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkImageLuminance.h"
#include "vtkMergeFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkWarpScalar.h"

#include "platform.h"
#include <filesystem>

int main() {
  const auto reader = vtkSmartPointer<vtkBMPReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/masonry.bmp").c_str());

  const auto luminance = vtkSmartPointer<vtkImageLuminance>::New();
  luminance->SetInputConnection(reader->GetOutputPort());

  const auto geometry = vtkSmartPointer<vtkImageDataGeometryFilter>::New();
  geometry->SetInputConnection(luminance->GetOutputPort());

  const auto warp = vtkSmartPointer<vtkWarpScalar>::New();
  warp->SetInputConnection(geometry->GetOutputPort());
  warp->SetScaleFactor(-0.1);

  const auto merge = vtkSmartPointer<vtkMergeFilter>::New();
  merge->SetGeometryConnection(warp->GetOutputPort());
  merge->SetScalarsConnection(reader->GetOutputPort());

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(merge->GetOutputPort());
  mapper->SetScalarRange(0, 255);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->ResetCamera();
  render->GetActiveCamera()->Azimuth(20);
  render->GetActiveCamera()->Elevation(30);
  render->SetBackground(0.1, 0.2, 0.4);
  render->ResetCameraClippingRange();

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);
  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}