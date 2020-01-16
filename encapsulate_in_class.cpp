#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

class MyVTKApp {

    using source_type = vtkSmartPointer<vtkConeSource>;
    using render_type = vtkSmartPointer<vtkRenderer>;
    using interactor_type = vtkSmartPointer<vtkRenderWindowInteractor>;
    using mapper_type = vtkSmartPointer<vtkPolyDataMapper>;
    using actor_type = vtkSmartPointer<vtkActor>;
    using render_window_type = vtkSmartPointer<vtkRenderWindow>;
    using interactor_style_type = vtkSmartPointer<vtkInteractorStyleTrackballCamera>;

public:
MyVTKApp() {
    source_ = source_type::New();
    source_->SetHeight(3.0);
    source_->SetRadius(1.0);
    source_->SetResolution(100);
    
    mapper_ = mapper_type::New();
    mapper_->SetInputConnection(source_->GetOutputPort());

    actor_ = actor_type::New();
    actor_->SetMapper(mapper_);
    
    render_ = render_type::New();
    render_->AddActor(actor_);
    render_->SetBackground(0.2, 0.4, 0.3);

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

int main () {
    MyVTKApp app;
    return 0;
}