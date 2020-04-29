#include "vtkActor.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkType.h"
#include "vtkUnsignedCharArray.h"
#include "vtkVertexGlyphFilter.h"

auto main(int argc, char** argv) -> int {
  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 0.0, 0.0);
  points->InsertNextPoint(0.0, 1.0, 0.0);

  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points);

  vtkNew<vtkVertexGlyphFilter> filter;
  filter->SetInputData(polydata);
  filter->Update();

  vtkNew<vtkPolyData> colored_polydata;
  colored_polydata->ShallowCopy(filter->GetOutput());

  vtkNew<vtkNamedColors> named_colors;

  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");
  colors->InsertNextTypedTuple(named_colors->GetColor3ub("Tomato").GetData());
  colors->InsertNextTypedTuple(named_colors->GetColor3ub("Mint").GetData());
  colors->InsertNextTypedTuple(named_colors->GetColor3ub("Peacock").GetData());

  colored_polydata->GetPointData()->SetScalars(colors);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(colored_polydata);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(20); // TODO why 10?

  vtkNew<vtkRenderer> render;
  //   render->SetBackground(named_colors->GetColor3d("Burlywood").GetData());
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();

  window->Render();
  interactor->Start();
}