#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapper.h"
#include "vtkImageMapper3D.h"
#include "vtkImageReader2.h"
#include "vtkImageReader2Factory.h"
#include "vtkInteractorStyleImage.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

#include "fmt/core.h"
#include "platform.h"

#include <filesystem>
#include <string>

auto main(int argc, char** argv) -> int {
  std::string file_name{
    fs::current_path().append("Data/GreenCircle.png").string()};
  fmt::print("file is {}\n", file_name);

  const auto reader_factory = vtkSmartPointer<vtkImageReader2Factory>::New();
  auto reader = vtkSmartPointer<vtkImageReader2>::New();
  reader.TakeReference(reader_factory->CreateImageReader2(file_name.c_str()));
  reader->SetFileName(file_name.c_str());
  reader->Update();

  vtkNew<vtkImageActor> actor;
  actor->GetMapper()->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->ResetCamera();
  render->SetBackground(colors->GetColor3d("Slate_grey").GetData());

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkInteractorStyleImage> image_style;
  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->SetInteractorStyle(image_style);
  interactor->Initialize();
  interactor->Start();
}