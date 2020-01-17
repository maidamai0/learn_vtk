#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

namespace {
// source parameter
const double source_heiht = 3.0;
const double source_radius = 1.0;
const double source_resolution = 100;

// backgroud color
const double background_color_r = 0.2;
const double background_color_g = 0.4;
const double background_color_b = 0.3;

} // namespace

class MyVTKApp {
  using source_type = vtkSmartPointer<vtkConeSource>;
  using render_type = vtkSmartPointer<vtkRenderer>;
  using interactor_type = vtkSmartPointer<vtkRenderWindowInteractor>;
  using mapper_type = vtkSmartPointer<vtkPolyDataMapper>;
  using actor_type = vtkSmartPointer<vtkActor>;
  using render_window_type = vtkSmartPointer<vtkRenderWindow>;
  using interactor_style_type
    = vtkSmartPointer<vtkInteractorStyleTrackballCamera>;

public:
  MyVTKApp() {
    source_ = source_type::New();
    source_->SetHeight(source_heiht);
    source_->SetRadius(source_radius);

    source_->SetResolution(source_resolution);

    mapper_ = mapper_type::New();
    mapper_->SetInputConnection(source_->GetOutputPort());

    actor_ = actor_type::New();
    actor_->SetMapper(mapper_);

    render_ = render_type::New();
    render_->AddActor(actor_);
    render_->SetBackground(background_color_r, background_color_g,
                           background_color_b);

    render_window_ = render_window_type::New();
    render_window_->AddRenderer(render_);
    // render_window_->SetParentId(parent);
    render_window_->SetSize(400, 400);

    const auto style = interactor_style_type::New();
    interactor_ = interactor_type::New();
    interactor_->SetInteractorStyle(style);
    interactor_->SetRenderWindow(render_window_);

    render_window_->Render();
    interactor_->Initialize();
    interactor_->Start();
  }
  ~MyVTKApp() = default;

private:
  source_type source_;
  render_window_type render_window_;
  render_type render_;
  interactor_type interactor_;
  mapper_type mapper_;
  actor_type actor_;
};

auto main() -> int {
  MyVTKApp app;
  return 0;
}