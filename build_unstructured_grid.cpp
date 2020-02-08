
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkDataSetMapper.h"
#include "vtkHexahedron.h"
#include "vtkIdList.h"
#include "vtkImageData.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPixel.h"
#include "vtkPoints.h"
#include "vtkProperty.h"
#include "vtkPyramid.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTetra.h"
#include "vtkUnstructuredGrid.h"
#include "vtkVoxel.h"
#include "vtkWedge.h"

auto create_voxel_actor() {
  vtkNew<vtkPoints> voxel_points;
  voxel_points->SetNumberOfPoints(8);
  voxel_points->InsertPoint(0, 0, 0, 0);
  voxel_points->InsertPoint(1, 1, 0, 0);
  voxel_points->InsertPoint(2, 0, 1, 0);
  voxel_points->InsertPoint(3, 1, 1, 0);
  voxel_points->InsertPoint(4, 0, 0, 1);
  voxel_points->InsertPoint(5, 1, 0, 1);
  voxel_points->InsertPoint(6, 0, 1, 1);
  voxel_points->InsertPoint(7, 1, 1, 1);

  vtkNew<vtkVoxel> voxel;
  voxel->GetPointIds()->SetId(0, 0);
  voxel->GetPointIds()->SetId(1, 1);
  voxel->GetPointIds()->SetId(2, 2);
  voxel->GetPointIds()->SetId(3, 3);
  voxel->GetPointIds()->SetId(4, 4);
  voxel->GetPointIds()->SetId(5, 5);
  voxel->GetPointIds()->SetId(6, 6);
  voxel->GetPointIds()->SetId(7, 7);

  vtkNew<vtkUnstructuredGrid> grid;
  grid->Allocate(1, 1);
  grid->InsertNextCell(voxel->GetCellType(), voxel->GetPointIds());
  grid->SetPoints(voxel_points);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(grid);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetDiffuseColor(1, 0, 0);

  return actor;
}

auto create_hexahedron() {
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(8);
  points->InsertPoint(0, 0, 0, 0);
  points->InsertPoint(1, 1, 0, 0);
  points->InsertPoint(2, 1, 1, 0);
  points->InsertPoint(3, 0, 1, 0);
  points->InsertPoint(4, 0, 0, 1);
  points->InsertPoint(5, 1, 0, 1);
  points->InsertPoint(6, 1, 1, 1);
  points->InsertPoint(7, 0, 1, 1);

  vtkNew<vtkHexahedron> hexahedron;
  hexahedron->GetPointIds()->SetId(0, 0);
  hexahedron->GetPointIds()->SetId(1, 1);
  hexahedron->GetPointIds()->SetId(2, 2);
  hexahedron->GetPointIds()->SetId(3, 3);
  hexahedron->GetPointIds()->SetId(4, 4);
  hexahedron->GetPointIds()->SetId(5, 5);
  hexahedron->GetPointIds()->SetId(6, 6);
  hexahedron->GetPointIds()->SetId(7, 7);

  vtkNew<vtkUnstructuredGrid> grid;
  grid->Allocate(1, 1);
  grid->InsertNextCell(hexahedron->GetCellType(), hexahedron->GetPointIds());
  grid->SetPoints(points);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(grid);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->AddPosition(2, 0, 0);
  actor->GetProperty()->SetDiffuseColor(1, 1, 0);

  return actor;
}

auto create_tetra() {
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(4);
  points->InsertPoint(0, 0, 0, 0);
  points->InsertPoint(1, 1, 0, 0);
  points->InsertPoint(2, 0.5, 1, 0);
  points->InsertPoint(3, 0.5, 0.5, 1);

  vtkNew<vtkHexahedron> geometry;
  geometry->GetPointIds()->SetId(0, 0);
  geometry->GetPointIds()->SetId(1, 1);
  geometry->GetPointIds()->SetId(2, 2);
  geometry->GetPointIds()->SetId(3, 3);

  vtkNew<vtkUnstructuredGrid> grid;
  grid->Allocate(1, 1);
  grid->InsertNextCell(geometry->GetCellType(), geometry->GetPointIds());
  grid->SetPoints(points);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(grid);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->AddPosition(4, 0, 0);
  actor->GetProperty()->SetDiffuseColor(0, 1, 0);

  return actor;
}

auto create_wedge() {
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(6);
  points->InsertPoint(0, 0, 1, 0);
  points->InsertPoint(1, 0, 0, 0);
  points->InsertPoint(2, 0, 0.5, 0.5);
  points->InsertPoint(3, 1, 1, 0);
  points->InsertPoint(4, 1, 0, 0);
  points->InsertPoint(5, 1, 0.5, 0.5);

  vtkNew<vtkWedge> geometry;
  geometry->GetPointIds()->SetId(0, 0);
  geometry->GetPointIds()->SetId(1, 1);
  geometry->GetPointIds()->SetId(2, 2);
  geometry->GetPointIds()->SetId(3, 3);
  geometry->GetPointIds()->SetId(4, 4);
  geometry->GetPointIds()->SetId(5, 5);

  vtkNew<vtkUnstructuredGrid> grid;
  grid->Allocate(1, 1);
  grid->InsertNextCell(geometry->GetCellType(), geometry->GetPointIds());
  grid->SetPoints(points);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(grid);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->AddPosition(6, 0, 0);
  actor->GetProperty()->SetDiffuseColor(0, 1, 1);

  return actor;
}

auto create_pyramid() {
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(5);
  points->InsertPoint(0, 0, 0, 0);
  points->InsertPoint(1, 1, 0, 0);
  points->InsertPoint(2, 1, 1, 0);
  points->InsertPoint(3, 0, 1, 0);
  points->InsertPoint(4, 0.5, 0.5, 1);

  vtkNew<vtkPyramid> geometry;
  geometry->GetPointIds()->SetId(0, 0);
  geometry->GetPointIds()->SetId(1, 1);
  geometry->GetPointIds()->SetId(2, 2);
  geometry->GetPointIds()->SetId(3, 3);
  geometry->GetPointIds()->SetId(4, 4);

  vtkNew<vtkUnstructuredGrid> grid;
  grid->Allocate(1, 1);
  grid->InsertNextCell(geometry->GetCellType(), geometry->GetPointIds());
  grid->SetPoints(points);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(grid);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->AddPosition(8, 0, 0);
  actor->GetProperty()->SetDiffuseColor(1, 0, 1);

  return actor;
}

auto create_pixel() {
  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(4);
  points->InsertPoint(0, 0, 0, 0);
  points->InsertPoint(1, 1, 0, 0);
  points->InsertPoint(2, 0, 1, 0);
  points->InsertPoint(3, 1, 1, 0);

  vtkNew<vtkPyramid> geometry;
  geometry->GetPointIds()->SetId(0, 0);
  geometry->GetPointIds()->SetId(1, 1);
  geometry->GetPointIds()->SetId(2, 2);
  geometry->GetPointIds()->SetId(3, 3);

  vtkNew<vtkUnstructuredGrid> grid;
  grid->Allocate(1, 1);
  grid->InsertNextCell(geometry->GetCellType(), geometry->GetPointIds());
  grid->SetPoints(points);

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(grid);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->AddPosition(0, -2, 0);
  actor->GetProperty()->SetDiffuseColor(0, 1, 1);

  return actor;
}

int main() {
  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkRenderer> render;
  render->AddActor(create_voxel_actor());
  render->AddActor(create_hexahedron());
  render->AddActor(create_tetra());
  render->AddActor(create_wedge());
  render->AddActor(create_pyramid());
  render->AddActor(create_pixel());
  render->SetBackground(colors->GetColor3d("Cornsilk").GetData());

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  window->Render();
  interactor->Start();
}