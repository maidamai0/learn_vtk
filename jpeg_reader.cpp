#include "vtkImageViewer2.h"
#include "vtkJPEGReader.h"
#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "platform.h"
#include <filesystem>
#include <string>

auto main(int argc, char** argv) -> int {
  const auto file_name{
    fs::current_path().append("Data/Infovis/Images/NE2_ps_bath.jpg").string()};
  vtkNew<vtkJPEGReader> jpeg_reader;
  jpeg_reader->SetFileName(file_name.c_str());

  vtkNew<vtkImageViewer2> image_viewer;
  image_viewer->SetInputConnection(jpeg_reader->GetOutputPort());

  vtkNew<vtkRenderWindowInteractor> interactor;
  image_viewer->SetupInteractor(interactor);
  image_viewer->Render();
  image_viewer->GetRenderer()->ResetCamera();
  image_viewer->Render();

  interactor->Start();
}