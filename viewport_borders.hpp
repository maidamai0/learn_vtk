#include "vtkActor2D.h"
#include "vtkCellArray.h"
#include "vtkCoordinate.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkPolyLine.h"
#include "vtkProperty2D.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

void viewport_borders(const vtkSmartPointer<vtkRenderer>& renderer,
                      double* color, bool last = false) {
  auto points = vtkSmartPointer<vtkPoints>::New();
  points->SetNumberOfPoints(4);
  points->InsertPoint(0, 1, 1, 0);
  points->InsertPoint(1, 0, 1, 0);
  points->InsertPoint(2, 0, 0, 0);
  points->InsertPoint(3, 1, 0, 0);

  auto line = vtkSmartPointer<vtkPolyLine>::New();
  auto point_ids = line->GetPointIds();
  auto num_of_ids = 4;
  if (last) {
    num_of_ids = 5;
  }

  point_ids->SetNumberOfIds(num_of_ids);

  for (auto i = 0; i < 4; ++i) {
    line->GetPointIds()->SetId(i, i);
  }

  if (last) {
    line->GetPointIds()->SetId(4, 0);
  }

  auto cells = vtkSmartPointer<vtkCellArray>::New();
  cells->Initialize();
  cells->InsertNextCell(line);

  auto poly = vtkSmartPointer<vtkPolyData>::New();
  poly->Initialize();
  poly->SetPoints(points);
  poly->SetLines(cells);

  auto coord = vtkSmartPointer<vtkCoordinate>::New();
  coord->SetCoordinateSystemToNormalizedViewport();

  auto mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
  mapper->SetInputData(poly);
  mapper->SetTransformCoordinate(coord);

  auto actor = vtkSmartPointer<vtkActor2D>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(color);
  actor->GetProperty()->SetLineWidth(4.0);

  renderer->AddViewProp(actor);
}