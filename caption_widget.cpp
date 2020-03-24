#include <array>
#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkFileOutputWindow.h>
#include <vtkNamedColors.h>
#include <vtkOutputWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkFileOutputWindow> output_file;
  output_file->SetFileName("vtk.log");
  vtkOutputWindow::SetInstance(output_file);

  vtkNew<vtkSphereSource> sphere_source;
  sphere_source->SetPhiResolution(50);
  sphere_source->SetThetaResolution(50);
  sphere_source->SetRadius(1);
  sphere_source->Update();

  vtkNew<vtkPolyDataMapper> sphere_mapper;
  sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

  vtkNew<vtkActor> sphere_actor;
  sphere_actor->SetMapper(sphere_mapper);

  vtkNew<vtkRenderer> render;
  sphere_actor->SetPosition(0.6, 0.6, 0);
  render->AddActor(sphere_actor);
  vtkNew<vtkNamedColors> colors;
  render->SetBackground(colors->GetColor3d("DarkSalmon").GetData());

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->DebugOn();

  vtkNew<vtkCaptionRepresentation> caption_rep;
  caption_rep->GetCaptionActor2D()->SetCaption("Test caption");
  caption_rep->GetCaptionActor2D()
    ->GetTextActor()
    ->GetTextProperty()
    ->SetFontSize(100);

  std::array<double, 3> pos{0.7, 0.7, 0.0};
  caption_rep->SetAnchorPosition(pos.data());

  vtkNew<vtkCaptionWidget> caption_widget;
  caption_widget->SetInteractor(interactor);
  caption_widget->SetRepresentation(caption_rep);

  window->Render();
  caption_widget->On();

  interactor->Start();
}