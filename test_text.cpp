#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLODActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"

int main() {
  const auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
  const auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

  const auto sphere_actor = vtkSmartPointer<vtkLODActor>::New();
  sphere_actor->SetMapper(sphere_mapper);

  const auto text_actor = vtkSmartPointer<vtkTextActor>::New();
  text_actor->SetTextScaleModeToProp();
  text_actor->SetDisplayPosition(90, 50);
  text_actor->SetInput("This is a sphere");

  const auto coor = text_actor->GetPosition2Coordinate();
  coor->SetCoordinateSystemToNormalizedViewport();
  coor->SetValue(0.6, 0.1);

  const auto text_property = text_actor->GetTextProperty();
  text_property->SetFontSize(18);
  text_property->SetFontFamilyToArial();
  text_property->SetJustificationToCentered();
  text_property->BoldOn();
  text_property->ItalicOn();
  text_property->ShadowOn();
  text_property->SetColor(0, 0, 1);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(sphere_actor);
  render->AddActor2D(text_actor);

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