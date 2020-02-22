#include "vtkAVIWriter.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCubeSource.h"
#include "vtkGraphicsFactory.h"
#include "vtkNew.h"
#include "vtkPNGWriter.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkWindowToImageFilter.h"

#include "platform.h"
#include <chrono>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

int main() {
  vtkNew<vtkCubeSource> source;
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(source->GetOutputPort());
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(1, 0, 0);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(0, 0, 0);

  vtkNew<vtkRenderWindow> window;
  window->OffScreenRenderingOn(); // don't show window.
  window->AddRenderer(render);
  window->SetSize(640, 480);

  vtkNew<vtkWindowToImageFilter> to_image;
  to_image->SetInputBufferTypeToRGB();
  to_image->ReadFrontBufferOff();
  to_image->SetInput(window);

  vtkNew<vtkPNGWriter> png_writer;
  png_writer->SetInputConnection(to_image->GetOutputPort());

  try {
    fs::create_directory(fs::current_path().append("build/image"));
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  vtkNew<vtkAVIWriter> avi_writer;
  avi_writer->SetRate(60);   // 36 frames in total, 3 seconds.
  avi_writer->SetQuality(2); // best quality
  avi_writer->SetInputConnection(to_image->GetOutputPort());
  avi_writer->SetFileName("build/image/off_screen.avi");
  avi_writer->Start();

  std::string base_name{"build/image/off_screen_render_"};
  for (auto i = 0; i < 360; ++i) {
    window->Render();

    to_image->Modified();
    to_image->Update();

    avi_writer->Write();

    png_writer->SetFileName((base_name + std::to_string(i) + ".png").c_str());
    png_writer->Write();

    render->GetActiveCamera()->Azimuth(1);
  }

  avi_writer->End();
}