#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 1.0, 0.0);
  points->InsertNextPoint(0.0, 1.0, 0.0);

  vtkNew<vtkPolygon> polygon;
  auto ids = polygon->GetPointIds();
  ids->SetNumberOfIds(points->GetNumberOfPoints());
  for (int i = 0; i < points->GetNumberOfPoints(); ++i) {
    ids->SetId(i, i);
  }

  vtkNew<vtkCellArray> polygons;
  polygons->InsertNextCell(polygon);

  vtkNew<vtkPolyData> polygon_polydata;
  polygon_polydata->SetPoints(points);
  polygon_polydata->SetPolys(polygons);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(polygon_polydata);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Silver").GetData());

  vtkNew<vtkRenderer> render;
  render->AddActor(actor);
  render->SetBackground(colors->GetColor3d("Salmon").GetData());

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);

  window->Render();
  interactor->Start();
}