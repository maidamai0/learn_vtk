#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"

class TimerCallback final : public vtkCommand {
public:
  TimerCallback(vtkSmartPointer<vtkActor> actor) : actor_{std::move(actor)} {};
  TimerCallback() = default;
  ~TimerCallback() final = default;

  static auto New() {
    return new TimerCallback();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    auto interactor = dynamic_cast<vtkRenderWindowInteractor*>(caller);
    switch (eventId) {
      case vtkCommand::TimerEvent:
        ++timer_cnt_;

        if (timer_cnt_ < 36) {
          actor_->RotateZ(5);
          interactor->GetRenderWindow()->Render();
        } else {
          interactor->DestroyTimer();
        }
    }
  }

private:
  std::size_t timer_cnt_ = 0;
  vtkSmartPointer<vtkActor> actor_;
};

auto main(int argc, char** argv) -> int {
  vtkNew<vtkSphereSource> sphere_source;
  sphere_source->SetCenter(0.0, 0.0, 0.0);
  sphere_source->SetRadius(1.0);
  sphere_source->SetThetaResolution(30);
  sphere_source->SetPhiResolution(30);
  //   sphere_source->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(sphere_source->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->RotateX(90);
  actor->GetProperty()->SetRepresentationToWireframe();

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  vtkNew<vtkNamedColors> colors;
  actor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());
  render->SetBackground(colors->GetColor4d("Seashell").GetData());

  auto callback = new TimerCallback(actor);
  interactor->AddObserver(vtkCommand::TimerEvent, callback);
  interactor->CreateRepeatingTimer(100);

  window->Render();
  interactor->Start();
}