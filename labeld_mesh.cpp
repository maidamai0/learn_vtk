#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkCellArray.h"
#include "vtkCellCenters.h"
#include "vtkIdFilter.h"
#include "vtkLabeledDataMapper.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSelectVisiblePoints.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include <iostream>

namespace {
double x_length = 100.0;
double y_length = 100.0;

vtkSmartPointer<vtkSelectVisiblePoints> visible_points;
vtkSmartPointer<vtkSelectVisiblePoints> visible_cells;
vtkSmartPointer<vtkPoints> points;
vtkSmartPointer<vtkRenderWindow> render_window;
}; // namespace

void place_window(const double x_min, const double y_min) {
  const auto x_max = x_min + x_length;
  const auto y_max = y_min + y_length;

  visible_points->SetSelection(x_min, x_max, y_min, y_max);
  visible_cells->SetSelection(x_min, x_max, y_min, y_max);

  points->InsertPoint(0, x_min, y_min, 0.0);
  points->InsertPoint(1, x_min, y_min, 0.0);
  points->InsertPoint(2, x_min, y_min, 0.0);
  points->InsertPoint(3, x_min, y_min, 0.0);

  points->Modified();
  render_window->Render();
}

void move_window() {
  for (int y = 100; y < 300; y += 25) {
    for (int x = 100; x < 300; x += 25) {
      place_window(y, y);
    }
  }
}

int main() {
  double x_min = 200;
  double x_max = x_min + x_length;

  double y_min = 200;
  double y_max = x_min + y_length;

  points = vtkSmartPointer<vtkPoints>::New();
  points->InsertPoint(0, x_min, y_min, 0.0);
  points->InsertPoint(1, x_max, y_min, 0.0);
  points->InsertPoint(2, x_max, y_max, 0.0);
  points->InsertPoint(3, x_min, y_max, 0.0);

  const auto rectangle = vtkSmartPointer<vtkCellArray>::New();
  rectangle->InsertNextCell(5);
  rectangle->InsertNextCell(0);
  rectangle->InsertNextCell(1);
  rectangle->InsertNextCell(2);
  rectangle->InsertNextCell(3);
  rectangle->InsertNextCell(0);

  const auto select_rectangle = vtkSmartPointer<vtkPolyData>::New();
  select_rectangle->SetPoints(points);
  select_rectangle->SetLines(rectangle);

  const auto rectange_mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
  rectange_mapper->SetInputData(select_rectangle);

  const auto rectange_actor = vtkSmartPointer<vtkActor2D>::New();
  rectange_actor->SetMapper(rectange_mapper);

  const auto sphere = vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetThetaResolution(10);
  sphere->SetPhiResolution(10);
  const auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphere_mapper->SetInputConnection(sphere->GetOutputPort());

  const auto sphere_actor = vtkSmartPointer<vtkActor>::New();
  sphere_actor->SetMapper(sphere_mapper);

  const auto ids = vtkSmartPointer<vtkIdFilter>::New();
  ids->SetInputConnection(sphere->GetOutputPort());
  ids->PointIdsOn();
  ids->CellIdsOn();
  ids->FieldDataOn();

  const auto render = vtkSmartPointer<vtkRenderer>::New();

  visible_points = vtkSmartPointer<vtkSelectVisiblePoints>::New();
  visible_points->SetInputConnection(ids->GetOutputPort());
  visible_points->SetRenderer(render);
  visible_points->SelectionWindowOn();
  visible_points->SetSelection(x_min, x_max, y_min, y_max);

  const auto label_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
  label_mapper->SetInputConnection(visible_points->GetOutputPort());
  label_mapper->SetLabelModeToLabelFieldData();
  const auto label_actor = vtkSmartPointer<vtkActor2D>::New();
  label_actor->SetMapper(label_mapper);

  const auto cell_centers = vtkSmartPointer<vtkCellCenters>::New();
  cell_centers->SetInputConnection(ids->GetOutputPort());

  visible_cells = vtkSmartPointer<vtkSelectVisiblePoints>::New();
  visible_cells->SetInputConnection(cell_centers->GetOutputPort());
  visible_cells->SetRenderer(render);
  visible_cells->SelectionWindowOn();
  visible_cells->SetSelection(x_min, x_max, y_min, y_max);

  const auto cell_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
  cell_mapper->SetInputConnection(visible_cells->GetOutputPort());
  cell_mapper->SetLabelModeToLabelFieldData();
  cell_mapper->GetLabelTextProperty()->SetColor(0, 1, 0);

  const auto cell_actor = vtkSmartPointer<vtkActor2D>::New();
  cell_actor->SetMapper(cell_mapper);

  render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render->SetBackground(1, 1, 1);
  render->AddActor(sphere_actor);
  render->AddActor(rectange_actor);
  render->AddActor(label_actor);
  render->AddActor(cell_actor);

  interactor->Initialize();
  render_window->Render();
  move_window();
  place_window(100, 100);
  interactor->Start();
}