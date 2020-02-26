#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapper3D.h"
#include "vtkInteractorStyleImage.h"
#include "vtkMetaImageReader.h"
#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "platform.h"

auto main(int argc, char** argv) -> int {
  const auto file_name{
    fs::current_path().append("Data/MetaIO/ChestCT-SHORT.mha").string()};
  vtkNew<vtkMetaImageReader> reader;
  reader->SetFileName(file_name.c_str());
  reader->Update();

  vtkNew<vtkImageActor> actor;
  actor->GetMapper()->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->ResetCamera();

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkInteractorStyleImage> style;
  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetInteractorStyle(style);
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->Initialize();
  interactor->Start();
}