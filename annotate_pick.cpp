#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLODActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"

#include <string>

namespace {
vtkSmartPointer<vtkCellPicker> cell_picker;
vtkSmartPointer<vtkActor2D> text_actor;
vtkSmartPointer<vtkTextMapper> text_mapper;
vtkSmartPointer<vtkRenderWindowInteractor> interactor;
vtkSmartPointer<vtkRenderer> render;
}; // namespace

class AnnotatePick : public vtkCommand {
public:
  static auto New() -> AnnotatePick* {
    return new AnnotatePick();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    switch (eventId) {
      case EndPickEvent: {
        handle_pick();
        break;
      }
      case vtkCommand::LeftButtonPressEvent: {
        handle_left_button_press();
        break;
      }
    }
  }

private:
  static void handle_pick() {
    if (cell_picker->GetCellId() < 0) {
      text_actor->VisibilityOff();
    } else {
      annotate();
    }
  }

  static void handle_left_button_press() {
    const auto pos = interactor->GetLastEventPosition();
    cell_picker->Pick(pos[0], pos[1], 0, render);
  }
  static void annotate() {
    const auto pick_pos = cell_picker->GetPickPosition();
    std::string text = std::to_string(pick_pos[0]) + ","
                       + std::to_string(pick_pos[1]) + ","
                       + std::to_string(pick_pos[2]);
    text_mapper->SetInput(text.c_str());

    const auto point = cell_picker->GetSelectionPoint();
    text_actor->SetPosition(point[0], point[1]);
    text_actor->VisibilityOn();
  }
};

auto main() -> int {
  const auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();

  const auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

  const auto sphere_actor = vtkSmartPointer<vtkActor>::New();
  sphere_actor->SetMapper(sphere_mapper);

  const auto glyph = vtkSmartPointer<vtkGlyph3D>::New();
  glyph->SetInputConnection(sphere_source->GetOutputPort());
  const auto cone_source = vtkSmartPointer<vtkConeSource>::New();
  glyph->SetSourceConnection(cone_source->GetOutputPort());
  glyph->SetVectorModeToUseNormal();
  glyph->SetScaleModeToScaleByVector();
  glyph->SetScaleFactor(0.25);

  const auto spiker_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  spiker_mapper->SetInputConnection(glyph->GetOutputPort());
  const auto spiker_actor = vtkSmartPointer<vtkLODActor>::New();
  spiker_actor->SetMapper(spiker_mapper);

  cell_picker = vtkSmartPointer<vtkCellPicker>::New();

  const auto annotate_pick = vtkSmartPointer<AnnotatePick>::New();
  cell_picker->AddObserver(vtkCommand::EndPickEvent, annotate_pick);

  text_mapper = vtkSmartPointer<vtkTextMapper>::New();
  const auto text_prop = text_mapper->GetTextProperty();
  text_prop->SetFontFamilyToArial();
  text_prop->SetFontSize(10);
  text_prop->BoldOn();
  text_prop->ShadowOn();
  text_prop->SetColor(1, 0, 0);

  text_actor = vtkSmartPointer<vtkActor2D>::New();
  text_actor->SetMapper(text_mapper);
  text_actor->VisibilityOff();

  render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor2D(text_actor);
  render->AddActor(sphere_actor);
  render->AddActor(spiker_actor);
  render->SetBackground(1, 1, 1);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->SetSize(400, 400);

  interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);
  const auto interactor_style
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  interactor->SetInteractorStyle(interactor_style);
  interactor->SetPicker(cell_picker);
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, annotate_pick);

  render->ResetCamera();
  render->GetActiveCamera()->Zoom(1.4);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}