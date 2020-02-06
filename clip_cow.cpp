#include "vtkActor.h"
#include "vtkBYUReader.h"
#include "vtkClipPolyData.h"
#include "vtkCutter.h"
#include "vtkNew.h"
#include "vtkPlane.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkStripper.h"
#include "vtkTriangleFilter.h"

#include "platform.h"
#include <filesystem>

int main() {
  vtkNew<vtkBYUReader> reader;
  reader->SetGeometryFileName(
    (fs::canonical("Data").string() + "/Viewpoint/cow.g").c_str());

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkPlane> plane;
  plane->SetOrigin(0.25, 0, 0);
  plane->SetNormal(-1, -1, 0);

  vtkNew<vtkClipPolyData> clip;
  clip->SetInputConnection(normals->GetOutputPort());
  clip->SetClipFunction(plane);
  clip->GenerateClipScalarsOn();
  clip->GenerateClippedOutputOn();
  clip->SetValue(0.9);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(clip->GetOutputPort());
  mapper->ScalarVisibilityOff();

  vtkNew<vtkProperty> property;
  property->SetDiffuseColor(1, 0, 0);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0, 1, 0);
  actor->SetBackfaceProperty(property);

  vtkNew<vtkCutter> cutter;
  cutter->SetInputConnection(normals->GetOutputPort());
  cutter->SetCutFunction(plane);
  cutter->GenerateCutScalarsOn();
  cutter->SetValue(0, 0.9);

  vtkNew<vtkStripper> stripper;
  stripper->SetInputConnection(cutter->GetOutputPort());
  stripper->Update();

  vtkNew<vtkPolyData> cut_poly;
  cut_poly->SetPoints(stripper->GetOutput()->GetPoints());
  cut_poly->SetPolys(stripper->GetOutput()->GetPolys());

  vtkNew<vtkTriangleFilter> cut_triangles;
  cut_triangles->SetInputData(cut_poly);

  vtkNew<vtkPolyDataMapper> cut_mapper;
  cut_mapper->SetInputConnection(cut_triangles->GetOutputPort());
  cut_mapper->SetInputData(cut_poly);

  vtkNew<vtkActor> cut_actor;
  cut_actor->SetMapper(cut_mapper);

  vtkNew<vtkPolyDataMapper> rest_mapper;
  rest_mapper->SetInputConnection(clip->GetClippedOutputPort());
  rest_mapper->ScalarVisibilityOff();

  vtkNew<vtkActor> rest_actor;
  rest_actor->SetMapper(rest_mapper);
  rest_actor->GetProperty()->SetRepresentationToWireframe();

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->AddActor(cut_actor);
  render->AddActor(rest_actor);
  render->SetBackground(1, 1, 1);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);
  window->Render();

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  interactor->Start();
}