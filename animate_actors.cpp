#include <array>
#include <vector>

#include "vtkActor.h"
#include "vtkAnimationCue.h"
#include "vtkAnimationScene.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

class AnimationSceneObserver final : public vtkCommand {
public:
  static auto New() -> AnimationSceneObserver* {
    return new AnimationSceneObserver();
  }

  void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> window) {
    window_ = std::move(window);
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
    switch (eventId) {
      case vtkCommand::AnimationCueTickEvent:
        window_->Render();
        break;
    }
  }

private:
  AnimationSceneObserver() = default;
  ~AnimationSceneObserver() final = default;

  vtkSmartPointer<vtkRenderWindow> window_;
};

class ActorAnimator;

class AnimationCubeObserver : public vtkCommand {
public:
  static auto New() -> AnimationCubeObserver* {
    return new AnimationCubeObserver();
  }

  void
  Execute(vtkObject* caller, unsigned long eventId, void* callData) override;
  ActorAnimator* animator_; // ref;
};

class ActorAnimator {
public:
  ActorAnimator() {
    cube_observer_ = vtkSmartPointer<AnimationCubeObserver>::New();
    cube_observer_->animator_ = this;
  }

  ~ActorAnimator() = default;

  void SetActor(vtkSmartPointer<vtkActor> actor) {
    actor_ = std::move(actor);
  }

  void SetStartPosition(std::array<double, 3> start_position) {
    start_position_ = start_position;
  }

  void SetEndPosition(std::array<double, 3> end_position) {
    end_position_ = end_position;
  }

  void AddObserverToCue(const vtkSmartPointer<vtkAnimationCue>& cue) {
    cue->AddObserver(vtkCommand::StartAnimationCueEvent, cube_observer_);
    cue->AddObserver(vtkCommand::EndAnimationCueEvent, cube_observer_);
    cue->AddObserver(vtkCommand::AnimationCueTickEvent, cube_observer_);
  }

  void Start(vtkAnimationCue::AnimationCueInfo*) {
    actor_->SetPosition(start_position_.data());
  }

  void Tick(const vtkAnimationCue::AnimationCueInfo* info) {
    const auto t = (info->AnimationTime - info->StartTime)
                   / (info->EndTime - info->AnimationTime);

    std::array<double, 3> position;
    for (int i = 0; i < start_position_.size(); ++i) {
      position[i]
        = start_position_[i] + (end_position_[i] - start_position_[i]) * t;
    }

    actor_->SetPosition(position.data());
  }

  void End(vtkAnimationCue::AnimationCueInfo*) {
    actor_->SetPosition(end_position_.data());
  }

private:
  vtkSmartPointer<vtkActor> actor_;
  vtkSmartPointer<AnimationCubeObserver> cube_observer_;
  std::array<double, 3> start_position_{0.0};
  std::array<double, 3> end_position_{0.5};
};

void AnimationCubeObserver::Execute(vtkObject* caller, unsigned long eventId,
                                    void* callData) {
  auto info = static_cast<vtkAnimationCue::AnimationCueInfo*>(callData);
  switch (eventId) {
    case vtkCommand::StartAnimationCueEvent:
      animator_->Start(info);
      break;
    case AnimationCueTickEvent:
      animator_->Tick(info);
      break;
    case EndAnimationCueEvent:
      animator_->End(info);
      break;
  }
}

auto main(int argc, char** argv) -> int {
  vtkNew<vtkCubeSource> cube_source;
  vtkNew<vtkPolyDataMapper> cube_mapper;
  cube_mapper->SetInputConnection(cube_source->GetOutputPort());
  vtkNew<vtkActor> cube_actor;
  cube_actor->SetMapper(cube_mapper);

  vtkNew<vtkConeSource> cone_source;
  vtkNew<vtkPolyDataMapper> cone_mapper;
  cone_mapper->SetInputConnection(cone_source->GetOutputPort());
  vtkNew<vtkActor> cone_actor;
  cone_actor->SetMapper(cone_mapper);

  vtkNew<vtkRenderer> render;
  render->AddActor(cube_actor);
  render->AddActor(cone_actor);

  vtkNew<vtkNamedColors> colors;
  cube_actor->GetProperty()->SetColor(colors->GetColor4d("Banana").GetData());
  cone_actor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());
  render->SetBackground(colors->GetColor4d("Peacock").GetData());

  vtkNew<vtkRenderWindow> window;
  window->SetSize(1920, 1080);
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  vtkNew<vtkAnimationScene> scene;
  scene->SetLoop(0);
  scene->SetFrameRate(25);
  scene->SetStartTime(0);
  scene->SetEndTime(20);

  vtkNew<AnimationSceneObserver> scene_observer;
  scene_observer->SetRenderWindow(window);
  scene->AddObserver(vtkCommand::AnimationCueTickEvent, scene_observer);

  vtkNew<vtkAnimationCue> cue_cube;
  cue_cube->SetStartTime(5);
  cue_cube->SetEndTime(23);
  scene->AddCue(cue_cube);

  vtkNew<vtkAnimationCue> cone_cue;
  cone_cue->SetStartTime(1);
  cone_cue->SetEndTime(10);
  scene->AddCue(cone_cue);

  ActorAnimator animator_cube;
  animator_cube.SetActor(cube_actor);
  animator_cube.AddObserverToCue(cue_cube);

  ActorAnimator animator_cone;
  animator_cone.SetEndPosition({-1, -1, -1});
  animator_cone.SetActor(cone_actor);
  animator_cone.AddObserverToCue(cone_cue);

  window->Render();
  render->ResetCamera();
  render->GetActiveCamera()->Dolly(0.8);
  render->ResetCameraClippingRange();

  scene->Play();
  scene->Stop();

  interactor->Start();
}