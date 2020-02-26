#include "vtkImageActor.h"
#include "vtkImageMapper3D.h"
#include "vtkInteractorStyleImage.h"
#include "vtkNew.h"
#include "vtkPNGReader.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "platform.h"

auto main(int argc, char** argv) -> int {
  const auto file_name{
    fs::current_path().append("Data/BlueCircle.png").string()};

  vtkNew<vtkPNGReader> reader;
  reader->SetFileName(file_name.c_str());
  reader->Update();

  vtkNew<vtkImageActor> actor;
  actor->GetMapper()->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkInteractorStyleImage> style;
  vtkNew<vtkRenderWindowInteractor> interacotr;
  interacotr->SetRenderWindow(window);
  interacotr->SetInteractorStyle(style);
  window->Render();
  interacotr->Initialize();
  interacotr->Start();
}