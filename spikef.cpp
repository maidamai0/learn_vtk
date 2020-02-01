#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkMaskPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataReader.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"

#include "platform.h"
#include <filesystem>

int main() {
  const auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/fran_cut.vtk").c_str());

  const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(reader->GetOutputPort());
  normals->FlipNormalsOn();

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(normals->GetOutputPort());

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(1.0, 0.49, 0.25);

  const auto mask = vtkSmartPointer<vtkMaskPoints>::New();
  mask->SetInputConnection(normals->GetOutputPort());
  mask->SetOnRatio(10);
  mask->RandomModeOn();

  const auto cone = vtkSmartPointer<vtkConeSource>::New();
  cone->SetResolution(6);

  const auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->Translate(0.5, 0.0, 0.0);

  const auto transform_filter
    = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transform_filter->SetInputConnection(cone->GetOutputPort());
  transform_filter->SetTransform(transform);

  const auto glyph = vtkSmartPointer<vtkGlyph3D>::New();
  glyph->SetInputConnection(mask->GetOutputPort());
  glyph->SetSourceConnection(transform_filter->GetOutputPort());
  glyph->SetVectorModeToUseNormal();
  glyph->SetScaleModeToScaleByVector();
  glyph->SetScaleFactor(0.004);

  const auto spike_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  spike_mapper->SetInputConnection(glyph->GetOutputPort());

  const auto spike_actor = vtkSmartPointer<vtkActor>::New();
  spike_actor->SetMapper(spike_mapper);
  spike_actor->GetProperty()->SetColor(0.0, 0.79, 0.34);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->AddActor(spike_actor);
  render->SetBackground(0.1, 0.2, 0.4);
  render->ResetCamera();
  const auto camera = render->GetActiveCamera();
  camera->Zoom(1.4);
  camera->Azimuth(110);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}