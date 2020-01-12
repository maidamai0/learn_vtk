#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

class vtkMyCallback : public vtkCommand {
public:
  static auto New() -> vtkMyCallback* {
    return new vtkMyCallback();
  }

  void Execute(vtkObject* caller, unsigned long, void*) override {
    const auto render = reinterpret_cast<vtkRenderer*>(caller);
    const auto camera_pos = render->GetActiveCamera()->GetPosition();

    std::cout << camera_pos[0] << " " << camera_pos[1] << " " << camera_pos[2]
              << std::endl;
  }
};

auto main() -> int {
  const auto cone_source = vtkConeSource::New();
  cone_source->SetResolution(500);

  const auto cone_mapper = vtkPolyDataMapper::New();
  cone_mapper->SetInputConnection(cone_source->GetOutputPort());

  const auto cone_actor = vtkActor::New();
  cone_actor->SetMapper(cone_mapper);

  const auto render = vtkRenderer::New();
  render->AddActor(cone_actor);

  const auto wind = vtkRenderWindow::New();
  wind->AddRenderer(render);

  const auto call_back = vtkMyCallback::New();
  render->AddObserver(vtkCommand::StartEvent, call_back);
  call_back->Delete();

  for (int i = 0; i < 360; ++i) {
    wind->Render();
    render->GetActiveCamera()->Azimuth(1);
  }

  return 0;
}