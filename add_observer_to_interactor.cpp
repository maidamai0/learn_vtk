#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include <sys/stat.h>

namespace {
auto rotating = 0;
auto panning = 0;
auto zooming = 0;
}; // namespace

void rotate(vtkSmartPointer<vtkRenderer> render, int x, int y, int last_x,
            int last_y, int cener_x, int center_y) {
}

void mouse_move(vtkSmartPointer<vtkRenderWindowInteractor> interacotr,
                vtkSmartPointer<vtkRenderWindow> render_window,
                vtkSmartPointer<vtkRenderer> render) {
  static auto last_pos = interacotr->GetLastEventPosition();
  auto last_x = last_pos[0];
  auto last_y = last_pos[1];

  const auto current_pos = interacotr->GetLastEventPosition();
  auto current_x = current_pos[0];
  auto current_y = current_pos[1];

  const auto center = render_window->GetSize();
  const auto center_x = center[0] / 2.0;
  const auto center_y = center[1] / 2.0;

  if (rotating) {
  }
}

void key_press() {
}

class MyCallBack : public vtkCommand {
public:
  static auto New() -> MyCallBack* {
    return new MyCallBack();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    switch (eventId) {
      case vtkCommand::LeftButtonPressEvent: {
        rotating = 1;
        break;
      }
      case vtkCommand::LeftButtonReleaseEvent: {
        rotating = 0;
        break;
      }
      case vtkCommand::MiddleButtonPressEvent: {
        panning = 1;
        break;
      }
      case vtkCommand::MiddleButtonReleaseEvent: {
        panning = 0;
        break;
      }
      case vtkCommand::RightButtonPressEvent: {
        zooming = 1;
        break;
      }
      case vtkCommand::RightButtonReleaseEvent: {
        zooming = 0;
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

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->SetBackground(0.1, 0.2, 0.4);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->SetSize(400, 400);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetInteractorStyle(nullptr);
  interactor->SetRenderWindow(render_window);

  const auto cmd = vtkSmartPointer<MyCallBack>::New();
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, cmd);

  return 0;
}