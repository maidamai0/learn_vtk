#include "vtkDICOMImageReader.h"
#include "vtkImageViewer2.h"
#include "vtkNew.h"
#include "vtkRenderWindowInteractor.h"

#include "platform.h"
#include <filesystem>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkDICOMImageReader> reader;
  reader->SetFileName(
    fs::current_path().append("Data/prostate.img").string().c_str());
  reader->Update();

  vtkNew<vtkImageViewer2> viewer;
  viewer->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkRenderWindowInteractor> interacotr;
  viewer->SetupInteractor(interacotr);
  viewer->Render();
  interacotr->Start();
}