#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkDataArray.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkImageData.h"
#include "vtkImageSinusoidSource.h"
#include "vtkLookupTable.h"
#include "vtkMapper.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

#include "platform.h"
#include <iostream>

int main() {
  const auto image_sinusoid = vtkSmartPointer<vtkImageSinusoidSource>::New();
  image_sinusoid->SetWholeExtent(0, 9, 0, 9, 0, 9);
  image_sinusoid->SetPeriod(5);
  image_sinusoid->Update();

  const auto image = image_sinusoid->GetOutput();
  double range[2] = {0.0};
  image->GetScalarRange(range);

  const auto filter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
  filter->SetInputConnection(image_sinusoid->GetOutputPort());

  const auto image_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  image_mapper->SetInputConnection(filter->GetOutputPort());
  image_mapper->SetScalarVisibility(VTK_TRUE);

  const auto lookup_table = vtkSmartPointer<vtkLookupTable>::New();
  lookup_table->SetTableRange(range);
  lookup_table->SetAlphaRange(0.5, 0.5);
  lookup_table->SetHueRange(0.3, 0.7);
  lookup_table->SetNumberOfTableValues(256);
  lookup_table->Build();

  image_mapper->SetLookupTable(lookup_table);

  const auto image_actor = vtkSmartPointer<vtkActor>::New();
  image_actor->SetMapper(image_mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->SetUseDepthPeeling(VTK_TRUE);
  render->SetMaximumNumberOfPeels(200);
  render->SetOcclusionRatio(0.1);
  render->AddActor(image_actor);
  render->SetBackground(0.1, 0.3, 0.0);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->SetMultiSamples(0); // disable multisample
  render_window->SetAlphaBitPlanes(VTK_TRUE);
  render_window->AddRenderer(render);
  render_window->Render();

  if (render->GetLastRenderingUsedDepthPeeling()) {
    std::cout << "depth pelling is used\n";
  } else {
    std::cout
      << "depth pelliing is not used(alpha blending is used instead.)\n";
  }

  const auto camera = render->GetActiveCamera();
  camera->Azimuth(-40);
  camera->Elevation(20);
  render->Render();

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  interactor->Start();
}