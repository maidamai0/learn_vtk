#include "vtkActor.h"
#include "vtkBMPReader.h"
#include "vtkDataSetMapper.h"
#include "vtkDelaunay3D.h"
#include "vtkNew.h"
#include "vtkPointSource.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTexture.h"
#include "vtkTextureMapToCylinder.h"
#include "vtkTransformTextureCoords.h"

#include "platform.h"
#include <filesystem>

int main() {
  vtkNew<vtkPointSource> points;
  points->SetNumberOfPoints(25);

  vtkNew<vtkDelaunay3D> delny;
  delny->SetInputConnection(points->GetOutputPort());
  delny->SetTolerance(0.01);

  vtkNew<vtkTextureMapToCylinder> texture_mapper;
  texture_mapper->SetInputConnection(delny->GetOutputPort());
  texture_mapper->PreventSeamOn();

  vtkNew<vtkTransformTextureCoords> xform;
  xform->SetInputConnection(texture_mapper->GetOutputPort());
  xform->SetScale(4, 4, 1);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputConnection(xform->GetOutputPort());

  vtkNew<vtkBMPReader> bmp_reader;
  bmp_reader->SetFileName(
    (fs::canonical("Data").string() + "/masonry.bmp").c_str());

  vtkNew<vtkTexture> texture;
  texture->SetInputConnection(bmp_reader->GetOutputPort());
  texture->InterpolateOn();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->SetTexture(texture);

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(1, 1, 1);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->Initialize();
  interactor->Start();
}