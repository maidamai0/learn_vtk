#include "vtkActor.h"
#include "vtkAnimationCue.h"
#include "vtkAnimationScene.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSetGet.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include <iostream>

class MyAnimationCue : public vtkAnimationCue {
public:
  static MyAnimationCue* New() {
    return new MyAnimationCue();
  }

  //   vtkTypeMacro(MyAnimationCue, vtkAnimationCue);

  vtkSmartPointer<vtkRenderWindow> render_window;
  vtkSmartPointer<vtkSphereSource> sphere;

private:
  MyAnimationCue() = default;
  ~MyAnimationCue() override = default;

  void TickInternal(double current_time, double delta_time,
                    double clock_time) override {
    std::cout << "current time is " << current_time << "\n";
    const auto start = current_time * 360;
    this->sphere->SetStartTheta(start);
    this->render_window->Render();
  }
};

int main() {
  const auto sphere = vtkSmartPointer<vtkSphereSource>::New();
  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(sphere->GetOutputPort());
  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->ResetCamera();

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->SetMultiSamples(0);
  render_window->AddRenderer(render);
  render_window->Render();

  const auto animation_scene = vtkSmartPointer<vtkAnimationScene>::New();
  animation_scene->SetModeToSequence();
  animation_scene->SetFrameRate(30);
  animation_scene->SetStartTime(0);
  animation_scene->SetEndTime(60);

  const auto cue = vtkSmartPointer<MyAnimationCue>::New();
  cue->sphere = sphere;
  cue->render_window = render_window;
  cue->SetTimeModeToNormalized();
  cue->SetStartTime(0);
  cue->SetEndTime(1);

  animation_scene->AddCue(cue);

  animation_scene->Play();
  animation_scene->Stop();
}