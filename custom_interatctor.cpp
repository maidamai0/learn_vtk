#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>

/**
 * @brief golobal variables.
 *
 */
namespace {
auto rotating = false;
auto panning = false;

int last_x = 0;
int last_y = 0;

constexpr double zoom_out_factor = 1.1;
constexpr double zoom_in_factor = 0.9;

vtkSmartPointer<vtkRenderWindowInteractor> interactor;
vtkSmartPointer<vtkRenderWindow> render_window;
vtkSmartPointer<vtkRenderer> render;

}; // namespace

/**
 * @brief rotate view
 *
 * @param x             current x position of cursor
 * @param y             current y position of cursor
 * @param last_x        last x postion of cursor
 * @param last_y        last y position of cursor
 * @param center_x      center x of window
 * @param center_y      center y of window
 */
void rotate(int x, int y, int last_x, int last_y, int center_x, int center_y) {
  auto camera = render->GetActiveCamera();
  camera->Azimuth(last_x - x);
  camera->Elevation(last_y - y);
  camera->OrthogonalizeViewUp();

  render_window->Render();
}

/**
 * @brief translate view
 *
 * @param x             current x position of cursor
 * @param y             current y position of cursor
 * @param last_x        last x postion of cursor
 * @param last_y        last y position of cursor
 * @param center_x      center x of window
 * @param center_y      center y of window
 */
void pan(int x, int y, int last_x, int last_y, int center_x, int center_y) {
  const auto camera = render->GetActiveCamera();
  const auto focal = camera->GetFocalPoint();
  const auto focal_x = focal[0];
  const auto focal_y = focal[1];
  const auto focal_z = focal[2];

  render->SetWorldPoint(focal_x, focal_y, focal_z, 1.0);
  render->WorldToDisplay();

  const auto display = render->GetDisplayPoint();
  const auto focal_depth = display[2];
  const auto display_x = center_x + (x - last_x);
  const auto display_y = center_y + (y - last_y);
  render->SetDisplayPoint(display_x, display_y, focal_depth);
  render->DisplayToWorld();

  const auto world = render->GetWorldPoint();
  auto world_x = world[0];
  auto world_y = world[1];
  auto world_z = world[2];
  auto world_a = world[3];
  if (world_a != 0.0) {
    world_x /= world_a;
    world_y /= world_a;
    world_z /= world_a;
  }

  camera->SetFocalPoint((focal_x - world_x) / 2.0 + focal_x,
                        (focal_y - world_y) / 2.0 + focal_y,
                        (focal_z - world_z) / 2.0 + focal_z);

  const auto position = camera->GetPosition();

  camera->SetPosition((focal_x - world_x) / 2.0 + position[0],
                      (focal_y - world_y) / 2.0 + position[1],
                      (focal_z - world_z) / 2.0 + position[2]);

  render_window->Render();
}

/**
 * @brief zoom out
 *
 */
void zoom_on_mouse_wheel_forward() {
  render->GetActiveCamera()->Dolly(zoom_out_factor);
  render->ResetCameraClippingRange();
  render_window->Render();
}

/**
 * @brief zoom in
 *
 */
void zoom_on_mouse_wheel_backward() {
  render->GetActiveCamera()->Dolly(zoom_in_factor);
  render->ResetCameraClippingRange();
  render_window->Render();
}

/**
 * @brief handle mouse move event
 *
 */
void on_mouse_move() {
  const auto current_pos = interactor->GetLastEventPosition();
  auto current_x = current_pos[0];
  auto current_y = current_pos[1];

  const auto center = render_window->GetSize();
  const auto center_x = center[0] / 2.0;
  const auto center_y = center[1] / 2.0;

  std::cout << "last postion:" << last_x << "," << last_y << std::endl;
  std::cout << "current position:" << current_x << "," << current_y
            << std::endl;
  std::cout << "center position:" << center_x << "," << center_y << "\n"
            << std::endl;
  if (rotating) {
    rotate(current_x, current_y, last_x, last_y, center_x, center_y);
  } else if (panning) {
    pan(current_x, current_y, last_x, last_y, center_x, center_y);
  }

  last_x = current_x;
  last_y = current_y;
}

/**
 * @brief show wire frame
 *
 */
void wire_frame() {
  const auto actors = render->GetActors();
  actors->InitTraversal();
  auto actor = actors->GetNextActor();

  while (actor) {
    actor->GetProperty()->SetRepresentationToWireframe();
    actor = actors->GetNextActor();
  }

  render_window->Render();
}

/**
 * @brief show surface of object
 *
 */
void surface() {
  const auto actors = render->GetActors();
  actors->InitTraversal();
  auto actor = actors->GetNextActor();

  while (actor) {
    actor->GetProperty()->SetRepresentationToSurface();
    actor = actors->GetNextActor();
  }

  render_window->Render();
}

/**
 * @brief handle key press event
 *
 */
void on_key_press() {
  const std::string key = interactor->GetKeySym();

  if (key == "e") {
    // TODO quit event is not implemented.
    interactor->InvokeEvent(vtkCommand::DeleteEvent);
  } else if (key == "w") {
    wire_frame();
  } else if (key == "s") {
    surface();
  }
}

/**
 * @brief command callback to handle mouse and key event
 *
 */
class ButtonPressCallback : public vtkCommand {
public:
  static auto New() -> ButtonPressCallback* {
    return new ButtonPressCallback();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    switch (eventId) {
      case vtkCommand::LeftButtonPressEvent: {
        rotating = true;
        break;
      }
      case vtkCommand::LeftButtonReleaseEvent: {
        rotating = false;
        break;
      }
      case vtkCommand::RightButtonPressEvent: {
        panning = true;
        break;
      }
      case vtkCommand::RightButtonReleaseEvent: {
        panning = false;
        break;
      }
      case vtkCommand::MouseWheelBackwardEvent: {
        zoom_on_mouse_wheel_backward();
        break;
      }
      case vtkCommand::MouseWheelForwardEvent: {
        zoom_on_mouse_wheel_forward();
        break;
      }
      case vtkCommand::MouseMoveEvent: {
        on_mouse_move();
        break;
      }
      case vtkCommand::KeyPressEvent: {
        on_key_press();
        break;
      }
    }
  }
};

auto main() -> int {
  const auto source = vtkSmartPointer<vtkConeSource>::New();
  source->SetHeight(3.0);
  source->SetRadius(1.0);
  source->SetResolution(100);

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(source->GetOutputPort());

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->SetBackground(0.1, 0.2, 0.4);

  render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->SetSize(400, 400);

  interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetInteractorStyle(nullptr);
  interactor->SetRenderWindow(render_window);

  const auto cmd_callback = vtkSmartPointer<ButtonPressCallback>::New();
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::RightButtonPressEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::RightButtonReleaseEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::MouseWheelForwardEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::MouseWheelBackwardEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::MouseMoveEvent, cmd_callback);
  interactor->AddObserver(vtkCommand::KeyPressEvent, cmd_callback);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();

  return 0;
}